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

    TChain oldEGChain("ggHiNtuplizerGED/EventTree");
    TChain oldEvtChain("hiEvtAnalyzer/HiTree");
    TChain oldGenChain("HiGenParticleAna/hi");
    TChain oldJetChain("akFlowPuCs4PFJetAnalyzer/t");

    FillChain(oldEGChain, oldFiles);
    FillChain(oldEvtChain, oldFiles);
    FillChain(oldGenChain, oldFiles);
    FillChain(oldJetChain, oldFiles);

    /* read in 2022 information */
    vector<string> newFiles;
    GetFiles(newInput, newFiles);

    TChain newEGChain("ggHiNtuplizer/EventTree");
    TChain newEvtChain("hiEvtAnalyzer/HiTree");
    TChain newGenChain("HiGenParticleAna/hi");
    TChain newJetChain("akCs4PFJetAnalyzer/t");

    FillChain(newEGChain, newFiles);
    FillChain(newEvtChain, newFiles);
    FillChain(newGenChain, newFiles);
    FillChain(newJetChain, newFiles);

    /* define plots */
    vector<string> EGPlots = { "nMC", "mcPt", "nEle", "elePt", "nPho", "phoEt" };
    vector<double> EGMax = { 350, 60, 3, 60, 12, 60 };
    vector<int> EGLog { 0, 1, 0, 0, 0, 0 };

    vector<string> EvtPlots = { "hiHF", "hiET", "hiEE", "hiEB", "hiNpix" };
    vector<double> EvtMax = { 7000, 1200, 3000, 2500, 100000 };
    vector<int> EvtLog { 1, 0, 0, 0, 0 };

    vector<string> GenPlots = { "mult", "pt" };
    vector<double> GenMax = { 22000, 60 };
    vector<int> GenLog { 0, 1 };

    vector<string> JetPlots = { "jtpt", "nref" };
    vector<double> JetMax = { 150, 80 };
    vector<int> JetLog { 1, 0 };

    /* create histograms for energy sum plots */
    int nbins = 100;

    TH1F* oldHist = new TH1F("oldTemp", "", nbins, 0, 1);
    TH1F* newHist = new TH1F("newTemp", "", nbins, 0, 1);

    FormatHistogram(newHist, 30);
    FormatHistogram(oldHist, 46);

    int oldEntries = oldEGChain.GetEntries();
    int newEntries = newEGChain.GetEntries();

    /* customize energy sum histogram draw options */
    auto legend = new TLegend(0.55, 0.75 ,0.85, 0.85);
    legend->SetTextSize(0.03);
    legend->AddEntry(oldHist, "2018 MB MC", "p");
    legend->AddEntry(newHist, "2022 MB MC", "p");

    /* plot the energy sum distributions */
    auto canvas = new TCanvas("canvas", "", 0 , 0, 500, 500);
    canvas->SetLeftMargin(0.15);
    canvas->SetBottomMargin(0.15);
    canvas->Print("OfflineComp.pdf[");

    /* EG Plots */
    for (size_t i = 0; i < EGPlots.size(); ++i) {
        canvas->Clear();
        canvas->SetLogy(EGLog[i]);

        nbins = EGMax[i] > 100 ? 100 : EGMax[i];

        oldHist = new TH1F("oldHist", "", nbins, 0, EGMax[i]);
        newHist = new TH1F("newHist", "", nbins, 0, EGMax[i]);

        FormatHistogram(newHist, 30);
        FormatHistogram(oldHist, 46);

        string oldDrawCommand = EGPlots[i] + " >> oldHist";
        string newDrawCommand = EGPlots[i] + " >> newHist";

        oldEGChain.Draw(oldDrawCommand.c_str(), "", "goff");
        newEGChain.Draw(newDrawCommand.c_str(), "", "goff");

        oldHist->Scale(1.0/oldEntries);
        newHist->Scale(1.0/newEntries);

        PrintHist(newHist, oldHist, EGPlots[i], canvas, legend, "OfflineComp.pdf");

        delete newHist; delete oldHist;
    }

    /* Evt Plots */
    for (size_t i = 0; i < EvtPlots.size(); ++i) {
        canvas->Clear();
        canvas->SetLogy(EvtLog[i]);

        nbins = EvtMax[i] > 100 ? 100 : EvtMax[i];

        oldHist = new TH1F("oldHist", "", nbins, 0, EvtMax[i]);
        newHist = new TH1F("newHist", "", nbins, 0, EvtMax[i]);

        FormatHistogram(newHist, 30);
        FormatHistogram(oldHist, 46);

        string oldDrawCommand = EvtPlots[i] + " >> oldHist";
        string newDrawCommand = EvtPlots[i] + " >> newHist";

        oldEvtChain.Draw(oldDrawCommand.c_str(), "", "goff");
        newEvtChain.Draw(newDrawCommand.c_str(), "", "goff");

        oldHist->Scale(1.0/oldEntries);
        newHist->Scale(1.0/newEntries);

        PrintHist(newHist, oldHist, EvtPlots[i], canvas, legend, "OfflineComp.pdf");

        delete newHist; delete oldHist;
    }

    /* Gen Plots */
    for (size_t i = 0; i < GenPlots.size(); ++i) {
        canvas->Clear();
        canvas->SetLogy(GenLog[i]);

        nbins = GenMax[i] > 100 ? 100 : GenMax[i];

        oldHist = new TH1F("oldHist", "", nbins, 0, GenMax[i]);
        newHist = new TH1F("newHist", "", nbins, 0, GenMax[i]);

        FormatHistogram(newHist, 30);
        FormatHistogram(oldHist, 46);

        string oldDrawCommand = GenPlots[i] + " >> oldHist";
        string newDrawCommand = GenPlots[i] + " >> newHist";

        oldGenChain.Draw(oldDrawCommand.c_str(), "", "goff");
        newGenChain.Draw(newDrawCommand.c_str(), "", "goff");

        oldHist->Scale(1.0/oldEntries);
        newHist->Scale(1.0/newEntries);

        PrintHist(newHist, oldHist, GenPlots[i], canvas, legend, "OfflineComp.pdf");

        delete newHist; delete oldHist;
    }

    /* Jet Plots */
    for (size_t i = 0; i < JetPlots.size(); ++i) {
        canvas->Clear();
        canvas->SetLogy(JetLog[i]);

        nbins = JetMax[i] > 100 ? 100 : JetMax[i];

        oldHist = new TH1F("oldHist", "", nbins, 0, JetMax[i]);
        newHist = new TH1F("newHist", "", nbins, 0, JetMax[i]);

        FormatHistogram(newHist, 30);
        FormatHistogram(oldHist, 46);

        string oldDrawCommand = JetPlots[i] + " >> oldHist";
        string newDrawCommand = JetPlots[i] + " >> newHist";

        oldJetChain.Draw(oldDrawCommand.c_str(), "", "goff");
        newJetChain.Draw(newDrawCommand.c_str(), "", "goff");

        oldHist->Scale(1.0/oldEntries);
        newHist->Scale(1.0/newEntries);

        PrintHist(newHist, oldHist, JetPlots[i], canvas, legend, "OfflineComp.pdf");

        delete newHist; delete oldHist;
    }

    canvas->Print("OfflineComp.pdf]");
   
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
