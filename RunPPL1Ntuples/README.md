## Instructions to run the L1Emulator with the Run 3 PP reference menu using CMSSW_13_2_0_pre1

These instructions are for running the L1Ntuples with the offline information included. They can be used to run on 2017 data or on 2023 MC, but in order to process 2017 MC please use the instructions on the Twiki.

These configs do not enable resimulating HCal TPs, which means that the Minimum Bias trigger thresholds will not be altered from when the data was taken or the MC was generated. If you would like to change the MB thresholds, contact me and I will tell you which lines to alter in the cmsRun configs.

### 1. Set up the emulator

```
cmsrel CMSSW_13_2_0_pre1
cd CMSSW_13_2_0_pre1/src
cmsenv
git cms-init
git remote add mitaylor git@github.com:mitaylor/cmssw.git
git fetch mitaylor CMSSW_13_2_X_merge
git cms-merge-topic -u mitaylor:CMSSW_13_2_X_merge
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data
svn export https://github.com/boundino/HltL1Run2021.git/trunk/L1/ADC

git cms-checkdeps -A -a

scram b -j 8
```

### 2. Add the Run 3 PP reference menu to the emulator

```
git cms-addpkg L1Trigger/L1TCommon
git cms-addpkg L1Trigger/L1TGlobal
mkdir -p L1Trigger/L1TGlobal/data/Luminosity/startup/
cd L1Trigger/L1TGlobal/data/Luminosity/startup/
wget wget https://raw.githubusercontent.com/mitaylor/HIMenus/main/Menus/L1Menu_CollisionsPPRef2023_v0_0_2.xml
cd ../../../../../
scram b -j 8
```

Edit the file L1Trigger/Configuration/python/customiseUtils.py by changing the L1TriggerMenuFile: process.TriggerMenu.L1TriggerMenuFile = cms.string('L1Menu_Collisions2022_v1_2_0.xml') → process.TriggerMenu.L1TriggerMenuFile = cms.string('L1Menu_CollisionsPPRef2023_v0_0_2.xml')

### 3. Do a local test of cmsRun

For the following instructions please alter the paths to reflect your own setup.

**For processing 2017 data:**

```
cp L1StudiesFramework/RunPPL1Ntuples/l1Ntuple_AOD_2017Data.py CMSSW_13_2_0_pre1/src
cd CMSSW_13_2_0_pre1/src
cmsRun l1Ntuple_AOD_2017Data.py
```

**For processing Run 3 MC:**

```
cp L1StudiesFramework/RunPPL1Ntuples/l1Ntuple_AOD_2023MC.py CMSSW_13_2_0_pre1/src
cd CMSSW_13_2_0_pre1/src
cmsRun l1Ntuple_AOD_2023MC.py
```

### 4. Submit CRAB jobs

Edit crabConfig_Run3MC_L1.py or crabConfig_2017Data_L1.py to input your storage area, storage site, dataset, and job name. Then for the following instructions please alter the paths to reflect your own setup.

**For processing 2018 data:**

```
cp L1StudiesFramework/RunPPL1Ntuples/crabConfig_2017Data_L1.py CMSSW_13_2_0_pre1/src
cd CMSSW_13_2_0_pre1/src
crab submit -c crabConfig_2017Data_L1.py
```

**For processing Run 3 MC:**

```
cp L1StudiesFramework/RunPPL1Ntuples/crabConfig_Run3MC_L1.py CMSSW_13_2_0_pre1/src
cd CMSSW_13_2_0_pre1/src
crab submit -c crabConfig_Run3MC_L1.py
```
