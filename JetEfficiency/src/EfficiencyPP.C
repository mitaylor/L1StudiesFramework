/*
Input:  L1Ntuple made with one of the following menu versions
    L1Menu_CollisionsHeavyIons2018_v4_2_0.xml --> Menu::Y2018_V4_2_0
    L1Menu_CollisionsHeavyIons2022_v0_0_0.xml --> Menu::Y2022_V0_0_0
    L1Menu_CollisionsHeavyIons2022_v0_0_1.xml --> Menu::Y2022_V0_0_1
    L1Menu_CollisionsHeavyIons2022_v0_0_2.xml --> Menu::Y2022_V0_0_2
    L1Menu_CollisionsHeavyIons2022_v0_0_3.xml --> Menu::Y2022_V0_0_3
    L1Menu_CollisionsHeavyIons2022_v0_0_4.xml --> Menu::Y2022_V0_0_4
Output: A list of how many times each menu trigger was passed
Note: If you are using your own custom menu, add it to Menu.h following the instructions in data/README
*/

#include "../include/Menu.h"

#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

#include "TDirectory.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TChain.h"

#include "TMath.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

double dr(float eta1, float phi1, float eta2, float phi2) {
    float deta = TMath::Abs(eta1 - eta2);
    float dphi = TMath::Abs(phi1 - phi2);
    if (dphi > TMath::Pi()) dphi = TMath::Abs(dphi - 2*TMath::Pi());

    return TMath::Sqrt(dphi*dphi + deta*deta);
}

void GetFiles(char const* input, vector<string>& files) {
    TSystemDirectory dir(input, input);
    TList *list = dir.GetListOfFiles();

    if (list) {
        TSystemFile *file;
        string fname;
        TIter next(list);
        while ((file = (TSystemFile*) next())) {
            fname = file->GetName();

            if (file->IsDirectory() && (fname.find(".") == string::npos)) {
                string newDir = string(input) + fname + "/";
                GetFiles(newDir.c_str(), files);
            }
            else if ((fname.find(".root") != string::npos)) {
                files.push_back(string(input) + fname);
                cout << files.back() << endl;
            }
        }
    }

    return;
}

void FillChain(TChain& chain, vector<string>& files) {
    for (auto file : files) {
        chain.Add(file.c_str());
    }
}

int Efficiency(char const* input) {
    /* initilaize menu */
    Menu menu(Menu::Y2022_V1_1_0);

    vector<string> files;
    GetFiles(input, files);

    /* read in calo jet information */
    TChain offChain("l1JetRecoTree/JetRecoTree");
    FillChain(offChain, files);
    TTreeReader offReader(&offChain);
    TTreeReaderValue<vector<float>> caloJetPt(offReader, "caloEt");
    TTreeReaderValue<vector<float>> caloJetEta(offReader, "caloEta");
    TTreeReaderValue<vector<float>> caloJetPhi(offReader, "caloPhi");

    /* read in emulated jet information */
    TChain emuChain("l1UpgradeEmuTree/L1UpgradeTree");
    FillChain(emuChain, files);
    TTreeReader emuReader(&emuChain);
    TTreeReaderValue<vector<float>> emuJetPt(emuReader, "jetEt");
    TTreeReaderValue<vector<float>> emuJetEta(emuReader, "jetEta");
    TTreeReaderValue<vector<float>> emuJetPhi(emuReader, "jetPhi");

    string seed = "L1_SingleJet120_BptxAND";
    float threshold = 120;

    /* create histograms for efficiency plots */
    int nbins = 100;
    float min = 0;
    float max = 200;

    TH1F emuHist("emuHist", "", nbins, min, max);
    TH1F emuMatchedHist("emuMatchedHist", "", nbins, min, max);
    TH1F caloHist("caloHist", "", nbins, min, max);

    Long64_t totalEvents = emuReader.GetEntries(true);

    /* read in information from TTrees */
    for (Long64_t i = 0; i < totalEvents; i++) {
        emuReader.Next(); offReader.Next();

        if (i % 20000 == 0) { 
            cout << "Entry: " << i << " / " <<  totalEvents << endl; 
        }

        float caloMaxJetPt = -999;
        float caloMaxJetPhi = -999;
        float caloMaxJetEta = -999;

        float emuMaxJetPt = -999;
        float emuMatchedJetPt = -999;
        float minDR = 10;

        /* iterate through calo jets and find the jet with max pT */
        for (size_t i = 0; i < (*caloJetPt).size(); ++i) {
            if (TMath::Abs((*caloJetEta)[i]) > 3) { continue; }

            if ((*caloJetPt)[i] > caloMaxJetPt) {
                caloMaxJetPt = (*caloJetPt)[i];
                caloMaxJetPhi = (*caloJetEta)[i];
                caloMaxJetEta = (*caloJetPhi)[i];
            }
        }

        if (caloMaxJetPt > 0) {
            caloHist.Fill(caloMaxJetPt);

            /* iterate through emu jets and find matched and unmatched jets with max pT */
            for (size_t i = 0; i < (*emuJetPt).size(); ++i) {
                if ((*emuJetPt)[i] > emuMaxJetPt) {
                    emuMaxJetPt = (*emuJetPt)[i];
                }

                if (dr((*emuJetEta)[i], (*emuJetPhi)[i], caloMaxJetPhi, caloMaxJetEta) < minDR) {
                    minDR = dr((*emuJetEta)[i], (*emuJetPhi)[i], caloMaxJetPhi, caloMaxJetEta);
                    emuMatchedJetPt = (*emuJetPt)[i];
                }
            }

            if (emuMaxJetPt >= threshold) {
                emuHist.Fill(caloMaxJetPt);
            }

            if (emuMatchedJetPt >= threshold && minDR < 0.4) {
                emuMatchedHist.Fill(caloMaxJetPt);
            }
        }
    }

    TGraphAsymmErrors emuCaloEff(&emuHist, &caloHist, "cl=0.683 b(1,1) mode");
    TGraphAsymmErrors emuCaloMatchedEff(&emuMatchedHist, &caloHist, "cl=0.683 b(1,1) mode");

    /* plot the turn ons vs calo jet pt */
    TCanvas caloCanvas("caloCanvas", "", 0, 0, 500, 500);
    caloCanvas.cd();

    emuCaloMatchedEff.GetXaxis()->SetTitle("Calo Jet pT (GeV)");
    emuCaloMatchedEff.GetXaxis()->CenterTitle(true);
    emuCaloMatchedEff.GetYaxis()->SetTitle("Efficiency");
    emuCaloMatchedEff.GetYaxis()->CenterTitle(true);

    emuCaloMatchedEff.SetMarkerColor(46);
    emuCaloMatchedEff.SetLineColor(46);
    emuCaloMatchedEff.SetMarkerSize(0.5);
    emuCaloMatchedEff.SetMarkerStyle(20);
    emuCaloMatchedEff.Draw();

    emuCaloEff.SetMarkerColor(30);
    emuCaloEff.SetLineColor(30);
    emuCaloEff.SetMarkerSize(0.5);
    emuCaloEff.SetMarkerStyle(20);
    emuCaloEff.Draw("LP SAME");

    TLegend caloLegend(0.53, 0.12 ,0.88, 0.3);
    caloLegend.SetTextSize(0.03);
    caloLegend.SetHeader(seed.c_str());
    caloLegend.AddEntry(&emuCaloEff, "Not #DeltaR Matched", "lep");
    caloLegend.AddEntry(&emuCaloMatchedEff, "#DeltaR Matched", "lep");
    caloLegend.Draw();

    caloCanvas.SaveAs("caloEfficiency.pdf");

    /* save histograms to file so I can look at them */
    TFile* fout = new TFile("histograms.root", "recreate");

    emuHist.Write();
    emuMatchedHist.Write();
    caloHist.Write();

    fout->Close();
   
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 2)
        return Efficiency(argv[1]);
    else {
        cout << "ERROR" << endl;
        return -1;
    }
}
