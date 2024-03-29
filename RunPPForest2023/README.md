## Instructions to run the L1Emulator + HIForest with the Run 3 PP Reference menu using CMSSW_13_2_4

These instructions are for running the L1Ntuples with the offline information included. 

### 1. Set up the emulator and the HIForest code

```
cmsrel CMSSW_13_2_4
cd CMSSW_13_2_4/src
cmsenv
git cms-init
git remote add mitaylor git@github.com:mitaylor/cmssw.git
git fetch mitaylor CMSSW_13_2_4_L1HiForest
git cms-merge-topic -u mitaylor:CMSSW_13_2_4_V2
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data

git cms-checkdeps -A -a

scram b -j 8
```

### 2. Do a local test of cmsRun

For the following instructions please alter the paths to reflect your own setup.

**For processing 2023 data:**

```
cp L1StudiesFramework/RunPPForest2023/forestL1_PP_2023Data.py CMSSW_13_2_4/src
cd CMSSW_13_2_4/src
cmsRun forestL1_PP_2023Data.py
```

### 4. Submit CRAB jobs

Edit crabConfig_forestL1_PP_2023Data.py to input your storage area, storage site, dataset, and job name. Then for the following instructions please alter the paths to reflect your own setup.

**For processing 2023 data:**

```
cp L1StudiesFramework/RunPPForest2023/crabConfig_forestL1_PP_2023Data.py CMSSW_13_2_4/src
cd CMSSW_13_2_4/src
crab submit crabConfig_forestL1_PP_2023Data.py
```
