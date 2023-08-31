#!/bin/bash

globaltag=94X_mc2017_realistic_forppRef5TeV
era=Run2_2017
filein='/store/user/gsfs/Pythia8_MinBias_pp_CUETP8M1_5020GeV/RAW_Pileup_Poisson2_20171027/171027_153932/0000/step2_pp_Pileup_DIGI_L1_DIGI2RAW_HLT_PU_1.root'
config=l1Ntuple_PP_2017MC

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --python_filename=${config}.py \
    -n 100 --no_output --era=${era} --mc --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulMCFromRAW \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU \
    --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloStage2Params_2017_v1_8_4_ppRef \
    --filein=${filein}