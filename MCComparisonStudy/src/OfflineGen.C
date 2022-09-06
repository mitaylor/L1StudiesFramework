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

    TChain oldGenChain("HiGenParticleAna/hi");
    FillChain(oldGenChain, oldFiles);

    /* read in 2022 information */
    vector<string> newFiles;
    GetFiles(newInput, newFiles);

    TChain newGenChain("HiGenParticleAna/hi");
    FillChain(newGenChain, newFiles);

    /* define plots */
    vector<string> GenName = { 
        "Gen Multiplicity", 
        "Gen Pt",
        "Gen PDG",
        "Gen PDG",
        "Gen Photon Pt",
        "Gen Charged Particle Pt",
        "Gen Charged Particle Eta",
        };
    vector<string> GenPlots = { 
        "mult", 
        "pt", 
        "pdg",
        "pdg",
        "pt",
        "pt",
        "eta"
        };
    vector<string> GenSel = {
        "",
        "",
        "pdg < 350 && pdg > -350",
        "pdg < 25 && pdg > -25",
        "pdg == 22",
        "chg != 0",
        "chg != 0"
    };
    vector<double> GenMax = { 22000, 60, 350, 25, 60, 60, 5.5 };
    vector<double> GenMin = { 0, 0, -350, -25, 0, 0, -5.5};
    vector<int> GenLog { 0, 1, 1, 1, 1, 1, 0 };

    /* create histograms for energy sum plots */
    int nbins = 100;

    TH1F* oldHist = new TH1F("oldTemp", "", nbins, 0, 1);
    TH1F* newHist = new TH1F("newTemp", "", nbins, 0, 1);

    FormatHistogram(newHist, 30);
    FormatHistogram(oldHist, 46);

    int oldEntries = oldGenChain.GetEntries();
    int newEntries = newGenChain.GetEntries();

    /* customize energy sum histogram draw options */
    auto legend = new TLegend(0.55, 0.75 ,0.85, 0.85);
    legend->SetTextSize(0.03);
    legend->AddEntry(oldHist, "2018 MB MC", "p");
    legend->AddEntry(newHist, "2022 MB MC", "p");

    /* plot the energy sum distributions */
    auto canvas = new TCanvas("canvas", "", 0 , 0, 500, 500);
    canvas->SetLeftMargin(0.15);
    canvas->SetBottomMargin(0.15);
    canvas->Print("GenComparison.pdf[");

    /* Gen Plots */
    for (size_t i = 0; i < GenPlots.size(); ++i) {
        canvas->Clear();
        canvas->SetLogy(GenLog[i]);

        nbins = GenMax[i] - GenMin[i] > 100 ? 100 : GenMax[i] - GenMin[i];

        oldHist = new TH1F("oldHist", "", nbins, GenMin[i], GenMax[i]);
        newHist = new TH1F("newHist", "", nbins, GenMin[i], GenMax[i]);

        FormatHistogram(newHist, 30);
        FormatHistogram(oldHist, 46);

        string oldDrawCommand = GenPlots[i] + " >> oldHist";
        string newDrawCommand = GenPlots[i] + " >> newHist";

        oldGenChain.Draw(oldDrawCommand.c_str(), GenSel[i].c_str(), "goff");
        newGenChain.Draw(newDrawCommand.c_str(), GenSel[i].c_str(), "goff");

        oldHist->Scale(1.0/oldEntries);
        newHist->Scale(1.0/newEntries);

        PrintHist(newHist, oldHist, GenName[i], canvas, legend, "GenComparison.pdf");

        delete newHist; delete oldHist;
    }

    canvas->Print("GenComparison.pdf]");
   
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
