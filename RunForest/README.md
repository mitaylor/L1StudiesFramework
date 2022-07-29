## Instructions to run the L1Emulator + HIForest with the Run 3 HI menu using CMSSW_12_4_0

These instructions are for running the L1Ntuples with the offline information included.

### 1. Set up the emulator and the HIForest code

```
cmsrel CMSSW_12_4_0
cd CMSSW_12_4_0/src
cmsenv
git cms-init
git remote add cms-l1t-offline git@github.com:cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline l1t-integration-CMSSW_12_4_0
git cms-merge-topic -u cms-l1t-offline:l1t-integration-v132.0
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data
git cms-merge-topic CmsHI:forest_CMSSW_12_4_0
git cms-merge-topic -u kakwok:CLCT_thresholds

git cms-checkdeps -A -a

scram b -j 8
```

### 2. Add the Run 3 HI menu to the emulator

```
git cms-addpkg L1Trigger/L1TCommon
git cms-addpkg L1Trigger/L1TGlobal
mkdir -p L1Trigger/L1TGlobal/data/Luminosity/startup/
cd L1Trigger/L1TGlobal/data/Luminosity/startup/
wget https://raw.githubusercontent.com/mitaylor/HIMenus/main/Menus/L1Menu_CollisionsHeavyIons2022_v0_0_4.xml
cd ../../../../../
scram b -j 8
```

Edit the file L1Trigger/Configuration/python/customiseUtils.py by changing the L1TriggerMenuFile: process.TriggerMenu.L1TriggerMenuFile = cms.string('L1Menu_Collisions2022_v1_0_1.xml') → process.TriggerMenu.L1TriggerMenuFile = cms.string('L1Menu_CollisionsHeavyIons2022_v0_0_4.xml')

### 3. Do a local test of cmsRun

```
cp L1StudiesFramework/RunForest/forestL1_miniAOD_2018Data.py CMSSW_12_4_0/src
cd CMSSW_12_4_0/src
cmsRun forestL1_miniAOD_2018Data.py
```

or

```
cp L1StudiesFramework/RunForest/forestL1_miniAOD_Run3MC.py CMSSW_12_4_0/src
cd CMSSW_12_4_0/src
cmsRun forestL1_miniAOD_Run3MC.py
```

### 4. Submit CRAB jobs

Edit crabConfig_2018Data_ForestL1.py or crabConfig_Run3MC_ForestL1.py to input your storage area, storage site, dataset, and job name. Then for the following instructions please alter the paths to reflect your own setup.

```
cp L1StudiesFramework/RunForest/crabConfig_2018Data_ForestL1.py CMSSW_12_4_0/src
cd CMSSW_12_4_0/src
crab submit crabConfig_2018Data_ForestL1.py
```

or

```
cp L1StudiesFramework/RunForest/crabConfig_Run3MC_ForestL1.py CMSSW_12_4_0/src
cd CMSSW_12_4_0/src
crab submit crabConfig_Run3MC_ForestL1.py
```
