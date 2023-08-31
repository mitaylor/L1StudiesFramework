## Instructions to run the L1Emulator with the Run 3 PP reference menu using CMSSW_9_4_10

These instructions are for creating only the L1Ntuples without the offline information included. They can be used to run on 2017 data or 2017 MC.

These configs do not enable resimulating HCal TPs, which means that the Minimum Bias trigger thresholds will not be altered from when the data was taken or the MC was generated. If you would like to change the MB thresholds, contact me and I will tell you which lines to alter in the cmsRun configs.

### 1. Set up the emulator

```
cmsrel CMSSW_9_4_10
cd CMSSW_9_4_10/src
cmsenv
git cms-init
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

### 3. Run cmsDriver.py script

For the following instructions please alter the paths to reflect your own setup.

**For processing 2017 data:**

```
cp L1StudiesFramework/RunPPL1Ntuples2017/runCmsDriver_PP_2017Data.sh CMSSW_9_4_10/src
cd CMSSW_9_4_10/src
./runCmsDriver_PP_2017Data.sh
```

**For processing 2017 MC:**

```
cp L1StudiesFramework/RunPPL1Ntuples2017/runCmsDriver_PP_2017MC.sh CMSSW_9_4_10/src
cd CMSSW_9_4_10/src
./runCmsDriver_PP_2017MC.sh
```

###4. Do a local test of cmsRun

**For processing 2017 data:**

```
cmsRun l1Ntuple_PP_2017Data.py
```

**For processing 2017 MC:**

```
cmsRun l1Ntuple_PP_2017MC.py
```

### 5. Submit CRAB jobs

Edit crabConfig_L1_PP_2017Data.py or crabConfig_L1_PP_2017MC.py to input your storage area, storage site, dataset, and job name. Then for the following instructions please alter the paths to reflect your own setup.

**For processing 2017 data:**

```
cp L1StudiesFramework/RunPPL1Ntuples2017/crabConfig_L1_PP_2017Data.py CMSSW_9_4_10/src
cd CMSSW_9_4_10/src
crab submit -c crabConfig_L1_PP_2017Data.py
```

**For processing 2017 MC:**

```
cp L1StudiesFramework/RunPPL1Ntuples2017/crabConfig_L1_PP_2017MC.py CMSSW_9_4_10/src
cd CMSSW_9_4_10/src
crab submit -c crabConfig_L1_PP_2017MC.py
```
