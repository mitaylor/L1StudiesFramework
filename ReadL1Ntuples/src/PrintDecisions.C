/*
Input:  L1Ntuple made with one of the following menu versions
    L1Menu_CollisionsHeavyIons2018_v4_2_0.xml --> Menu::Y2018_V4_2_0
    L1Menu_CollisionsHeavyIons2022_v0_0_0.xml --> Menu::Y2022_V0_0_0
    L1Menu_CollisionsHeavyIons2022_v0_0_1.xml --> Menu::Y2022_V0_0_1
    L1Menu_CollisionsHeavyIons2022_v0_0_2.xml --> Menu::Y2022_V0_0_2
    L1Menu_CollisionsHeavyIons2022_v0_0_3.xml --> Menu::Y2022_V0_0_3
    L1Menu_CollisionsHeavyIons2022_v0_0_4.xml --> Menu::Y2022_V0_0_4
    L1Menu_CollisionsHeavyIons2022_v0_0_5.xml --> Menu::Y2022_V0_0_5
    L1Menu_CollisionsHeavyIons2022_v1_0_0.xml --> Menu::Y2022_V1_0_0
    L1Menu_CollisionsHeavyIons2022_v1_0_1.xml --> Menu::Y2022_V1_0_1
    L1Menu_CollisionsHeavyIons2022_v1_1_0.xml --> Menu::Y2022_V1_1_0
    L1Menu_CollisionsHeavyIons2023_v0_0_0.xml --> Menu::Y2023_V0_0_0
    L1Menu_CollisionsHeavyIons2023_v0_0_1.xml --> Menu::Y2023_V0_0_1
    L1Menu_CollisionsHeavyIons2023_v0_0_2.xml --> Menu::Y2023_V0_0_1
    L1Menu_CollisionsHeavyIons2023_v0_0_3.xml --> Menu::Y2023_V0_0_1
Output: A list of how many times each menu trigger was passed
Note: If you are using your own custom menu, add it to Menu.h following the instructions in data/README
*/

#include "../include/Menu.h"

#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TDirectory.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

int Print(char const* input) {
    /* initilaize menu */
    Menu menu(Menu::Y2023_V0_0_3);

    /* file, trees, and branches */
    TFile* inFile = new TFile(input, "read");
    TDirectory* emuDir = inFile->GetDirectory("l1uGTEmuTree");
    TTreeReader emuReader("L1uGTTree", emuDir);
    TTreeReaderValue<std::vector<bool>> emuDecision(emuReader, "m_algoDecisionInitial");

    Long64_t totalEvents = emuReader.GetEntries(true);

    /* create data structures for outputs */
    vector<bool>* emuDecisionVec;
    Int_t emuCounts[menu.NBits] = {0};

    while (emuReader.Next()) {
        emuDecisionVec = emuDecision.Get();

        for (Int_t i = 0; i < menu.NBits; ++i) {
            emuCounts[i] += (*emuDecisionVec)[i];
        }
    }

    cout << "Analyzed " << totalEvents << " events" << endl;

    for (Int_t i = 0; i < menu.NBits; ++i) {
        if (menu.Map().count(i)) {
            cout << "Emulation has passed trigger bit " << i << " " << menu.Map()[i] << " " << emuCounts[i]
                << " times" << endl;
        }
        else {
            if (emuCounts[i] != 0) {
                cout << "Error: emulation has passed nonexistant trigger bit " << i << " " << menu.Map()[i]
                    << " " << emuCounts[i] << " times" << endl;
            }
        }
    }
   
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 2)
        return Print(argv[1]);

    else {
        cout << "ERROR: Please pass a single L1Ntuple root file." << endl;
        return -1;
    }
}
