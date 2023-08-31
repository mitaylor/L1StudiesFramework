#!/bin/bash

globaltag=132X_mcRun3_2023_realistic_v2 # update ASAP with new tag, this one has muon bug
era=Run3
filein='/store/group/phys_heavyions/dileptons/junseok/DIGIRAW_MC_MBPU_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v1/MB_TuneCP5_5p36TeV_ppref_NOTPU-pythia8/DIGIRAW_MC_MBPU_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v1/230712_062651/0000/JME-RunIIAutumn18DR-00003_step1_1.root'
config=l1Ntuple_PP_2023MC

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --python_filename=${config}.py \
    -n 100 --no_output --era=${era} --mc --conditions=${globaltag} \
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