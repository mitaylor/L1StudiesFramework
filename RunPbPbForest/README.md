## Instructions to run the L1Emulator + HIForest with the Run 3 HI menu using CMSSW_13_1_0_pre4

These instructions are for running the L1Ntuples with the offline information included. 

These configs do not enable resimulating HCal TPs, which means that the Minimum Bias trigger thresholds will not be altered from when the data was taken or the MC was generated. If you would like to change the MB thresholds, contact me and I will tell you which lines to alter in the cmsRun configs (currently they are commented out).

### 1. Set up the emulator and the HIForest code

```
cmsrel CMSSW_13_1_0_pre4
cd CMSSW_13_1_0_pre4/src
cmsenv
git cms-init
git remote add cms-l1t-offline git@github.com:cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline l1t-integration-CMSSW_13_1_0_pre4
git cms-merge-topic -u cms-l1t-offline:l1t-integration-v161
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data
git cms-merge-topic CmsHI:forest_CMSSW_13_1_0_pre4
git remote add cmshi git@github.com:CmsHI/cmssw.git
svn export https://github.com/boundino/HltL1Run2021.git/trunk/L1/ADC

git cms-checkdeps -A -a

scram b -j 8
```

### 2. Add the Run 3 HI menu to the emulator

```
git cms-addpkg L1Trigger/L1TCommon
git cms-addpkg L1Trigger/L1TGlobal
mkdir -p L1Trigger/L1TGlobal/data/Luminosity/startup/
cd L1Trigger/L1TGlobal/data/Luminosity/startup/
wget https://raw.githubusercontent.com/mitaylor/HIMenus/main/Menus/L1Menu_CollisionsHeavyIons2023_v0_0_1.xml
cd ../../../../../
scram b -j 8
```

Edit the file L1Trigger/Configuration/python/customiseUtils.py by changing the L1TriggerMenuFile: process.TriggerMenu.L1TriggerMenuFile = cms.string('L1Menu_Collisions2022_v1_2_0.xml') → process.TriggerMenu.L1TriggerMenuFile = cms.string('L1Menu_CollisionsHeavyIons2023_v0_0_1.xml')

### 3. Do a local test of cmsRun

For the following instructions please alter the paths to reflect your own setup.

**For processing 2018 data:**

```
cp L1StudiesFramework/RunPbPbForest/forestL1_miniAOD_2018Data.py CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
cmsRun forestL1_miniAOD_2018Data.py
```

**For processing Run 3 MC:**

```
cp L1StudiesFramework/RunPbPbForest/forestL1_miniAOD_Run3MC.py CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
cmsRun forestL1_miniAOD_Run3MC.py
```

**For processing 2022 data:**

```cp L1StudiesFramework/RunForest/forestL1_miniAOD_Run3MC.py CMSSW_12_6_0_pre1/src
cp L1StudiesFramework/RunPbPbForest/forestL1_miniAOD_2022Data_*.py CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
cmsRun forestL1_miniAOD_2022Data_MB.py
cmsRun forestL1_miniAOD_2022Data_ZB.py
```

### 4. Submit CRAB jobs

Edit crabConfig_2018Data_ForestL1.py, crabConfig_Run3MC_ForestL1.py, or crabConfig_2022Data_ForestL1.py to input your storage area, storage site, dataset, and job name. Then for the following instructions please alter the paths to reflect your own setup.

**For processing 2018 data:**

```
cp L1StudiesFramework/RunPbPbForest/crabConfig_2018Data_ForestL1.py CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
crab submit crabConfig_2018Data_ForestL1.py
```

**For processing Run 3 MC:**

```
cp L1StudiesFramework/RunPbPbForest/crabConfig_Run3MC_ForestL1.py CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
crab submit -c crabConfig_Run3MC_ForestL1.py
```

**For processing 2022 data:**

```
cp L1StudiesFramework/RunPbPbForest/crabConfig_2022Data_ForestL1.py CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
crab submit crabConfig_2022Data_ForestL1.py
```

