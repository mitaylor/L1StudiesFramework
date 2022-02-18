**ReadL1Ntuples** = example for how to read the emulated trigger decisions from L1Ntuples created with the 2018 PbPb menu L1Menu_CollisionsHeavyIons2018_v4_2_0.xml



### Instructions to create L1Ntuples

**1. Set up the emulator**

```
cmsrel CMSSW_12_3_0_pre1
cd CMSSW_12_3_0_pre1/src
cmsenv
git cms-init
git remote add cms-l1t-offline git@github.com:cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline l1t-integration-CMSSW_12_3_0
git cms-merge-topic -u cms-l1t-offline:l1t-integration-v114.0-CMSSW_12_3_0
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data

git cms-checkdeps -A -a

scram b -j 8
```

L1T emulation relevant GlobalTags in CMSSW_12_3_0 are:

* for run2 data reprocessing '123X_dataRun2_v1'
* for run2 mc '123X_mcRun2_asymptotic_v1'
* for run3 mc '123X_mcRun3_2021_realistic_v1'

**2. Add the 2018 PbPb menu to the emulator**

```
git cms-addpkg L1Trigger/L1TCommon
git cms-addpkg L1Trigger/L1TGlobal
mkdir -p L1Trigger/L1TGlobal/data/Luminosity/startup/
cd L1Trigger/L1TGlobal/data/Luminosity/startup/
wget https://raw.githubusercontent.com/cms-l1-dpg/L1Menu2018/master/official/XMLs/L1Menu_CollisionsHeavyIons2018_v4_2_0.xml
cd ../../../../../
scram b -j 8
```

**3. Run cmsDriver.py script**

Create a script to hold the cmsDriver.py command:

```
touch runCmsDriver.py
chmod +x runCmsDriver.py
```

Add the contents of the script:

```
#!/bin/bash

globaltag=123X_dataRun2_v1 # for run2 data reprocessing
era=Run2_2018_pp_on_AA # for running on 2018 HI data
filein='/store/hidata/HIRun2018A/HIMinimumBias2/RAW/v1/000/326/295/00000/13A98415-9E97-224D-BCE6-7BAE24526AF6.root' # change to desired test file
config=L1Ntuple_data2018; # cmsRun config file name

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --repacked --python_filename=${config}.py \
    -n 1000 --no_output --era=${era} --data --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU \
    --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2018_v1_4_1 \
    --customise=L1Trigger/Configuration/customiseUtils.L1TGlobalMenuXML \
    --filein=${filein}

echo '
process.hcalDigis.saveQIE10DataNSamples = cms.untracked.vint32( 10) 
process.hcalDigis.saveQIE10DataTags = cms.untracked.vstring( "MYDATA" )
process.es_prefer_caloparams = cms.ESPrefer("PoolDBESSource","l1conddb")
MassReplaceInputTag(process, new="rawDataMapperByLabel", old="rawDataCollector")
' >> ${config}.py
```

Create the cmsRun config:

```
./runCmsDriver.py
```

**4. Do a local test of cmsRun**

```
cmsRun L1Ntuple_data2018.py
```

