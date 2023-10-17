## Instructions to run the L1Emulator + HIForest with the Run 3 HI menu using CMSSW_13_2_4

These instructions are for running the L1Ntuples with the offline information included. 

These configs do not enable resimulating HCal TPs, which means that the Minimum Bias trigger thresholds will not be altered from when the data was taken or the MC was generated. If you would like to change the MB thresholds, contact me and I will tell you which lines to alter in the cmsRun configs.

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

### 2. Add the Run 3 HI menu to the emulator

```
git cms-addpkg L1Trigger/L1TCommon
git cms-addpkg L1Trigger/L1TGlobal
mkdir -p L1Trigger/L1TGlobal/data/Luminosity/startup/
cd L1Trigger/L1TGlobal/data/Luminosity/startup/
wget https://raw.githubusercontent.com/mitaylor/HIMenus/main/Menus/L1Menu_CollisionsHeavyIons2023_v1_1_4.xml
cd ../../../../../
scram b -j 8
```

Edit the file L1Trigger/Configuration/python/customiseUtils.py by changing the L1TriggerMenuFile: process.TriggerMenu.L1TriggerMenuFile = cms.string('L1Menu_Collisions2022_v1_2_0.xml') → process.TriggerMenu.L1TriggerMenuFile = cms.string('L1Menu_CollisionsHeavyIons2023_v1_1_4.xml')

### 3. Do a local test of cmsRun

For the following instructions please alter the paths to reflect your own setup.

**For processing 2023 data:**

```
cp L1StudiesFramework/RunPbPbForest/forestL1_PbPb_2023Data.py CMSSW_13_2_4/src
cd CMSSW_13_2_4/src
cmsRun forestL1_PbPb_2023Data.py
```

### 4. Submit CRAB jobs

Edit crabConfig_forestL1_PbPb_2023Data.py to input your storage area, storage site, dataset, and job name. Then for the following instructions please alter the paths to reflect your own setup.

**For processing 2023 data:**

```
cp L1StudiesFramework/RunPbPbForest/crabConfig_forestL1_PbPb_2023Data.py CMSSW_13_2_4/src
cd CMSSW_13_2_4/src
crab submit crabConfig_forestL1_PbPb_2023Data.py
```
