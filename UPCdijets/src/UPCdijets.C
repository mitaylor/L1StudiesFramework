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

int UPCdijets(char const* input= "/afs/cern.ch/work/g/ginnocen/public/CMSwork/myUPCMCsamplesRun3/20230729_UPCdijet/20230729_UPCdijet_CMSSW_13_1_0_pre4_L1Forest/",
//int UPCdijets(char const* input= "/eos/user/g/ginnocen/CMSRun3/ZeroBias_HIRun2018A_MiniAOD_Run326776_131X_HF_14_19_v1/230701_025938/0000/",
//int UPCdijets(char const* input= "data/",
	      bool doZDCOr=false, float ZDC_1nthreshold=2000) {
    /* read in all files in the input folder */
    vector<string> files;
    GetFiles(input, files);

    /* read in offline ZDC information */
   /*
    TChain offChainZDC("zdcanalyzer/zdcrechit");
    FillChain(offChainZDC, files);
    TTreeReader offReaderZDC(&offChainZDC);
    TTreeReaderValue<float> sumPlus(offReaderZDC, "sumPlus");
    TTreeReaderValue<float> sumMinus(offReaderZDC, "sumMinus");
    */
    /* read in calo jet information */
    TChain offChain("akCs4PFJetAnalyzer/t");
    FillChain(offChain, files);
    TTreeReader offReader(&offChain);
    TTreeReaderValue<int>   jetN(offReader, "ncalo");
    TTreeReaderArray<float> jetPt(offReader, "calopt");
    TTreeReaderArray<float> jetEta(offReader, "caloeta");
    TTreeReaderArray<float> jetPhi(offReader, "calophi");

    /* read in emulated jet information */
    TChain emuChain("l1UpgradeEmuTree/L1UpgradeTree");
    FillChain(emuChain, files);
    TTreeReader emuReader(&emuChain);
    TTreeReaderValue<vector<float>> emuJetPt(emuReader, "jetEt");
    TTreeReaderValue<vector<float>> emuJetEta(emuReader, "jetEta");
    TTreeReaderValue<vector<float>> emuJetPhi(emuReader, "jetPhi");

    string seed = "L1_SingleJet_UPCprototype";
    float threshold = 8;

    /* create histograms for efficiency plots */
    int nbins = 40;
    float min = 0;
    float max = 40;
    TH1F emuHist("emuHist", "", nbins, min, max);
    TH1F emuMatchedHist("emuMatchedHist", "", nbins, min, max);
    TH1F recoHist("recoHist", "", nbins, min, max);

    Long64_t totalEvents = emuReader.GetEntries(true);
    int counter_ZDCOrselected = 0;
    int counter_ZDCOrJetselected = 0;
    /* read in information from TTrees */
    int totnjets = 0;
    for (Long64_t i = 0; i < totalEvents; i++) {
        emuReader.Next(); offReader.Next(); //offReaderZDC.Next();
        if (i % 100 == 0) {
            cout << "Entry: " << i << " / " <<  totalEvents << endl;
        }
	//FIXME: this is a very preliminary attempt to apply an offline-like selection on the ZDC
	//signal. Later on we will use it to mimic the effect of a ZDCOr L1 selection
	//bool isofflinesignal=((*sumPlus)>ZDC_1nthreshold&&(*sumMinus)<ZDC_1nthreshold)||((*sumPlus)<ZDC_1nthreshold&&(*sumMinus)>ZDC_1nthreshold);
        //if (doZDCOr && !isofflinesignal) continue;
        //counter_ZDCOrselected++;

	float maxJetPt = -999;
        float maxJetPhi = -999;
        float maxJetEta = -999;

        float emuMaxJetPt = -999;
        float emuMatchedJetPt = -999;
        float minDR = 10;
        /* iterate through jets and find the jet with max pT */
	//std::cout<<"N jets= "<<*jetN<<std::endl;
	totnjets = totnjets + *jetN;
	for (int i = 0; i < *jetN; ++i) {
            if (TMath::Abs(jetEta[i]) > 9) { continue; }

            if (jetPt[i] > maxJetPt) {
                maxJetPt = jetPt[i];
                maxJetEta = jetEta[i];
                maxJetPhi = jetPhi[i];
            }
        }

        if (maxJetPt > 0) {
            recoHist.Fill(maxJetPt);

            /* iterate through emu jets and find matched and unmatched jets with max pT */
            for (size_t i = 0; i < (*emuJetPt).size(); ++i) {
                if ((*emuJetPt)[i] > emuMaxJetPt) {
                    emuMaxJetPt = (*emuJetPt)[i];
                }

                if (dr((*emuJetEta)[i], (*emuJetPhi)[i], maxJetEta, maxJetPhi) < minDR) {
                    minDR = dr((*emuJetEta)[i], (*emuJetPhi)[i], maxJetEta, maxJetPhi);
                    emuMatchedJetPt = (*emuJetPt)[i];
                }
            }

            if (emuMaxJetPt >= threshold) {
		counter_ZDCOrJetselected++;
                emuHist.Fill(maxJetPt);
            }

            if (emuMatchedJetPt >= threshold && minDR < 0.4) {
                emuMatchedHist.Fill(maxJetPt);
            }
        }
    }

    TGraphAsymmErrors emuRecoEff(&emuHist, &recoHist, "cl=0.683 b(1,1) mode");
    TGraphAsymmErrors emuRecoMatchedEff(&emuMatchedHist, &recoHist, "cl=0.683 b(1,1) mode");

    /* plot the turn ons vs reco jet pt */
    TCanvas recoCanvas("recoCanvas", "", 0, 0, 500, 500);
    recoCanvas.cd();

    emuRecoMatchedEff.GetXaxis()->SetTitle("Reco Jet pT (GeV)");
    emuRecoMatchedEff.GetXaxis()->CenterTitle(true);
    emuRecoMatchedEff.GetYaxis()->SetTitle("Efficiency");
    emuRecoMatchedEff.GetYaxis()->CenterTitle(true);

    emuRecoMatchedEff.SetMarkerColor(46);
    emuRecoMatchedEff.SetLineColor(46);
    emuRecoMatchedEff.SetMarkerSize(0.5);
    emuRecoMatchedEff.SetMarkerStyle(20);
    emuRecoMatchedEff.Draw();

    emuRecoEff.SetMarkerColor(30);
    emuRecoEff.SetLineColor(30);
    emuRecoEff.SetMarkerSize(0.5);
    emuRecoEff.SetMarkerStyle(20);
    emuRecoEff.Draw("LP SAME");

    TLegend recoLegend(0.53, 0.12 ,0.88, 0.3);
    recoLegend.SetTextSize(0.03);
    recoLegend.SetHeader(seed.c_str());
    recoLegend.AddEntry(&emuRecoEff, "Not #DeltaR Matched", "lep");
    recoLegend.AddEntry(&emuRecoMatchedEff, "#DeltaR Matched", "lep");
    recoLegend.Draw();

    recoCanvas.SaveAs("CaloJetEfficiency.pdf");

    /* save histograms to file so I can look at them */
    TFile* fout = new TFile("histograms.root", "recreate");

    emuHist.Write();
    emuMatchedHist.Write();
    recoHist.Write();

    fout->Close();
    std::cout<<"N total jets= "<<totnjets<<std::endl;
    std::cout<<"ZDCOrselected fraction ="<<(float)counter_ZDCOrselected/(float)totalEvents<<std::endl;
    std::cout<<"ZDCOrJetselected, threshold="<<threshold<<" is ="<<(float)counter_ZDCOrJetselected/(float)totalEvents<<std::endl;
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc == 2)
        return UPCdijets(argv[1]);
    else {
        cout << "ERROR" << endl;
        return -1;
    }
}
