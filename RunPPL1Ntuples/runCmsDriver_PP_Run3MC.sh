#!/bin/bash

globaltag=auto:phase1_2023_realistic
era=Run3
filein='/store/group/phys_heavyions/dileptons/junseok/RECO_AOD_MC_MB_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v3/MB_TuneCP5_5p36TeV_ppref_NOTPU-pythia8/RECO_AOD_MC_MB_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v3/230713_004450/0000/step3_RAW2DIGI_L1Reco_RECO_RECOSIM_PAT_1.root'
secondfilein='/store/group/phys_heavyions/dileptons/junseok/DIGIRAW_MC_MBPU_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v1/MB_TuneCP5_5p36TeV_ppref_NOTPU-pythia8/DIGIRAW_MC_MBPU_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v1/230712_062651/0000/JME-RunIIAutumn18DR-00003_step1_1.root'
config=l1Ntuple_PP_2023MC

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --python_filename=${config}.py \
    -n 10000 --no_output --era=${era} --mc --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU \
    --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParamsHI_2023_v0_3 \
    --filein=${filein} --secondfilein=${secondfilein}