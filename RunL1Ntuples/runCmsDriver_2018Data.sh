#!/bin/bash

globaltag=124X_dataRun2_v5 # for Run 2 data reprocessing
era=Run2_2018_pp_on_AA # for Run 2
filein='file:/eos/cms/store/group/phys_heavyions/mitaylor/L1EmulatorTestFiles/MinimumBias_Run326295.root' # change to desired test file
config=L1Ntuple_Data2018; # cmsRun config file name

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --repacked --python_filename=${config}.py \
    -n 1000 --no_output --era=${era} --data --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAWsimHcalTP \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU \
    --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2018_v1_4_1 \
    --customise=L1Trigger/Configuration/customiseUtils.L1TGlobalMenuXML \
    --filein=${filein}

echo '
process.hcalDigis.saveQIE10DataNSamples = cms.untracked.vint32(10) 
process.hcalDigis.saveQIE10DataTags = cms.untracked.vstring( "MYDATA" )
process.HcalTPGCoderULUT.FG_HF_thresholds = cms.vuint32(14, 19)

MassReplaceInputTag(process, new="rawDataMapperByLabel", old="rawDataCollector")
' >> ${config}.py