## Instructions to run the L1Emulator with the Run 3 HI menu using CMSSW_13_1_0_pre4

These instructions are for creating only the L1Ntuples without the offline information included.

These configs do not enable resimulating HCal TPs, which means that the Minimum Bias trigger thresholds will not be altered from when the data was taken or the MC was generated. If you would like to change the MB thresholds, contact me and I will tell you which lines to alter in the cmsRun configs.

### 1. Set up the emulator

```
cmsrel CMSSW_13_1_0_pre4
cd CMSSW_13_1_0_pre4/src
cmsenv
git cms-init
git remote add cms-l1t-offline git@github.com:cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline l1t-integration-CMSSW_13_1_0_pre4
git cms-merge-topic -u cms-l1t-offline:l1t-integration-v161
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data
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

### 3. Run cmsDriver.py script

For the following instructions please alter the paths to reflect your own setup.

**For processing 2018 data:**

```
cp L1StudiesFramework/RunPbPbL1Ntuples/runCmsDriver_2018Data.sh CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
./runCmsDriver_2018Data.sh
```

**For processing Run 3 MC:**

```
cp L1StudiesFramework/RunPbPbL1Ntuples/runCmsDriver_Run3MC.sh CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
./runCmsDriver_Run3MC.sh
```

**For processing 2022 data:**

```
cp L1StudiesFramework/RunPbPbL1Ntuples/runCmsDriver_2022Data_*.sh CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
./runCmsDriver_2022Data_MB.sh
./runCmsDriver_2022Data_ZB.sh
```

### 4. Add the Spike Killer settings

If you want to apply the HI spike killer settings, add the following lines to the python config directly after the `process.GlobalTag` declaration.

```
process.GlobalTag.toGet.extend = cms.VPSet(
   cms.PSet(record = cms.string('EcalTPGFineGrainStripEERcd'),
            tag = cms.string('EcalTPGFineGrainStrip_7'),
            connect =cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    ),
    cms.PSet(record = cms.string('EcalTPGSpikeRcd'),
        tag = cms.string('EcalTPGSpike_12'),
        connect =cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    )
)
```

### 5. Do a local test of cmsRun

**For processing 2018 data:**

```
cmsRun l1Ntuple_2018Data.py
```

**For processing Run 3 MC:**

```
cmsRun l1Ntuple_Run3MC.py
```

**For processing 2022 data:**

```
cmsRun l1Ntuple_2022Data_MB.py
cmsRun l1Ntuple_2022Data_ZB.py
```

### 6. Submit CRAB jobs

Edit crabConfig_2018Data_L1.py, crabConfig_Run3MC_L1.py, or crabConfig_2022Data_L1.py to input your storage area, storage site, dataset, and job name. Then for the following instructions please alter the paths to reflect your own setup.

**For processing 2018 data:**

```
cp L1StudiesFramework/RunPbPbL1Ntuples/crabConfig_2018Data.py CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
crab submit -c crabConfig_2018Data_L1.py
```

**For processing Run 3 MC:**

```
cp L1StudiesFramework/RunPbPbL1Ntuples/crabConfig_Run3MC.py CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
crab submit -c crabConfig_Run3MC_L1.py
```

**For processing 2022 data:**

```
cp L1StudiesFramework/RunPbPbL1Ntuples/crabConfig_2022Data.py CMSSW_13_1_0_pre4/src
cd CMSSW_13_1_0_pre4/src
crab submit -c crabConfig_2022Data_L1.py
```

