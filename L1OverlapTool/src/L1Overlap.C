/*
Input: Root file output of the L1 rate-estimation tool and name for output plot
Output: Plot of the overlap between each L1 seed
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
#include "TLatex.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

int L1Overlap(char const* input, char const* output) {
    /* get input histogram */
    auto file = new TFile(input, "read");
    auto corSeeds = (TH2F*) file->Get("Col0/cor_Seeds");
    auto corSeedsPercent = (TH2F*) corSeeds->Clone();

    /* scale each row so that the entries correspond to the percent overlap with that L1 seed */
    for (int i = 1; i <= corSeeds->GetNbinsX(); ++i) {
        for (int j = 1; j <= corSeeds->GetNbinsY(); ++j) {
            if (corSeeds->GetBinContent(j, j) != 0) {
                auto percentOverlap = corSeeds->GetBinContent(i, j) / corSeeds->GetBinContent(j, j) * 100;
                corSeedsPercent->SetBinContent(i, j, percentOverlap);
            } else {
                corSeedsPercent->SetBinContent(i, j, 0);
            }
        }
    }

    /* plot histogram */
    double leftMargin = 700;
    double rightMargin = 300;
    double bottomMargin = 700;
    double topMargin = 100;
    double padWidth = 1000;
    double plotWidth = padWidth + leftMargin + rightMargin;
    double plotHeight = padWidth + bottomMargin + topMargin;

    corSeedsPercent->SetStats(0);

    corSeedsPercent->GetXaxis()->SetLabelSize(1500/plotWidth/corSeedsPercent->GetNbinsX());
    corSeedsPercent->GetYaxis()->SetLabelSize(1500/plotHeight/corSeedsPercent->GetNbinsY());
    corSeedsPercent->GetZaxis()->SetLabelSize(0.02);

    corSeedsPercent->SetTitle("");

    corSeedsPercent->SetMinimum(-0.000001);
    corSeedsPercent->SetMaximum(100);

    auto canvas = new TCanvas("canvas", "", 0 , 0, plotWidth, plotHeight);
    canvas->SetLeftMargin(leftMargin/plotWidth);
    canvas->SetBottomMargin(bottomMargin/plotHeight);
    canvas->SetRightMargin(rightMargin/plotWidth);
    canvas->SetTopMargin(topMargin/plotHeight);
    // canvas->SetLogy(1);

    corSeedsPercent->Draw("COLZ0");

    TLatex* text = new TLatex();
    text->SetTextSize(topMargin*0.6);
    text->SetTextFont(43);
    text->SetTextAlign(22);
    text->DrawLatexNDC((leftMargin + padWidth/2)/plotWidth, (plotHeight-topMargin*0.6)/plotHeight, "L1 Overlap");

    string outputPDF = string(output) + ".pdf";
    canvas->Print(outputPDF.c_str());

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 3)
        return L1Overlap(argv[1], argv[2]);
    else {
        cout << "ERROR" << endl;
        return -1;
    }
}
