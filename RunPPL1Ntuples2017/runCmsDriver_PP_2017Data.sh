#!/bin/bash

globaltag=94X_dataRun2_ReReco_EOY17_v6
era=Run2_2017_ppRef
filein='/store/data/Run2017G/ZeroBias/RAW/v1/000/306/709/00000/D625424F-09CB-E711-B045-02163E01A484.root'
config=l1Ntuple_PP_2017Data

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --python_filename=${config}.py \
    -n 100 --no_output --era=${era} --data --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU \
    --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloStage2Params_2017_v1_8_4_ppRef \
    --filein=${filein}