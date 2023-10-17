#include "TFile.h"
#include "TTree.h"

void MakeMeAClass(const char *filename, const char *mytag)
{
  char saythis[500];

  TFile *f1 = TFile::Open(filename);
  TDirectory *mydir; 
  mydir = (TDirectory*)f1->GetDirectory("hltanalysis");
  if(!mydir)
    mydir = (TDirectory*)f1->GetDirectory("hltbitanalysis");
  if(!mydir)
    mydir = (TDirectory*)f1->GetDirectory("");
  TTree *HltTree = (TTree*)mydir->Get("HltTree");
  sprintf(saythis,"LoopForOverlaps__%s",mytag);
  //cout << saythis << endl;
  HltTree->MakeClass(saythis);

  return;
}
