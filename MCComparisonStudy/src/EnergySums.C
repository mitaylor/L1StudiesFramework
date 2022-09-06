#include "TFile.h"
#include "TDirectory.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TChain.h"

#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "TMath.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TProfile2D.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

void FormatHistogram(TH1F* hist, int color) {
    hist->SetMarkerColor(color);
    hist->SetLineColor(color);
    hist->SetMarkerSize(0.5);
    hist->SetMarkerStyle(20);
    hist->SetStats(0);
    hist->GetXaxis()->CenterTitle(true);
    hist->GetYaxis()->SetTitle("Normalized Counts");
    hist->GetYaxis()->CenterTitle(true);
}

void FormatHistogramProf2D(TProfile2D* hist, double max) {
    hist->SetStats(0);
    hist->GetXaxis()->SetTitle("Eta");
    hist->GetXaxis()->CenterTitle(true);
    hist->GetYaxis()->SetTitle("Phi");
    hist->GetYaxis()->CenterTitle(true);
    hist->SetMinimum(0);
    hist->SetMaximum(max);
}

void PrintHist(TH1* hist1, TH1* hist2, string title, TCanvas* canvas, TLegend* legend, string filename) {
    hist1->GetXaxis()->SetTitle(title.c_str());
    hist1->Draw("HIST LP");
    hist2->Draw("HIST LP SAME");
    legend->Draw();

    TLatex* newMean = new TLatex();
    string newMeanText;
    newMean->SetTextFont(43);
    newMean->SetTextSize(12);
    newMean->SetTextColor(30);

    TLatex* oldMean = new TLatex();
    string oldMeanText;
    oldMean->SetTextFont(43);
    oldMean->SetTextSize(12);
    oldMean->SetTextColor(46);

    oldMeanText = "2018 Mean: " + to_string(hist2->GetMean());
    oldMean->DrawLatexNDC(0.6, 0.64, oldMeanText.c_str());
    newMeanText = "2022 Mean: " + to_string(hist1->GetMean());
    newMean->DrawLatexNDC(0.6, 0.60, newMeanText.c_str());
    canvas->Print(filename.c_str());
}

void PrintHistProf2D(TProfile2D* hist1, TProfile2D* hist2, TCanvas* canvas, string filename) {
    hist1->Draw("COLZ");
    canvas->Print(filename.c_str());
    hist2->Draw("COLZ");
    canvas->Print(filename.c_str());
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

int Compare(char const* oldInput, char const* newInput) {
    /* create map of bit number to energy sum */
    map<int, string> EnergySum = {
        {0, "etSumTotalEt"},
        {1, "etSumTotalEtHF"},
        {2, "etSumTotalEtEm"},
        {3, "etSumMinBiasHFP0"},
        {4, "htSumht"},
        {5, "htSumhtHF"},
        {6, "etSumMinBiasHFM0"},
        {7, "etSumMissingEt"},
        {8, "etSumMinBiasHFP1"},
        {9, "htSumMissingHt"},
        {10, "etSumMinBiasHFM1"},
        {11, "etSumMissingEtHF"},
        {12, "htSumMissingHtHF"},
        {13, "etSumTowCount"},
        {14, "etAsym"},
        {15, "etHFAsym"},
        {16, "htAsym"},
        {17, "htHFAsym"},
        {18, "centrality"}
    };

    /* read in 2018 information */
    vector<string> oldFiles;
    GetFiles(oldInput, oldFiles);

    TChain oldEnergySumChain("l1UpgradeEmuTree/L1UpgradeTree");
    TChain oldCaloTowerChain("l1CaloTowerEmuTree/L1CaloTowerTree");
    FillChain(oldEnergySumChain, oldFiles);
    FillChain(oldCaloTowerChain, oldFiles);
    TTreeReader oldCaloTowerReader(&oldCaloTowerChain);
    TTreeReaderValue<short> oldCaloNTowers(oldCaloTowerReader, "nTower");
    TTreeReaderValue<vector<short>> oldCaloIEt(oldCaloTowerReader, "iet");
    TTreeReaderValue<vector<short>> oldCaloIEm(oldCaloTowerReader, "iem");
    TTreeReaderValue<vector<short>> oldCaloIHad(oldCaloTowerReader, "ihad");
    TTreeReaderValue<vector<short>> oldCaloIEta(oldCaloTowerReader, "ieta");
    TTreeReaderValue<vector<short>> oldCaloIPhi(oldCaloTowerReader, "iphi");

    /* read in 2022 information */
    vector<string> newFiles;
    GetFiles(newInput, newFiles);

    TChain newEnergySumChain("l1UpgradeEmuTree/L1UpgradeTree");
    TChain newCaloTowerChain("l1CaloTowerEmuTree/L1CaloTowerTree");
    FillChain(newEnergySumChain, newFiles);
    FillChain(newCaloTowerChain, newFiles);
    TTreeReader newCaloTowerReader(&newCaloTowerChain);
    TTreeReaderValue<short> newCaloNTowers(newCaloTowerReader, "nTower");
    TTreeReaderValue<vector<short>> newCaloIEt(newCaloTowerReader, "iet");
    TTreeReaderValue<vector<short>> newCaloIEm(newCaloTowerReader, "iem");
    TTreeReaderValue<vector<short>> newCaloIHad(newCaloTowerReader, "ihad");
    TTreeReaderValue<vector<short>> newCaloIEta(newCaloTowerReader, "ieta");
    TTreeReaderValue<vector<short>> newCaloIPhi(newCaloTowerReader, "iphi");

    /* create histograms for energy sum plots */
    int nbins = 40;
    float min = 0;
    float max = 3000;

    size_t size = EnergySum.size();

    auto oldEnergySumHist = new TH1F("oldEnergySumHist", "", nbins, min, max);
    auto newEnergySumHist = new TH1F("newEnergySumHist", "", nbins, min, max);

    int oldEntries = oldEnergySumChain.GetEntries();
    int newEntries = newEnergySumChain.GetEntries();

    /* customize energy sum histogram draw options */
    auto legend = new TLegend(0.55, 0.75 ,0.85, 0.85);
    legend->SetTextSize(0.03);
    legend->AddEntry(oldEnergySumHist, "2018 MB MC", "p");
    legend->AddEntry(newEnergySumHist, "2022 MB MC", "p");

    FormatHistogram(newEnergySumHist, 30);
    FormatHistogram(oldEnergySumHist, 46);

    /* plot the energy sum distributions */
    auto canvas = new TCanvas("canvas", "", 0 , 0, 500, 500);
    canvas->SetLeftMargin(0.15);
    canvas->SetBottomMargin(0.15);
    canvas->Print("L1EnergySums.pdf[");

    for (size_t i = 0; i < size; ++i) {
        canvas->Clear();

        string oldDrawCommand = "sumEt[" + to_string(i) + "] >> oldEnergySumHist";
        string newDrawCommand = "sumEt[" + to_string(i) + "] >> newEnergySumHist";

        oldEnergySumChain.Draw(oldDrawCommand.c_str(), "", "goff");
        newEnergySumChain.Draw(newDrawCommand.c_str(), "", "goff");

        oldEnergySumHist->Scale(1.0/oldEntries);
        newEnergySumHist->Scale(1.0/newEntries);

        PrintHist(newEnergySumHist, oldEnergySumHist, EnergySum[i], canvas, legend, "L1EnergySums.pdf");
    }

    canvas->Print("L1EnergySums.pdf]");

    /* create histograms for caloTower plots */
    auto oldCaloNTowersHist = new TH1F("oldCaloNTowersHist", "", nbins*2, 0, 5500);
    auto oldCaloIEtHist = new TH1F("oldCaloIEtHist", "", nbins*2, 0, 30000);
    auto oldCaloIEmHist = new TH1F("oldCaloIEmHist", "", nbins*2, 0, 9000);
    auto oldCaloIHadHist = new TH1F("oldCaloIHadHist", "", nbins*2, 0, 9000);
    auto oldCaloIHFHist = new TH1F("oldCaloIHFHist", "", nbins*2, 0, 13000);

    auto newCaloNTowersHist = new TH1F("newCaloNTowersHist", "", nbins*2, 0, 5500);
    auto newCaloIEtHist = new TH1F("newCaloIEtHist", "", nbins*2, 0, 30000);
    auto newCaloIEmHist = new TH1F("newCaloIEmHist", "", nbins*2, 0, 9000);
    auto newCaloIHadHist = new TH1F("newCaloIHadHist", "", nbins*2, 0, 9000);
    auto newCaloIHFHist = new TH1F("newCaloIHFHist", "", nbins*2, 0, 13000);

    auto oldCaloNTowersHistZoom = new TH1F("oldCaloNTowersHistZoom", "", nbins, 0, 550);
    auto oldCaloIEtHistZoom = new TH1F("oldCaloIEtHistZoom", "", nbins, 0, 3000);
    auto oldCaloIEmHistZoom = new TH1F("oldCaloIEmHistZoom", "", nbins, 0, 900);
    auto oldCaloIHadHistZoom = new TH1F("oldCaloIHadHistZoom", "", nbins, 0, 900);
    auto oldCaloIHFHistZoom = new TH1F("oldCaloIHFHistZoom", "", nbins, 0, 1300);

    auto newCaloNTowersHistZoom = new TH1F("newCaloNTowersHistZoom", "", nbins, 0, 550);
    auto newCaloIEtHistZoom = new TH1F("newCaloIEtHistZoom", "", nbins, 0, 3000);
    auto newCaloIEmHistZoom = new TH1F("newCaloIEmHistZoom", "", nbins, 0, 900);
    auto newCaloIHadHistZoom = new TH1F("newCaloIHadHistZoom", "", nbins, 0, 900);
    auto newCaloIHFHistZoom = new TH1F("newCaloIHFHistZoom", "", nbins, 0, 1300);

    auto oldCaloIEtEtaPhiHist = new TProfile2D("oldCaloIEtEtaPhiHist", "2018 Average Et", 84, -42, 42, 73, 0, 73);
    auto oldCaloIEmEtaPhiHist = new TProfile2D("oldCaloIEmEtaPhiHist", "2018 Average Em", 84, -42, 42, 73, 0, 73);
    auto oldCaloIHadEtaPhiHist = new TProfile2D("oldCaloIHadEtaPhiHist", "2018 Average Had", 84, -42, 42, 73, 0, 73);
    auto oldCaloIHFEtaPhiHist = new TProfile2D("oldCaloIHFEtaPhiHist", "2018 Average HF", 84, -42, 42, 73, 0, 73, 0);

    auto newCaloIEtEtaPhiHist = new TProfile2D("newCaloIEtEtaPhiHist", "2022 Average Et", 84, -42, 42, 73, 0, 73);
    auto newCaloIEmEtaPhiHist = new TProfile2D("newCaloIEmEtaPhiHist", "2022 Average Em", 84, -42, 42, 73, 0, 73);
    auto newCaloIHadEtaPhiHist = new TProfile2D("newCaloIHadEtaPhiHist", "2022 Average Had", 84, -42, 42, 73, 0, 73);
    auto newCaloIHFEtaPhiHist = new TProfile2D("newCaloIHFEtaPhiHist", "2022 Average HF", 84, -42, 42, 73, 0, 73);

    /* customize calo tower histogram draw options */
    FormatHistogram(oldCaloNTowersHist, 46);
    FormatHistogram(oldCaloIEtHist, 46);
    FormatHistogram(oldCaloIEmHist, 46);
    FormatHistogram(oldCaloIHadHist, 46);
    FormatHistogram(oldCaloIHFHist, 46);

    FormatHistogram(newCaloNTowersHist, 30);
    FormatHistogram(newCaloIEtHist, 30);
    FormatHistogram(newCaloIEmHist, 30);
    FormatHistogram(newCaloIHadHist, 30);
    FormatHistogram(newCaloIHFHist, 30);

    FormatHistogram(oldCaloNTowersHistZoom, 46);
    FormatHistogram(oldCaloIEtHistZoom, 46);
    FormatHistogram(oldCaloIEmHistZoom, 46);
    FormatHistogram(oldCaloIHadHistZoom, 46);
    FormatHistogram(oldCaloIHFHistZoom, 46);

    FormatHistogram(newCaloNTowersHistZoom, 30);
    FormatHistogram(newCaloIEtHistZoom, 30);
    FormatHistogram(newCaloIEmHistZoom, 30);
    FormatHistogram(newCaloIHadHistZoom, 30);
    FormatHistogram(newCaloIHFHistZoom, 30);

    FormatHistogramProf2D(oldCaloIEtEtaPhiHist, 8);
    FormatHistogramProf2D(oldCaloIEmEtaPhiHist, 3.5);
    FormatHistogramProf2D(oldCaloIHadEtaPhiHist, 4);
    FormatHistogramProf2D(oldCaloIHFEtaPhiHist, 4);

    FormatHistogramProf2D(newCaloIEtEtaPhiHist, 8);
    FormatHistogramProf2D(newCaloIEmEtaPhiHist, 3.5);
    FormatHistogramProf2D(newCaloIHadEtaPhiHist, 4);
    FormatHistogramProf2D(newCaloIHFEtaPhiHist, 4);

    /* read in information from TTrees */
    for (int i = 1; i < oldEntries; ++i) {
        oldCaloTowerReader.Next();
        if (i % (oldEntries / 20) == 0) cout << i << " / " << oldEntries << endl;

        int et = 0;
        int em = 0;
        int had = 0;

        for (int j = 0; j < (*oldCaloNTowers); ++j) {
            // if (TMath::Abs((*oldCaloIEta)[j]) < 30) {
                et += (*oldCaloIEt)[j];
                em += (*oldCaloIEm)[j];
                had += (*oldCaloIHad)[j];
            // }

            oldCaloIEtEtaPhiHist->Fill((*oldCaloIEta)[j], (*oldCaloIPhi)[j], (*oldCaloIEt)[j]);
            oldCaloIEmEtaPhiHist->Fill((*oldCaloIEta)[j], (*oldCaloIPhi)[j], (*oldCaloIEm)[j]);
            oldCaloIHadEtaPhiHist->Fill((*oldCaloIEta)[j], (*oldCaloIPhi)[j], (*oldCaloIHad)[j]);
            oldCaloIHFEtaPhiHist->Fill((*oldCaloIEta)[j], (*oldCaloIPhi)[j], (*oldCaloIEt)[j]-(*oldCaloIEm)[j]-(*oldCaloIHad)[j]);
        }


        oldCaloNTowersHist->Fill(*oldCaloNTowers);
        oldCaloIEtHist->Fill(et);
        oldCaloIEmHist->Fill(em);
        oldCaloIHadHist->Fill(had);
        oldCaloIHFHist->Fill(et-em-had);

        oldCaloNTowersHistZoom->Fill(*oldCaloNTowers);
        oldCaloIEtHistZoom->Fill(et);
        oldCaloIEmHistZoom->Fill(em);
        oldCaloIHadHistZoom->Fill(had);
        oldCaloIHFHistZoom->Fill(et-em-had);
    }

    for (int i = 1; i < newEntries; ++i) {
        newCaloTowerReader.Next();
        if (i % (newEntries / 20) == 0) cout << i << " / " << newEntries << endl;

        int et = 0;
        int em = 0;
        int had = 0;

        for (int j = 0; j < (*newCaloNTowers); ++j) {
            // if (TMath::Abs((*newCaloIEta)[j]) < 30) {
                et += (*newCaloIEt)[j];
                em += (*newCaloIEm)[j];
                had += (*newCaloIHad)[j];
            // }

            newCaloIEtEtaPhiHist->Fill((*newCaloIEta)[j], (*newCaloIPhi)[j], (*newCaloIEt)[j]);
            newCaloIEmEtaPhiHist->Fill((*newCaloIEta)[j], (*newCaloIPhi)[j], (*newCaloIEm)[j]);
            newCaloIHadEtaPhiHist->Fill((*newCaloIEta)[j], (*newCaloIPhi)[j], (*newCaloIHad)[j]);
            newCaloIHFEtaPhiHist->Fill((*newCaloIEta)[j], (*newCaloIPhi)[j], (*newCaloIEt)[j]-(*newCaloIEm)[j]-(*newCaloIHad)[j]);
        }

        newCaloNTowersHist->Fill(*newCaloNTowers);
        newCaloIEtHist->Fill(et);
        newCaloIEmHist->Fill(em);
        newCaloIHadHist->Fill(had);
        newCaloIHFHist->Fill(et-em-had);

        newCaloNTowersHistZoom->Fill(*newCaloNTowers);
        newCaloIEtHistZoom->Fill(et);
        newCaloIEmHistZoom->Fill(em);
        newCaloIHadHistZoom->Fill(had);
        newCaloIHFHistZoom->Fill(et-em-had);
    }

    /* scale the histograms */
    oldCaloNTowersHist->Scale(1.0/oldEntries);
    oldCaloIEtHist->Scale(1.0/oldEntries);
    oldCaloIEmHist->Scale(1.0/oldEntries);
    oldCaloIHadHist->Scale(1.0/oldEntries);
    oldCaloIHFHist->Scale(1.0/oldEntries);

    oldCaloNTowersHistZoom->Scale(1.0/oldEntries);
    oldCaloIEtHistZoom->Scale(1.0/oldEntries);
    oldCaloIEmHistZoom->Scale(1.0/oldEntries);
    oldCaloIHadHistZoom->Scale(1.0/oldEntries);
    oldCaloIHFHistZoom->Scale(1.0/oldEntries);

    newCaloNTowersHist->Scale(1.0/newEntries);
    newCaloIEtHist->Scale(1.0/newEntries);
    newCaloIEmHist->Scale(1.0/newEntries);
    newCaloIHadHist->Scale(1.0/newEntries);
    newCaloIHFHist->Scale(1.0/newEntries);

    newCaloNTowersHistZoom->Scale(1.0/newEntries);
    newCaloIEtHistZoom->Scale(1.0/newEntries);
    newCaloIEmHistZoom->Scale(1.0/newEntries);
    newCaloIHadHistZoom->Scale(1.0/newEntries);
    newCaloIHFHistZoom->Scale(1.0/newEntries);

    /* plot the caloTower distributions */
    canvas->Print("L1CaloTowers.pdf[");
    canvas->SetLogy(1);
    canvas->Clear();

    PrintHist(newCaloNTowersHist, oldCaloNTowersHist, "nTowers", canvas, legend, "L1CaloTowers.pdf");
    PrintHist(newCaloIEtHist, oldCaloIEtHist, "Et Sum", canvas, legend, "L1CaloTowers.pdf");
    PrintHist(newCaloIEmHist, oldCaloIEmHist, "EM Sum", canvas, legend, "L1CaloTowers.pdf");
    PrintHist(newCaloIHadHist, oldCaloIHadHist, "Had Sum", canvas, legend, "L1CaloTowers.pdf");
    PrintHist(newCaloIHFHist, oldCaloIHFHist, "HF Sum", canvas, legend, "L1CaloTowers.pdf");

    PrintHist(newCaloNTowersHistZoom, oldCaloNTowersHistZoom, "nTowers", canvas, legend, "L1CaloTowers.pdf");
    PrintHist(newCaloIEtHistZoom, oldCaloIEtHistZoom, "Et Sum", canvas, legend, "L1CaloTowers.pdf");
    PrintHist(newCaloIEmHistZoom, oldCaloIEmHistZoom, "EM Sum", canvas, legend, "L1CaloTowers.pdf");
    PrintHist(newCaloIHadHistZoom, oldCaloIHadHistZoom, "Had Sum", canvas, legend, "L1CaloTowers.pdf");
    PrintHist(newCaloIHFHistZoom, oldCaloIHFHistZoom, "HF Sum", canvas, legend, "L1CaloTowers.pdf");

    canvas->Print("L1CaloTowers.pdf]");

    canvas->Print("L1CaloTowersEtaPhi.pdf[");
    canvas->SetLogy(0);
    canvas->Clear();

    PrintHistProf2D(newCaloIEtEtaPhiHist, oldCaloIEtEtaPhiHist, canvas, "L1CaloTowersEtaPhi.pdf");
    PrintHistProf2D(newCaloIEmEtaPhiHist, oldCaloIEmEtaPhiHist, canvas, "L1CaloTowersEtaPhi.pdf");
    PrintHistProf2D(newCaloIHadEtaPhiHist, oldCaloIHadEtaPhiHist, canvas, "L1CaloTowersEtaPhi.pdf");
    PrintHistProf2D(newCaloIHFEtaPhiHist, oldCaloIHFEtaPhiHist, canvas, "L1CaloTowersEtaPhi.pdf");

    canvas->Print("L1CaloTowersEtaPhi.pdf]");

   
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 3)
        return Compare(argv[1], argv[2]);

    else {
        cout << "ERROR: Please pass two paths for 2018 MC and 2022 MC." << endl;
        return -1;
    }
}
