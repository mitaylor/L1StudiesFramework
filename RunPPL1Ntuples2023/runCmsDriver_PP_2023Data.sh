#!/bin/bash

globaltag=132X_dataRun3_v2
era=Run3
filein='/store/data/Run2023F/PPRefZeroBias0/RAW/v1/000/373/710/00000/0030cd7f-4667-4732-b98c-defe04d2064a.root'
config=l1Ntuple_PP_2023Data

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --python_filename=${config}.py \
    -n 100 --no_output --era=${era} --data --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAW \
    --filein=${filein}

echo '
process.Trigger = cms.EDFilter( "TriggerResultsFilter",
      triggerConditions = cms.vstring(                                                                                                                                                                               
        "HLT_PPRefZeroBias_v*" # Replace with your favorite HLT path
         ),
      hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
      l1tResults = cms.InputTag( "gtStage2Digis" ),
      l1tIgnoreMask = cms.bool( False ),
      l1techIgnorePrescales = cms.bool( False ),
      daqPartitions = cms.uint32( 1 ),
      throw = cms.bool( True )
)

for path in process.paths:
    getattr(process,path)._seq = process.Trigger * getattr(process,path)._seq
' >> ${config}.py