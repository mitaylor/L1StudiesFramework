/*
Input:  Directory containing L1Ntuples made with one of the following menu versions
    L1Menu_CollisionsHeavyIons2018_v4_2_0.xml --> Menu::Y2018_V4_2_0
    L1Menu_CollisionsHeavyIons2022_v0_0_0.xml --> Menu::Y2022_V0_0_0
    L1Menu_CollisionsHeavyIons2022_v0_0_1.xml --> Menu::Y2022_V0_0_1
    L1Menu_CollisionsHeavyIons2022_v0_0_2.xml --> Menu::Y2022_V0_0_2
    L1Menu_CollisionsHeavyIons2022_v0_0_3.xml --> Menu::Y2022_V0_0_3
    L1Menu_CollisionsHeavyIons2022_v0_0_4.xml --> Menu::Y2022_V0_0_4
Output: A list of events which have mismatch passing the centrality triggers, due
to ECal/Hcal/Tower saturation.
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

int CentralityCheck(char const* input) {
    /* initilaize menu */
    Menu menu(Menu::Y2022_V0_0_4);

    /* get list of files in the input directory and subdirectories */
    vector<string> files;
    GetFiles(input, files);

    /* read in emulated uGT trigger decision */
    TChain ugtChain("l1uGTEmuTree/L1uGTTree");
    FillChain(ugtChain, files);
    TTreeReader ugtReader(&ugtChain);
    TTreeReaderValue<vector<bool>> ugtDecision(ugtReader, "m_algoDecisionInitial");

    /* read in calo tower information */
    TChain caloTowerChain("l1CaloTowerEmuTree/L1CaloTowerTree");
    FillChain(caloTowerChain, files);
    TTreeReader caloTowerReader(&caloTowerChain);
    TTreeReaderValue<short> caloNTowers(caloTowerReader, "nTower");
    TTreeReaderValue<vector<short>> caloIEt(caloTowerReader, "iet");
    TTreeReaderValue<vector<short>> caloIEta(caloTowerReader, "ieta");
    TTreeReaderValue<vector<short>> caloIPhi(caloTowerReader, "iphi");

    /* read in energy sum information */
    TChain upgradeTreeChain("l1UpgradeEmuTree/L1UpgradeTree");
    FillChain(upgradeTreeChain, files);
    TTreeReader upgradeTreeReader(&upgradeTreeChain);
    TTreeReaderValue<vector<short>> upgradeSumIEt(upgradeTreeReader, "sumIEt");

    string seed = "L1_Centrality_0_1_BptxAND";
    int threshold = 9414;

    // string seed = "L1_Centrality_0_0p5_BptxAND";
    // int threshold = 9715;

    /* read in information from TTrees */
    int event = 0;
    int sum = 0;
    bool saturation;

    while (ugtReader.Next() && caloTowerReader.Next() && upgradeTreeReader.Next()) {
        sum = 0;
        ++event;
        saturation = false;

        for (int j = 0; j < *caloNTowers; ++j)
        {
            if (TMath::Abs((*caloIEta)[j]) > 29) {
                sum += (*caloIEt)[j];
            }

            if ((*caloIEt)[j] > 500) {
                saturation = true;
            }

        }

        /* emu ugt decision */
        auto l1pass = (*ugtDecision)[menu.Map()[seed]];
        auto sumpass = sum > threshold;

        if (l1pass != sumpass) {
            cout << "Event: " << event << "\t L1 decision: " << l1pass;
            cout << "\t Threshold pass: " << sumpass << endl;
            cout << "\t Centrality bits: " << hex << "0x" << (*upgradeSumIEt)[18];
            cout << "\t Saturation: " << saturation << "\t HF sum: " << sum << endl;
        }
    }
   
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 2)
        return CentralityCheck(argv[1]);

    else {
        cout << "ERROR: Please pass a single path to a directory of root files." << endl;
        return -1;
    }
}
