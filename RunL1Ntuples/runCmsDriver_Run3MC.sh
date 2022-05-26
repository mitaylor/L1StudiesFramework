#!/bin/bash

globaltag=123X_mcRun3_2021_realistic_v1 # for Run 3 MC
era=Run3_pp_on_PbPb # for running on Run 3
filein='/store/user/mnguyen/Run3MC/QCDPhoton_pThat15_Run3_HydjetEmbedded/QCDPhoton_pThat15_Run3_HydjetEmbedded_DIGI/211126_120712/0000/step2_DIGI_L1_DIGI2RAW_HLT_PU_20.root' # replace with your test MC file
config=L1Ntuple_Run3MC; # cmsRun config file name

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --python_filename=${config}.py \
    -n 1 --no_output --era=${era} --mc --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulMCFromRAWSimHcalTP \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU \
    --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2018_v1_4_1 \
    --customise=L1Trigger/Configuration/customiseUtils.L1TGlobalMenuXML \
    --filein=${filein}

echo '
process.hcalDigis.saveQIE10DataNSamples = cms.untracked.vint32(10) 
process.hcalDigis.saveQIE10DataTags = cms.untracked.vstring( "MYDATA" )
process.HcalTPGCoderULUT.FG_HF_thresholds = cms.vuint32(14, 16)

process.es_prefer_caloparams = cms.ESPrefer("PoolDBESSource","l1conddb")

process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("hcalDigis"),
    minimized = cms.untracked.bool(True)
)

process.HFAdc = cms.Path(process.HFAdcana)
process.schedule.append(process.HFAdc)
' >> ${config}.py