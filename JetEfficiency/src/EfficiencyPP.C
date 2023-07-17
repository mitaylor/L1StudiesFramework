/*
Input: Folder of L1Ntuples
Output: A plot of the jet turn-ons with and with out L1 dR matching vs calo jet pT
*/

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
    /* read in all files in the input folder */
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
                caloMaxJetEta = (*caloJetEta)[i];
                caloMaxJetPhi = (*caloJetPhi)[i];
            }
        }

        if (caloMaxJetPt > 0) {
            caloHist.Fill(caloMaxJetPt);

            /* iterate through emu jets and find matched and unmatched jets with max pT */
            for (size_t i = 0; i < (*emuJetPt).size(); ++i) {
                if ((*emuJetPt)[i] > emuMaxJetPt) {
                    emuMaxJetPt = (*emuJetPt)[i];
                }

                if (dr((*emuJetEta)[i], (*emuJetPhi)[i], caloMaxJetEta, caloMaxJetPhi) < minDR) {
                    minDR = dr((*emuJetEta)[i], (*emuJetPhi)[i], caloMaxJetEta, caloMaxJetPhi);
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
