#include "TFile.h"
#include "TDirectory.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TChain.h"

#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

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
    /* read in 2018 information */
    vector<string> oldFiles;
    GetFiles(oldInput, oldFiles);

    TChain oldCaloTowerChain("rechitanalyzerpp/tower");
    FillChain(oldCaloTowerChain, oldFiles);
    TTreeReader oldCaloTowerReader(&oldCaloTowerChain);
    TTreeReaderValue<int> oldCaloNTowers(oldCaloTowerReader, "n");
    TTreeReaderArray<float> oldCaloIEt(oldCaloTowerReader, "et");
    TTreeReaderArray<float> oldCaloIEm(oldCaloTowerReader, "emEt");
    TTreeReaderArray<float> oldCaloIHad(oldCaloTowerReader, "hadEt");
    TTreeReaderArray<int> oldCaloIEta(oldCaloTowerReader, "ieta");
    TTreeReaderArray<int> oldCaloIPhi(oldCaloTowerReader, "iphi");

    /* read in 2022 information */
    vector<string> newFiles;
    GetFiles(newInput, newFiles);

    TChain newCaloTowerChain("rechitanalyzer/tower");
    FillChain(newCaloTowerChain, newFiles);
    TTreeReader newCaloTowerReader(&newCaloTowerChain);
    TTreeReaderValue<int> newCaloNTowers(newCaloTowerReader, "n");
    TTreeReaderArray<float> newCaloIEt(newCaloTowerReader, "et");
    TTreeReaderArray<float> newCaloIEm(newCaloTowerReader, "emEt");
    TTreeReaderArray<float> newCaloIHad(newCaloTowerReader, "hadEt");
    TTreeReaderArray<int> newCaloIEta(newCaloTowerReader, "ieta");
    TTreeReaderArray<int> newCaloIPhi(newCaloTowerReader, "iphi");

    /* create histograms for energy sum plots */
    int nbins = 40;

    int oldEntries = oldCaloTowerChain.GetEntries();
    int newEntries = newCaloTowerChain.GetEntries();

    /* create histograms for caloTower plots */
    auto oldCaloNTowersHist = new TH1F("oldCaloNTowersHist", "", nbins*2, 0, 4500);
    auto oldCaloIEtHist = new TH1F("oldCaloIEtHist", "", nbins*2, 0, 15000);
    auto oldCaloIEmHist = new TH1F("oldCaloIEmHist", "", nbins*2, 0, 6500);
    auto oldCaloIHBHist = new TH1F("oldCaloIHBHist", "", nbins*2, 0, 1100);
    auto oldCaloIHEHist = new TH1F("oldCaloIHEHist", "", nbins*2, 0, 4000);
    auto oldCaloIHFHist = new TH1F("oldCaloIHFHist", "", nbins*2, 0, 5000);

    auto newCaloNTowersHist = new TH1F("newCaloNTowersHist", "", nbins*2, 0, 4500);
    auto newCaloIEtHist = new TH1F("newCaloIEtHist", "", nbins*2, 0, 15000);
    auto newCaloIEmHist = new TH1F("newCaloIEmHist", "", nbins*2, 0, 6500);
    auto newCaloIHBHist = new TH1F("newCaloIHBHist", "", nbins*2, 0, 1100);
    auto newCaloIHEHist = new TH1F("newCaloIHEHist", "", nbins*2, 0, 4000);
    auto newCaloIHFHist = new TH1F("newCaloIHFHist", "", nbins*2, 0, 5000);

    auto oldCaloNTowersHistZoom = new TH1F("oldCaloNTowersHistZoom", "", nbins, 0, 800);
    auto oldCaloIEtHistZoom = new TH1F("oldCaloIEtHistZoom", "", nbins, 0, 1500);
    auto oldCaloIEmHistZoom = new TH1F("oldCaloIEmHistZoom", "", nbins, 0, 600);
    auto oldCaloIHBHistZoom = new TH1F("oldCaloIHBHistZoom", "", nbins, 0, 200);
    auto oldCaloIHEHistZoom = new TH1F("oldCaloIHEHistZoom", "", nbins, 0, 400);
    auto oldCaloIHFHistZoom = new TH1F("oldCaloIHFHistZoom", "", nbins, 0, 500);

    auto newCaloNTowersHistZoom = new TH1F("newCaloNTowersHistZoom", "", nbins, 0, 800);
    auto newCaloIEtHistZoom = new TH1F("newCaloIEtHistZoom", "", nbins, 0, 1500);
    auto newCaloIEmHistZoom = new TH1F("newCaloIEmHistZoom", "", nbins, 0, 600);
    auto newCaloIHBHistZoom = new TH1F("newCaloIHBHistZoom", "", nbins, 0, 200);
    auto newCaloIHEHistZoom = new TH1F("newCaloIHEHistZoom", "", nbins, 0, 400);
    auto newCaloIHFHistZoom = new TH1F("newCaloIHFHistZoom", "", nbins, 0, 500);

    auto oldCaloIEtEtaPhiHist = new TProfile2D("oldCaloIEtEtaPhiHist", "2018 Average Et", 84, -42, 42, 36, 0, 73);
    auto oldCaloIEmEtaPhiHist = new TProfile2D("oldCaloIEmEtaPhiHist", "2018 Average Em", 84, -42, 42, 36, 0, 73);
    auto oldCaloIHadEtaPhiHist = new TProfile2D("oldCaloIHadEtaPhiHist", "2018 Average Had", 84, -42, 42, 36, 0, 73);

    auto newCaloIEtEtaPhiHist = new TProfile2D("newCaloIEtEtaPhiHist", "2022 Average Et", 84, -42, 42, 36, 0, 73);
    auto newCaloIEmEtaPhiHist = new TProfile2D("newCaloIEmEtaPhiHist", "2022 Average Em", 84, -42, 42, 36, 0, 73);
    auto newCaloIHadEtaPhiHist = new TProfile2D("newCaloIHadEtaPhiHist", "2022 Average Had", 84, -42, 42, 36, 0, 73);

    /* customize calo tower histogram draw options */
    auto legend = new TLegend(0.55, 0.75 ,0.85, 0.85);
    legend->SetTextSize(0.03);
    legend->AddEntry(oldCaloNTowersHist, "2018 MB MC", "p");
    legend->AddEntry(newCaloNTowersHist, "2022 MB MC", "p");

    FormatHistogram(oldCaloNTowersHist, 46);
    FormatHistogram(oldCaloIEtHist, 46);
    FormatHistogram(oldCaloIEmHist, 46);
    FormatHistogram(oldCaloIHBHist, 46);
    FormatHistogram(oldCaloIHEHist, 46);
    FormatHistogram(oldCaloIHFHist, 46);

    FormatHistogram(newCaloNTowersHist, 30);
    FormatHistogram(newCaloIEtHist, 30);
    FormatHistogram(newCaloIEmHist, 30);
    FormatHistogram(newCaloIHBHist, 30);
    FormatHistogram(newCaloIHEHist, 30);
    FormatHistogram(newCaloIHFHist, 30);

    FormatHistogram(oldCaloNTowersHistZoom, 46);
    FormatHistogram(oldCaloIEtHistZoom, 46);
    FormatHistogram(oldCaloIEmHistZoom, 46);
    FormatHistogram(oldCaloIHBHistZoom, 46);
    FormatHistogram(oldCaloIHEHistZoom, 46);
    FormatHistogram(oldCaloIHFHistZoom, 46);

    FormatHistogram(newCaloNTowersHistZoom, 30);
    FormatHistogram(newCaloIEtHistZoom, 30);
    FormatHistogram(newCaloIEmHistZoom, 30);
    FormatHistogram(newCaloIHBHistZoom, 30);
    FormatHistogram(newCaloIHEHistZoom, 30);
    FormatHistogram(newCaloIHFHistZoom, 30);

    FormatHistogramProf2D(oldCaloIEtEtaPhiHist, 4);
    FormatHistogramProf2D(oldCaloIEmEtaPhiHist, 2.5);
    FormatHistogramProf2D(oldCaloIHadEtaPhiHist, 2.5);

    FormatHistogramProf2D(newCaloIEtEtaPhiHist, 4);
    FormatHistogramProf2D(newCaloIEmEtaPhiHist, 2.5);
    FormatHistogramProf2D(newCaloIHadEtaPhiHist, 2.5);

    /* read in information from TTrees */
    for (int i = 1; i < oldEntries; ++i) {
        oldCaloTowerReader.Next();
        if (i % (oldEntries / 20) == 0) cout << i << " / " << oldEntries << endl;

        double et = 0;
        double em = 0;
        double hb = 0;
        double he = 0;

        for (int j = 0; j < (*oldCaloNTowers); ++j) {
            et += oldCaloIEt[j];
            em += oldCaloIEm[j];
            if (oldCaloIEta[j] <= 16 && oldCaloIEta[j] >= -16)
                hb += oldCaloIHad[j];
            if ( (oldCaloIEta[j] <= 29 && oldCaloIEta[j] >= 17) || (oldCaloIEta[j] >= -29 && oldCaloIEta[j] <= -17) )
                he += oldCaloIHad[j];

            oldCaloIEtEtaPhiHist->Fill(oldCaloIEta[j], oldCaloIPhi[j], oldCaloIEt[j]);
            oldCaloIEmEtaPhiHist->Fill(oldCaloIEta[j], oldCaloIPhi[j], oldCaloIEm[j]);
            oldCaloIHadEtaPhiHist->Fill(oldCaloIEta[j], oldCaloIPhi[j], oldCaloIHad[j]);
        }


        oldCaloNTowersHist->Fill(*oldCaloNTowers);
        oldCaloIEtHist->Fill(et);
        oldCaloIEmHist->Fill(em);
        oldCaloIHBHist->Fill(hb);
        oldCaloIHEHist->Fill(he);
        oldCaloIHFHist->Fill(et-em-hb-he);

        oldCaloNTowersHistZoom->Fill(*oldCaloNTowers);
        oldCaloIEtHistZoom->Fill(et);
        oldCaloIEmHistZoom->Fill(em);
        oldCaloIHBHistZoom->Fill(hb);
        oldCaloIHEHistZoom->Fill(he);
        oldCaloIHFHistZoom->Fill(et-em-hb-he);
    }

    for (int i = 1; i < newEntries; ++i) {
        newCaloTowerReader.Next();
        if (i % (newEntries / 20) == 0) cout << i << " / " << newEntries << endl;

        double et = 0;
        double em = 0;
        double hb = 0;
        double he = 0;

        for (int j = 0; j < (*newCaloNTowers); ++j) {
            et += newCaloIEt[j];
            em += newCaloIEm[j];
            if (newCaloIEta[j] <= 16 && newCaloIEta[j] >= -16)
                hb += newCaloIHad[j];
            if ( (newCaloIEta[j] <= 29 && newCaloIEta[j] >= 17) || (newCaloIEta[j] >= -29 && newCaloIEta[j] <= -17) )
                he += newCaloIHad[j];

            newCaloIEtEtaPhiHist->Fill(newCaloIEta[j], newCaloIPhi[j], newCaloIEt[j]);
            newCaloIEmEtaPhiHist->Fill(newCaloIEta[j], newCaloIPhi[j], newCaloIEm[j]);
            newCaloIHadEtaPhiHist->Fill(newCaloIEta[j], newCaloIPhi[j], newCaloIHad[j]);
        }


        newCaloNTowersHist->Fill(*newCaloNTowers);
        newCaloIEtHist->Fill(et);
        newCaloIEmHist->Fill(em);
        newCaloIHBHist->Fill(hb);
        newCaloIHEHist->Fill(he);
        newCaloIHFHist->Fill(et-em-hb-he);

        newCaloNTowersHistZoom->Fill(*newCaloNTowers);
        newCaloIEtHistZoom->Fill(et);
        newCaloIEmHistZoom->Fill(em);
        newCaloIHBHistZoom->Fill(hb);
        newCaloIHEHistZoom->Fill(he);
        newCaloIHFHistZoom->Fill(et-em-hb-he);
    }

    /* scale the histograms */
    oldCaloNTowersHist->Scale(1.0/oldEntries);
    oldCaloIEtHist->Scale(1.0/oldEntries);
    oldCaloIEmHist->Scale(1.0/oldEntries);
    oldCaloIHBHist->Scale(1.0/oldEntries);
    oldCaloIHEHist->Scale(1.0/oldEntries);
    oldCaloIHFHist->Scale(1.0/oldEntries);

    oldCaloNTowersHistZoom->Scale(1.0/oldEntries);
    oldCaloIEtHistZoom->Scale(1.0/oldEntries);
    oldCaloIEmHistZoom->Scale(1.0/oldEntries);
    oldCaloIHBHistZoom->Scale(1.0/oldEntries);
    oldCaloIHEHistZoom->Scale(1.0/oldEntries);
    oldCaloIHFHistZoom->Scale(1.0/oldEntries);

    newCaloNTowersHist->Scale(1.0/newEntries);
    newCaloIEtHist->Scale(1.0/newEntries);
    newCaloIEmHist->Scale(1.0/newEntries);
    newCaloIHBHist->Scale(1.0/newEntries);
    newCaloIHEHist->Scale(1.0/newEntries);
    newCaloIHFHist->Scale(1.0/newEntries);

    newCaloNTowersHistZoom->Scale(1.0/newEntries);
    newCaloIEtHistZoom->Scale(1.0/newEntries);
    newCaloIEmHistZoom->Scale(1.0/newEntries);
    newCaloIHBHistZoom->Scale(1.0/newEntries);
    newCaloIHEHistZoom->Scale(1.0/newEntries);
    newCaloIHFHistZoom->Scale(1.0/newEntries);

    /* plot the caloTower distributions */
    auto canvas = new TCanvas("canvas", "", 0 , 0, 500, 500);
    canvas->SetLeftMargin(0.15);
    canvas->SetBottomMargin(0.15);
    canvas->Print("RechitTowers.pdf[");
    canvas->SetLogy(1);
    canvas->Clear();

    PrintHist(newCaloNTowersHist, oldCaloNTowersHist, "nTowers", canvas, legend, "RechitTowers.pdf");
    PrintHist(newCaloIEtHist, oldCaloIEtHist, "Et Sum", canvas, legend, "RechitTowers.pdf");
    PrintHist(newCaloIEmHist, oldCaloIEmHist, "EM Sum", canvas, legend, "RechitTowers.pdf");
    PrintHist(newCaloIHBHist, oldCaloIHBHist, "HB Sum", canvas, legend, "RechitTowers.pdf");
    PrintHist(newCaloIHEHist, oldCaloIHEHist, "HE Sum", canvas, legend, "RechitTowers.pdf");
    PrintHist(newCaloIHFHist, oldCaloIHFHist, "HF Sum", canvas, legend, "RechitTowers.pdf");

    PrintHist(newCaloNTowersHistZoom, oldCaloNTowersHistZoom, "nTowers", canvas, legend, "RechitTowers.pdf");
    PrintHist(newCaloIEtHistZoom, oldCaloIEtHistZoom, "Et Sum", canvas, legend, "RechitTowers.pdf");
    PrintHist(newCaloIEmHistZoom, oldCaloIEmHistZoom, "EM Sum", canvas, legend, "RechitTowers.pdf");
    PrintHist(newCaloIHBHistZoom, oldCaloIHBHistZoom, "HB Sum", canvas, legend, "RechitTowers.pdf");
    PrintHist(newCaloIHEHistZoom, oldCaloIHEHistZoom, "HE Sum", canvas, legend, "RechitTowers.pdf");
    PrintHist(newCaloIHFHistZoom, oldCaloIHFHistZoom, "HF Sum", canvas, legend, "RechitTowers.pdf");

    canvas->Print("RechitTowers.pdf]");

    canvas->Print("RechitTowersEtaPhi.pdf[");
    canvas->SetLogy(0);
    canvas->Clear();

    PrintHistProf2D(newCaloIEtEtaPhiHist, oldCaloIEtEtaPhiHist, canvas, "RechitTowersEtaPhi.pdf");
    PrintHistProf2D(newCaloIEmEtaPhiHist, oldCaloIEmEtaPhiHist, canvas, "RechitTowersEtaPhi.pdf");
    PrintHistProf2D(newCaloIHadEtaPhiHist, oldCaloIHadEtaPhiHist, canvas, "RechitTowersEtaPhi.pdf");

    canvas->Print("RechitTowersEtaPhi.pdf]");

   
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
