#!/bin/bash

globaltag=auto:phase1_2023_realistic_hi # for Run 3 MC
era=Run3_pp_on_PbPb # for running on Run 3
filein='/store/himc/Run3Winter22PbPbNoMixDIGI/MinBias_Hydjet_Drum5F_5p02TeV/GEN-SIM-DIGI-RAW-HLTDEBUG/122X_mcRun3_2021_realistic_HI_v10-v3/2430000/31013444-6d03-489d-9b18-3e19d5675eec.root' # replace with your test MC file
config=l1Ntuple_Run3MC; # cmsRun config file name

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --python_filename=${config}.py \
    -n 1 --no_output --era=${era} --mc --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulMCFromRAW \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU \
    --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParamsHI_2023_v0_3 \
    --customise=L1Trigger/Configuration/customiseUtils.L1TGlobalMenuXML \
    --filein=${filein}

echo '

process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("hcalDigis"),
    minimized = cms.untracked.bool(True),
    fillhf = cms.bool(False)
)

process.HFAdc = cms.Path(process.HFAdcana)
process.schedule.append(process.HFAdc)
' >> ${config}.py