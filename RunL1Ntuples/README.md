## Instructions to run the L1Emulator with the baseline Run 3 HI menu using CMSSW_12_3_0_pre1

These instructions are for creating only the L1Ntuples without the offline information included.

### 1. Set up the emulator

```
cmsrel CMSSW_12_3_0_pre1
cd CMSSW_12_3_0_pre1/src
cmsenv
git cms-init
git remote add cms-l1t-offline git@github.com:cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline l1t-integration-CMSSW_12_3_0
git cms-merge-topic -u cms-l1t-offline:l1t-integration-v114.0-CMSSW_12_3_0
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data
svn export https://github.com/boundino/HltL1Run2021.git/trunk/L1/ADC

git cms-checkdeps -A -a

scram b -j 8
```

### 2. Add the baseline Run 3 HI menu to the emulator

```
git cms-addpkg L1Trigger/L1TCommon
git cms-addpkg L1Trigger/L1TGlobal
mkdir -p L1Trigger/L1TGlobal/data/Luminosity/startup/
cd L1Trigger/L1TGlobal/data/Luminosity/startup/
wget https://raw.githubusercontent.com/mitaylor/HIMenus/main/L1Menu_CollisionsHeavyIons2022_v0_0_0.xml
cd ../../../../../
scram b -j 8
```

Edit the file L1Trigger/Configuration/python/customiseUtils.py by changing the L1TriggerMenuFile: process.TriggerMenu.L1TriggerMenuFile = cms.string('L1Menu_Collisions2016_v2c.xml') → process.TriggerMenu.L1TriggerMenuFile = cms.string('L1Menu_CollisionsHeavyIons2022_v0_0_0.xml')

### 3. Run cmsDriver.py script

For the following instructions please alter the paths to reflect your own setup.

Edit runCmsDriver_2018Data.sh or runCmsDriver_Run3MC to change the test data file or output config file name.

**For processing Run 2 data:**

```
cp L1StudiesFramework/RunL1Ntuples/runCmsDriver_2018Data.sh CMSSW_12_3_0_pre1/src
cd CMSSW_12_3_0_pre1/src
./runCmsDriver_2018Data.py
```

**For processing Run 3 MC:**

```
cp L1StudiesFramework/RunL1Ntuples/runCmsDriver_Run3MC.sh CMSSW_12_3_0_pre1/src
cd CMSSW_12_3_0_pre1/src
./runCmsDriver_Run3MC.py
```

### 4. Do a local test of cmsRun

```
cmsRun L1Ntuple_2018Data.py
```

or

```
cmsRun L1Ntuple_Run3MC.py
```

### 5. Submit CRAB jobs

Edit crabConfig_2018Data_L1.py or crabConfig_Run3MC_L1.py to input your storage area, storage site, dataset, and job name. Then for the following instructions please alter the paths to reflect your own setup.

```
cp L1StudiesFramework/RunL1Ntuples/crabConfig_2018Data.py CMSSW_12_3_0_pre1/src
cd CMSSW_12_3_0_pre1/src
crab submit crabConfig_2018Data_L1.py
```

or

```
cp L1StudiesFramework/RunL1Ntuples/crabConfig_Run3MC.py CMSSW_12_3_0_pre1/src
cd CMSSW_12_3_0_pre1/src
crab submit crabConfig_Run3MC_L1.py
```

