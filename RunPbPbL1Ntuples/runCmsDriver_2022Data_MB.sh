#!/bin/bash

globaltag=130X_dataRun3_Prompt_v3 # latest GT from https://twiki.cern.ch/twiki/bin/view/CMS/LatestOnlineGTs
era=Run3_pp_on_PbPb # for Run 2
filein='/store/hidata/HIRun2022A/HITestRaw1/RAW/v1/000/362/219/00000/fabf58d4-c2e8-4275-a868-fbc8eb3436b1.root'
config=l1Ntuple_2022Data_MB; # cmsRun config file name

cmsDriver.py l1Ntuple -s RAW2DIGI --no_exec --repacked --python_filename=${config}.py \
    -n 1000 --no_output --era=${era} --data --conditions=${globaltag} \
    --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW \
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

process.Trigger = cms.EDFilter( "TriggerResultsFilter",
      triggerConditions = cms.vstring(
        # "HLT_HIZeroBias_v*",                                                                                                                                                                                  
        "HLT_HIMinimumBias_v*"
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

MassReplaceInputTag(process, new="rawDataMapperByLabel", old="rawDataCollector")
' >> ${config}.py