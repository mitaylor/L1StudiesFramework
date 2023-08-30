## Instructions to run the L1Emulator + HIForest with the Run 3 PP reference menu using CMSSW_9_4_10

These instructions are for running the L1Ntuples with the offline information included. 

These configs do not enable resimulating HCal TPs, which means that the Minimum Bias trigger thresholds will not be altered from when the data was taken or the MC was generated. If you would like to change the MB thresholds, contact me and I will tell you which lines to alter in the cmsRun configs (currently they are commented out).

### 1. Set up the emulator

```
cmsrel CMSSW_9_4_10
cd CMSSW_9_4_10/src
cmsenv
git cms-init
git cms-merge-topic -u CmsHI:forest_CMSSW_9_4_10
git remote add cmshi git@github.com:CmsHI/cmssw.git
cd HeavyIonsAnalysis/JetAnalysis/python/jets
./makeJetSequences.sh
cd ../../../..
git remote add cms-l1t-offline git@github.com:cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline --no-tags
git cms-merge-topic -u cms-l1t-offline:l1t-integration-v97.13
git cms-addpkg L1Trigger/L1TCommon
git cms-addpkg L1Trigger/L1TMuon
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TMuon.git L1Trigger/L1TMuon/data

scram b -j 8
```

### 2. Add the CaloParams definition

Add the following to L1Trigger/Configuration/python/customiseSettings.py

```
def L1TSettingsToCaloStage2Params_2017_v1_8_4_ppRef(process):
    process.load("L1Trigger.L1TCalorimeter.caloStage2Params_2017_v1_8_4_ppRef_cfi")
    return process
```

### 3. Do a local test of cmsRun

For the following instructions please alter the paths to reflect your own setup.

**For processing 2017 data:**

```
cp L1StudiesFramework/RunPPForest2017/forestL1_PP_2017Data.py CMSSW_9_4_10/src
cd CMSSW_9_4_10/src
cmsRun forestL1_PP_2017Data.py
```

### 4. Submit CRAB jobs

Edit crabConfig_forestL1_PP_2017Data.py to input your storage area, storage site, dataset, and job name. Then for the following instructions please alter the paths to reflect your own setup.

**For processing 2017 data:**

```
cp L1StudiesFramework/RunPPForest/crabConfig_forestL1_PP_2017Data.py CMSSW_9_4_10/src
cd CMSSW_9_4_10/src
crab submit -c crabConfig_forestL1_PP_2017Data.py
```
