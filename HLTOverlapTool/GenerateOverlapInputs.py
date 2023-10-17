#!/usr/bin/env python 

# Created Oct 2016.
# For run preparation studies for the upcoming pPb run.
# 
# debug:
# 0-  run normally, automatic output root file. 
# 1-  run normally, don't run MakeClass 
#     (must subsequently be run by hand).
# 
# -Jason Kamin 1.Oct.2016

import os
import sys

_j_args = sys.argv

_debug = 0;
_verbosity = 0;
_nEvents = -1;
_mytag = "N_U_L_L";
_inputrate = 1000000;
_fileloctree = "N_U_L_L";
_myrootpath = "root";
_allEvtSize = -1;

for i in range(0,len(_j_args)):
    if _j_args[i] == "--HltTreePath":
        _fileloctree = _j_args[i+1]
    if _j_args[i] == "--mytag":
        _mytag = _j_args[i+1]
    if _j_args[i] == "--rate":
        _inputrate = _j_args[i+1]
    if _j_args[i] == "--nEvents":
        _nEvents = _j_args[i+1]
    if _j_args[i] == "--debug":
        _debug = int(_j_args[i+1])
    if _j_args[i] == "--verbosity":
        _verbosity = int(_j_args[i+1])
    if _j_args[i] == "--customrootpath":
        _myrootpath = _j_args[i+1]
    if _j_args[i] == "--SetAllEvtSizes":
        _allEvtSize = _j_args[i+1]

_bail_out = 0;

if len(_j_args) < 2:
    _bail_out = 1
else:
    _myprescaleinputfile = _j_args[1];
    if len(_j_args) < 6:
        _bail_out = 1
    if _mytag == "N_U_L_L":
        _bail_out = 1
    if _fileloctree == "N_U_L_L":
        _bail_out = 1
    
    testinputfile = open(_j_args[1],'r')
    for line in testinputfile:
        splitline = line.split()
        if len(splitline)<1:
            continue
        if splitline[0].startswith("#") == True:
            continue
        elif len(splitline) != 6:
            if len(splitline)==5 and _allEvtSize!=-1:
                _bail_out = _bail_out
            else:
                _bail_out = 2

if _bail_out == 1:
    print "\nHand me more arguments!"
    print "eg. like this: \n"
    print "./GenerateOverlapInputs.py <file_with_HLTs_and_PSs> --mytag <base_output_name> --rate <full_rate> --HltTreePath <path_to_HltTree_file> "
    print "optional:    --nEvents 100 --verbosity <0 or 1> --debug <0 or 1> --customrootpath <path_to_root>\n"
    print "... quitting... \n"
    sys.exit(0)
elif _bail_out == 2:
    print "\nCheck input file. It should have 6 columns: \n"
    print "#HLT_trigger    L1_seed    PrimaryDataset    L1_prescale    HLT_prescale   Evt_Size_kb/evt"
    print "#___________    _______    ______________    ___________    ____________   _______________"
    print "HLT_SingleMu3   L1_Mu3     SingleMuons       2              10             150            "
    print "etc.... \n"

    print "  OR  "

    print "\nUse option --SetAllEvtSizes <100>  and it can only have 5 columns: \n"
    print "#HLT_trigger    L1_seed    PrimaryDataset    L1_prescale    HLT_prescale "
    print "#___________    _______    ______________    ___________    ____________ "
    print "HLT_SingleMu3   L1_Mu3     SingleMuons       2              10           "
    print "etc.... \n"
    print "... quitting...\n"
    sys.exit(0)


myoutputfilename1 = "Inputs_" + _mytag + ".h"

os.system("cp ./utils/MakeMeAClass.C .")
os.system("cp ./utils/RestOfCFile1.C .")
os.system("cp ./utils/RestOfCFile2.C .")

dothisatprompt = _myrootpath+"  -l -b -q MakeMeAClass.C\\(\\\"" + _fileloctree + "\\\",\\\"" + _mytag + "\\\"\\)"
os.system(dothisatprompt)
if _verbosity == 1:
    print dothisatprompt

#clean up the h file from MakeClass()... 
linecounter = 0;
my_h_filename = "LoopForOverlaps__" + _mytag + ".h";
my_h_file = open(my_h_filename,'r')
my_new_h_file = open("tmp.h",'w')
for line in my_h_file:
    if _fileloctree in line:
        linecounter = linecounter+1
        if _verbosity == 1:
            print line
        if linecounter == 2:
            line = "      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(inputfile);\n"
        if linecounter == 3:
            line = "         f = new TFile(inputfile);\n"
        if linecounter == 4:
            mydirectoryline = line
        #if linecounter == 4: #now we have this directory get set automatically by the MakeClass. 
            #line = "      TDirectory * dir = (TDirectory*)f->Get(\"hltbitanalysis\");\n"
    my_new_h_file.write(line)
my_new_h_file.close()

dothisatprompt = "mv tmp.h " + my_h_filename
os.system(dothisatprompt)
if _verbosity == 1:
    print dothisatprompt

#clean up the C file from MakeClass()... 
linecounter = 0;
my_C_filename = "LoopForOverlaps__" + _mytag + ".C";
my_C_file = open(my_C_filename,'r')
my_new_C_file = open("tmp.C",'w')
for line in my_C_file:
    if my_h_filename in line:
        storeforlater1 = line
        continue
    if "void LoopForOverlaps__" in line:
        my_new_C_file.write("#include \"TROOT.h\"\n")
        my_new_C_file.write("#include \"TStyle.h\"\n")
        my_new_C_file.write("#include \"TFile.h\"\n")
        my_new_C_file.write("#include \"TNtuple.h\"\n")
        my_new_C_file.write("#include \"TF1.h\"\n")
        my_new_C_file.write("#include \"TRandom3.h\"\n")
        my_new_C_file.write("#include \"TVector3.h\"\n")
        my_new_C_file.write("#include \"TRotation.h\"\n")
        my_new_C_file.write("#include \"TLorentzVector.h\"\n")
        my_new_C_file.write("#include \"TBox.h\"\n")
        my_new_C_file.write("#include \"TLine.h\"\n\n")

        my_new_C_file.write("#include <iostream>\n")
        my_new_C_file.write("#include <cmath>\n")
        my_new_C_file.write("#include <unistd.h>\n\n")

        my_new_C_file.write("using namespace std;\n\n")

        newline = "#include                       \"" + myoutputfilename1 + "\"\n"
        my_new_C_file.write(newline)
        newline = "const char outputfile[500] = \"Overlaps_" + _mytag      + ".root\";\n"
        my_new_C_file.write(newline)
        newline = "const char inputfile[500]  = \""          + _fileloctree + "\";\n\n"
        my_new_C_file.write(newline)
        my_new_C_file.write(storeforlater1+"\n")
    my_new_C_file.write(line)
    if "Loop on all entries" in line:
        break
#we'll finish this job up in a bit... 


#make list of tree variables
rootfilefortree = open("MakeTreeVars_tmp.C",'w')
rootfilefortree.write("#include \"TFile.h\"\n")
rootfilefortree.write("#include \"TTree.h\"\n\n")
rootfilefortree.write("void MakeTreeVars_tmp(const char *filename)\n\n")
rootfilefortree.write("{\n")
rootfilefortree.write("  char saythis[500];\n\n")
rootfilefortree.write("  TFile *f = TFile::Open(filename);\n")
if 'mydirectoryline' in locals():
    rootfilefortree.write(mydirectoryline)
    rootfilefortree.write("  TTree *HltTree = (TTree*)dir->Get(\"HltTree\");\n")
else:
    rootfilefortree.write("  TTree *HltTree = (TTree*)f->Get(\"HltTree\");\n")
rootfilefortree.write("  HltTree->Show();\n\n")
rootfilefortree.write("  return;\n")
rootfilefortree.write("}\n")
rootfilefortree.close()

dothisatprompt = _myrootpath+" -l -q MakeTreeVars_tmp.C\(\\\""+_fileloctree+"\\\"\\) | grep -v _Prescl | grep HLT > TreeVarsTmp.txt"
os.system(dothisatprompt)
os.system("rm MakeTreeVars_tmp.C")

filefortreevars = open("TreeVarsTmp.txt",'r')
AllHltsInTree = [];
for line in filefortreevars:
    splitline = line.split()
    AllHltsInTree.append(splitline[0])
filefortreevars.close()
os.system("rm TreeVarsTmp.txt")


#read in the text input file, fill variables... 
myfile = open(_myprescaleinputfile, 'r')

words = [];
FoundL1inList = -1;
FoundPDinList = -1;
UsedL1Seeds     = [];
UsedPDs         = [];
UsedHLTtriggers = [];
UsedL1Seeds = [];
L1prescales = [];
HLTprescales = [];
L1StringForHlt = [];
PDForHlt = [];
EvtSizeHlt = [];

L1sAreFuckedUp = 0;
GarbageTriggerIndex = [];

print '\n'
i=0;
for line in myfile:
    splitline = line.split()
    if len(splitline)<1:
        continue
    words.append(splitline)
    if words[i][0].startswith("#") == True:
        i=i+1
        continue

    #look for PD in list and/or add it...
    for j in range(0,len(UsedPDs)):
        if UsedPDs[j] == words[i][2]:
            if FoundPDinList < 0:
                FoundPDinList = j
    if FoundPDinList < 0:
        UsedPDs.append(words[i][2])
    FoundPDinList = -1

    #look for L1 in list and/or add it...
    for j in range(0,len(UsedL1Seeds)):
        if UsedL1Seeds[j] == words[i][1]:
            if FoundL1inList < 0:
                FoundL1inList = j
                if L1prescales[j] != words[i][3]:
                    print 'L1 prescales don\'t match !!'
                    print 'check  ', words[i][1]
                    L1sAreFuckedUp = 1
    if FoundL1inList < 0:
        UsedL1Seeds.append(words[i][1])
        if words[i][3] == "0":
            words[i][3] = 200000
        L1prescales.append(words[i][3])
    FoundL1inList = -1

    #check if hlt exists in tree and then
    #try to find some similar name if it doesn't
    itDOESexist = 0;
    StringContainsVersionNum = 0
    for k in range(0,len(AllHltsInTree)):
        if words[i][0] == AllHltsInTree[k]:
            itDOESexist = 1
    if itDOESexist == 0:
        print "warning:: ",words[i][0],"    is NOT in HltTree !! (will look for something similar in tree...)"
        for i_v in range(1,20):
            if words[i][0].endswith("_v"+str(i_v)):
                StringContainsVersionNum = 1
                break
        if StringContainsVersionNum == 1:
            for k in range(0,len(AllHltsInTree)):
                if words[i][0][:-3] == AllHltsInTree[k][:-3]:
                    print "    found ",AllHltsInTree[k]," ... will replace input string."
                    words[i][0] = AllHltsInTree[k]
                    itDOESexist = 1
                    break
        elif StringContainsVersionNum == 0 and itDOESexist == 0:
            for k in range(0,len(AllHltsInTree)):
                if words[i][0] == AllHltsInTree[k][:-3]:
                    print "    found ",AllHltsInTree[k]," ... will replace input string."
                    words[i][0] = AllHltsInTree[k]
                    itDOESexist = 1
                    break
    if itDOESexist == 0:
        GarbageTriggerIndex.append(i)
        print "\n\n  =============================================================="
        print     "  || !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        print     "  || Couldn't find anything matching in Tree... will bail out !"
        print     "  || Please check ",words[i][0]
        print     "  || !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        print    "  ==============================================================\n"
        _bail_out = 1;

    UsedHLTtriggers.append(words[i][0])
    L1StringForHlt.append(words[i][1])
    PDForHlt.append(words[i][2])
    if words[i][4] == "0":
        words[i][4] = 200000
    HLTprescales.append(words[i][4])
    if _allEvtSize == -1:
        EvtSizeHlt.append(words[i][5])
    else:
        EvtSizeHlt.append(_allEvtSize)

    i=i+1


if _verbosity == 1:
    print 'Here are the L1 seeds we used: '
    for i in range(0,len(UsedL1Seeds)):
        print UsedL1Seeds[i]

if L1sAreFuckedUp == 1:
    print 'L1\'s don\'t match somewhere !!'
    sys.exit(0)

if _bail_out == 1:
    print "\nSome input trigger strings are not in the HltTree, check your inputs !"
    print "Specifically:"

    for i in range(0,len(GarbageTriggerIndex)):
        print " ",words[GarbageTriggerIndex[i]][0]
    print "\n"
    sys.exit(0)

print '\n ... done ! \n'


#finish cleaning up the C file from MakeClass()
my_new_C_file.write("\n\n  Long64_t nentries = fChain->GetEntriesFast();\n");
my_new_C_file.write("  Long64_t Userentries = " + str(_nEvents) + ";\n");
my_new_C_file.write("  if(Userentries>0 && Userentries<nentries) nentries = Userentries;\n");
restofcfile = open("RestOfCFile1.C", 'r')
for line in restofcfile:
    my_new_C_file.write(line)

my_new_C_file.write("    Int_t My_L1[] = {\n")
#my_new_C_file.write("  0                                                                ,\n")#leftover legacy needed in 2015 (I forget why!)
for i in range(0,len(UsedL1Seeds)):
    my_new_C_file.write("  ")
    my_new_C_file.write(UsedL1Seeds[i])
    for j in range(0,63-len(UsedL1Seeds[i])):
        my_new_C_file.write(" ")
    if i < len(UsedL1Seeds)-1:
        my_new_C_file.write(",\n")
    else:
        my_new_C_file.write("\n  };\n\n")

my_new_C_file.write("    Int_t MyHLT[] = {\n")
for i in range(0,len(UsedHLTtriggers)):
    my_new_C_file.write("  ")
    my_new_C_file.write(UsedHLTtriggers[i])
    for j in range(0,63-len(UsedHLTtriggers[i])):
        my_new_C_file.write(" ")
    if i < len(UsedHLTtriggers)-1:
        my_new_C_file.write(",\n")
    else:
        my_new_C_file.write("\n  };\n\n")

my_new_C_file.write("    Int_t MyL1ForHlt[] = {\n")
for i in range(0,len(L1StringForHlt)):
    my_new_C_file.write("  ")
    my_new_C_file.write(L1StringForHlt[i])
    for j in range(0,63-len(L1StringForHlt[i])):
        my_new_C_file.write(" ")
    if i < len(L1StringForHlt)-1:
        my_new_C_file.write(",\n")
    else:
        my_new_C_file.write("\n  };\n\n")

restofcfile = open("RestOfCFile2.C", 'r')
for line in restofcfile:
    my_new_C_file.write(line)
my_new_C_file.close()

dothisatprompt = "mv tmp.C " + my_C_filename
os.system(dothisatprompt)
if _verbosity == 1:
    print dothisatprompt



#Make the "Inputs__..." file
#this contains all the prescales and organization
#for the PDs and L1->HLT paths.
myoutputfile1 = open(myoutputfilename1, 'w')
myoutputfile1.write("Double_t FullRate = ")
myoutputfile1.write(str(_inputrate))
myoutputfile1.write(";// Hz \n")
myoutputfile1.write("static const int nTriggers_L1 = ")
myoutputfile1.write(str(len(UsedL1Seeds)))
myoutputfile1.write(";\n")
myoutputfile1.write("static const int nTriggersHlt = ")
myoutputfile1.write(str(len(UsedHLTtriggers)))
myoutputfile1.write(";\n")
myoutputfile1.write("static const int nPrimaryDSet = ")
myoutputfile1.write(str(len(UsedPDs)))
myoutputfile1.write(";\n\n")

myoutputfile1.write("char PDStrings[nPrimaryDSet][35] = {\n")
for i in range(0,len(UsedPDs)):
    myoutputfile1.write("  \"")
    for j in range(0,33-len(UsedPDs[i])):
        myoutputfile1.write(" ")
    myoutputfile1.write(UsedPDs[i])
    if i < len(UsedPDs)-1:
        myoutputfile1.write("\",\n")
    else:
        myoutputfile1.write("\"\n};\n\n")

myoutputfile1.write("char TrStringsHlt[nTriggersHlt][65] = {\n")
for i in range(0,len(UsedHLTtriggers)):
    myoutputfile1.write("  \"")
    myoutputfile1.write(UsedHLTtriggers[i])
    for j in range(0,63-len(UsedHLTtriggers[i])):
        myoutputfile1.write(" ")
    if i < len(UsedHLTtriggers)-1:
        myoutputfile1.write("\",\n")
    else:
        myoutputfile1.write("\"\n};\n\n")

myoutputfile1.write("char L1StringForHlt[nTriggersHlt][65] = {\n")
for i in range(0,len(L1StringForHlt)):
    myoutputfile1.write("  \"")
    myoutputfile1.write(L1StringForHlt[i])
    for j in range(0,63-len(L1StringForHlt[i])):
        myoutputfile1.write(" ")
    if i < len(L1StringForHlt)-1:
        myoutputfile1.write("\",\n")
    else:
        myoutputfile1.write("\"\n};\n\n")

myoutputfile1.write("const char TrStrings_L1[nTriggers_L1][65] = {\n")
for i in range(0,len(UsedL1Seeds)):
    myoutputfile1.write("  \"")
    myoutputfile1.write(UsedL1Seeds[i])
    for j in range(0,63-len(UsedL1Seeds[i])):
        myoutputfile1.write(" ")
    if i < len(UsedL1Seeds)-1:
        myoutputfile1.write("\",\n")
    else:
        myoutputfile1.write("\"\n};\n\n")

myoutputfile1.write("char PDForHlt[nTriggersHlt][35] = {\n")
for i in range(0,len(PDForHlt)):
    myoutputfile1.write("  \"")
    for j in range(0,33-len(PDForHlt[i])):
        myoutputfile1.write(" ")
    myoutputfile1.write(PDForHlt[i])
    if i < len(PDForHlt)-1:
        myoutputfile1.write("\",\n")
    else:
        myoutputfile1.write("\"\n};\n\n\n")

myoutputfile1.write("Int_t PDIndexEdges[nPrimaryDSet] = {0};\n")
myoutputfile1.write("Int_t PDForHltID[nTriggersHlt] = {0};\n")
myoutputfile1.write("Int_t MaskPathStatusHlt[nTriggersHlt] = {0};\n\n\n")

myoutputfile1.write("Double_t PreScale_L1[nTriggers_L1] = {\n")
for i in range(0,len(L1prescales)):
    myoutputfile1.write("  ")
    myoutputfile1.write(str(L1prescales[i]))
    for j in range(0,7-len(str(L1prescales[i]))):
        myoutputfile1.write(" ")
    if i < len(L1prescales)-1:
        myoutputfile1.write(", // \"")
        myoutputfile1.write(UsedL1Seeds[i])
        for j in range(0,63-len(UsedL1Seeds[i])):
            myoutputfile1.write(" ")
        myoutputfile1.write("\",\n")
    else:
        myoutputfile1.write("  // \"")
        myoutputfile1.write(UsedL1Seeds[i])
        for j in range(0,63-len(UsedL1Seeds[i])):
            myoutputfile1.write(" ")
        myoutputfile1.write("\"\n};\n\n")

myoutputfile1.write("Double_t PreScaleHlt[nTriggersHlt] = {\n")
for i in range(0,len(HLTprescales)):
    myoutputfile1.write("  ")
    myoutputfile1.write(str(HLTprescales[i]))
    for j in range(0,7-len(str(HLTprescales[i]))):
        myoutputfile1.write(" ")
    if i < len(HLTprescales)-1:
        myoutputfile1.write(", // \"")
        myoutputfile1.write(UsedHLTtriggers[i])
        for j in range(0,63-len(UsedHLTtriggers[i])):
            myoutputfile1.write(" ")
        myoutputfile1.write("\",\n")
    else:
        myoutputfile1.write("  // \"")
        myoutputfile1.write(UsedHLTtriggers[i])
        for j in range(0,63-len(UsedHLTtriggers[i])):
            myoutputfile1.write(" ")
        myoutputfile1.write("\"\n};\n\n")

myoutputfile1.write("Double_t EvtSizeHlt[nTriggersHlt] = {\n")
for i in range(0,len(EvtSizeHlt)):
    myoutputfile1.write("  ")
    myoutputfile1.write(str(EvtSizeHlt[i]))
    for j in range(0,7-len(str(EvtSizeHlt[i]))):
        myoutputfile1.write(" ")
    if i < len(EvtSizeHlt)-1:
        myoutputfile1.write(", // \"")
        myoutputfile1.write(UsedHLTtriggers[i])
        for j in range(0,63-len(UsedHLTtriggers[i])):
            myoutputfile1.write(" ")
        myoutputfile1.write("\",\n")
    else:
        myoutputfile1.write("  // \"")
        myoutputfile1.write(UsedHLTtriggers[i])
        for j in range(0,63-len(UsedHLTtriggers[i])):
            myoutputfile1.write(" ")
        myoutputfile1.write("\"\n};\n\n")

myoutputfile1.write("char saythis[100];\n")
myoutputfile1.write("char saythis1[100];\n")
myoutputfile1.write("char saythis2[100];\n")
myoutputfile1.write("char saythis3[100];\n")
myoutputfile1.write("char saythis4[100];\n")
myoutputfile1.write("char saythis5[100];\n")
myoutputfile1.write("char saythis6[100];\n")
myoutputfile1.write("char saythis7[500];\n")
myoutputfile1.write("char saythis8[500];\n")
myoutputfile1.write("Int_t L1PathForHLT[nTriggersHlt] = {0};\n\n")

myoutputfile1.write("// c\'est parti ! \n\n")
myoutputfile1.close()

os.system("rm MakeMeAClass.C")
os.system("rm RestOfCFile1.C")
os.system("rm RestOfCFile2.C")


#now make a quick macro to run the MakeClass to 
#loop over the HltTree...
myquickrootmacro = open("RunTreeMacro.C", 'w')
myquickrootmacro.write("#include \""+my_C_filename+"\"\n\n")
myquickrootmacro.write("void RunTreeMacro()\n")
myquickrootmacro.write("{\n\n")
myquickrootmacro.write("  LoopForOverlaps__"+_mytag+" t;\n")
myquickrootmacro.write("  t.Loop();\n\n")
myquickrootmacro.write("  return;\n")
myquickrootmacro.write("}\n")
myquickrootmacro.close()

if _debug > 0:
    print "\n\n"
    print "_________________________________________________"
    linecounter = 0;
    my_C_file_instructions = open(my_C_filename,'r')
    for line in my_C_file_instructions:
        linecounter +=1
        if "//" in line:
            if linecounter<30:
                print line
    print "_________________________________________________"
    print "\n"
    sys.exit(0)

if _verbosity == 1:
    os.system("more RunTreeMacro.C")
dothisatprompt = _myrootpath+" -l -q RunTreeMacro.C"
os.system(dothisatprompt)
os.system("rm RunTreeMacro.C")



#check the Inputs file... 
if _verbosity == 1:
    dothisatprompt = "more "
    dothisatprompt = dothisatprompt + myoutputfilename1 + "\n"
    os.system(dothisatprompt)

print "\n\n"
if _verbosity ==1:
    print "_________________________________________________"
    linecounter = 0;
    my_C_file_instructions = open(my_C_filename,'r')
    for line in my_C_file_instructions:
        linecounter +=1
        if "//" in line:
            if linecounter<30:
                print line
print "_________________________________________________"
print "\n"

