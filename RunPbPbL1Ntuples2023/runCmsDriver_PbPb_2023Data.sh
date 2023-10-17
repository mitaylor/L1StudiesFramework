#!/bin/bash

globaltag=132X_dataRun3_Prompt_v4
era=Run3_pp_on_PbPb_2023
filein='/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/2c2a7732-f798-4476-a5ad-4cad128814bc.root'
config=l1Ntuple_PbPb_2023Data

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --repacked --python_filename=${config}.py \
    -n 100 --no_output --era=${era} --data --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW \
    --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU \
    --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParamsHI_2023_v0_4_2 \
    --customise=L1Trigger/Configuration/customiseUtils.L1TGlobalMenuXML \
    --filein=${filein}

echo '
process.l1UpgradeTree.sumZDCTag = cms.untracked.InputTag("etSumZdcProducer")
process.l1UpgradeEmuTree.sumZDCTag = cms.untracked.InputTag("etSumZdcProducer")

process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("hcalDigis"),
    minimized = cms.untracked.bool(True),
    fillhf = cms.bool(False)
)

process.HFAdc = cms.Path(process.HFAdcana)
process.schedule.append(process.HFAdc)

process.hcalDigis.saveQIE10DataNSamples = cms.untracked.vint32(6) 
process.hcalDigis.saveQIE10DataTags = cms.untracked.vstring( "MYDATA" )

process.Trigger = cms.EDFilter( "TriggerResultsFilter",
      triggerConditions = cms.vstring(                                                                                                                                                                               
        "HLT_HIZeroBias_HighRate_v*" # Replace with your favorite HLT path
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

MassReplaceInputTag(process, new="rawPrimeDataRepacker", old="rawDataCollector")
' >> ${config}.py