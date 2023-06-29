### HiForest Configuration
# Input: miniAOD
# Type: data

import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run3_pp_on_PbPb_cff import Run3_pp_on_PbPb
process = cms.Process('HiForest',Run3_pp_on_PbPb)

###############################################################################

# HiForest info
process.load("HeavyIonsAnalysis.EventAnalysis.HiForestInfo_cfi")
process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 131X, data")

###############################################################################

# Input files
process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
        '/store/hidata/HIRun2022A/HITestRaw0/AOD/PromptReco-v1/000/362/293/00000/397cc4f3-4eb7-41b9-b2d1-baa004c7f61a.root'
    ), 
    secondaryFileNames = cms.untracked.vstring(
        '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/293/00000/3b4858f8-deea-4ffa-8d53-3e2db6f57f9f.root',
        '/store/hidata/HIRun2022A/HITestRaw0/RAW/v1/000/362/293/00000/ac6f9fea-ed6c-41ee-befc-ec218276da8e.root'
        )
)

# Number of events to process, set to -1 to process all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
    )

###############################################################################

# Load Global Tag, geometry, etc.
# process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

# Required for L1
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.RawToDigi_DataMapper_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# Required for PAT
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.StandardSequences.PAT_cff')

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    accelerators = cms.untracked.vstring('*'),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    deleteNonConsumedUnscheduledModules = cms.untracked.bool(True),
    dumpOptions = cms.untracked.bool(False),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(
            allowAnyLabel_=cms.required.untracked.uint32
        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(0)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(0),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

###############################################################################

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '130X_dataRun3_Prompt_v3', '')
process.HiForestInfo.GlobalTagLabel = process.GlobalTag.globaltag

# Set spike killer settings for emulation
process.GlobalTag.toGet.extend = cms.VPSet(
   cms.PSet(record = cms.string('EcalTPGFineGrainStripEERcd'),
            tag = cms.string('EcalTPGFineGrainStrip_7'),
            connect =cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    ),
    cms.PSet(record = cms.string('EcalTPGSpikeRcd'),
        tag = cms.string('EcalTPGSpike_12'),
        connect =cms.string('frontier://FrontierProd/CMS_CONDITIONS')
    )
)

###############################################################################

# PAT stuff
# process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_data_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.skimanalysis_cfi')
# process.load('HeavyIonsAnalysis.EventAnalysis.hltobject_cfi')
# process.load('HeavyIonsAnalysis.EventAnalysis.l1object_cfi')

# from HeavyIonsAnalysis.EventAnalysis.hltobject_cfi import trigger_list_mc
# process.hltobject.triggerNames = trigger_list_mc

process.load('HeavyIonsAnalysis.EventAnalysis.particleFlowAnalyser_cfi')
process.load('HeavyIonsAnalysis.EGMAnalysis.ggHiNtuplizer_cfi')
process.ggHiNtuplizer.doMuons = cms.bool(False)
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load('HeavyIonsAnalysis.JetAnalysis.akCs4PFJetSequence_pponPbPb_data_cff')
process.load("HeavyIonsAnalysis.TrackAnalysis.TrackAnalyzers_cff")

###############################################################################

# Main forest sequence
process.forest = cms.Path(
    process.HiForestInfo + 
    # process.hltanalysis +
    process.trackSequencePbPb +
    process.particleFlowAnalyser +
    process.hiEvtAnalyzer +
    process.ggHiNtuplizer +
    process.akCs4PFJetAnalyzer
    )

# Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllData 

# Call to customisation function miniAOD_customizeAllData imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
process = miniAOD_customizeAllData(process)

# Root output
process.TFileService = cms.Service("TFileService",
    fileName = cms.string("HiForestMiniAOD.root"))

###############################################################################

# Selection of at least a two-track fitted vertex                                                                                                                                                                     
process.primaryVertexFilterHI = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlineSlimmedPrimaryVertices"),
    cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2"), #in miniADO trackSize()==0, however there is no influence.                                                                                      
    filter = cms.bool(True), # otherwise it won't filter the events                                                                                                                                                   
)

process.load("HeavyIonsAnalysis.EventAnalysis.clusterCompatibilityFilter_cfi")

process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
process.pprimaryVertexFilterHI = cms.Path(process.primaryVertexFilterHI)
process.pAna = cms.EndPath(process.skimanalysis)

###############################################################################

process.raw2digi_step = cms.Path(process.RawToDigi)
process.recotowers = cms.Path(
    process.bunchSpacingProducer *
    process.calolocalreco *
    process.hcalGlobalRecoSequence *
    process.caloTowersRec
)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.forest,process.raw2digi_step,process.recotowers,process.endjob_step)
process.schedule.associate(process.patTask)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

###############################################################################

# Emulation
from Configuration.Applications.ConfigBuilder import MassReplaceInputTag

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
# from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAWsimHcalTP 
from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAW

# Call to customisation function L1TReEmulFromRAW imported from L1Trigger.Configuration.customiseReEmul
# process = L1TReEmulFromRAWsimHcalTP(process)
process = L1TReEmulFromRAW(process)

# Automatic addition of the customisation function from L1Trigger.L1TNtuples.customiseL1Ntuple
from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleRAWEMU 

# Call to customisation function L1NtupleRAWEMU imported from L1Trigger.L1TNtuples.customiseL1Ntuple
process = L1NtupleRAWEMU(process)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseSettings
from L1Trigger.Configuration.customiseSettings import L1TSettingsToCaloParamsHI_2023_v0_3 

# Call to customisation function L1TSettingsToCaloParamsHI_2023_v0_3 imported from L1Trigger.Configuration.customiseSettings
process = L1TSettingsToCaloParamsHI_2023_v0_3(process)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseUtils
from L1Trigger.Configuration.customiseUtils import L1TGlobalMenuXML 

# Call to customisation function L1TGlobalMenuXML imported from L1Trigger.Configuration.customiseUtils
process = L1TGlobalMenuXML(process)

###############################################################################

# Addictional customization
# process.HcalTPGCoderULUT.FG_HF_thresholds = cms.vuint32(14, 19)

process.HFAdcana = cms.EDAnalyzer("HFAdcToGeV",
    digiLabel = cms.untracked.InputTag("hcalDigis"),
    minimized = cms.untracked.bool(True),
    fillhf = cms.bool(True)
)

process.HFAdc = cms.Path(process.HFAdcana)
process.schedule.append(process.HFAdc)

# Filter MB or ZB events
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltfilter = hltHighLevel.clone(
    HLTPaths = [
        "HLT_HIZeroBias_v*",                                                                                                                                                                                  
        #"HLT_HIMinimumBias_v*",
    ]
)
process.filterSequence = cms.Sequence(
    process.hltfilter
)

process.superFilterPath = cms.Path(process.filterSequence)
process.skimanalysis.superFilters = cms.vstring("superFilterPath")

for path in process.paths:
    getattr(process, path)._seq = process.filterSequence * getattr(process,path)._seq

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)

# Switch this tag name to what is used for Heavy Ion
MassReplaceInputTag(process, new="rawDataMapperByLabel", old="rawDataCollector")