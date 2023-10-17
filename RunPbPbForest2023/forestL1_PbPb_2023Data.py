### HiForest Configuration
# Collisions: PbPb
# Type: data
# Input: miniAOD

from Configuration.Applications.ConfigBuilder import MassReplaceInputTag
import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run3_pp_on_PbPb_2023_cff import Run3_pp_on_PbPb_2023
process = cms.Process('HiForest',Run3_pp_on_PbPb_2023)

#####################################################################################
# HiForest labeling info
#####################################################################################

process.load("HeavyIonsAnalysis.EventAnalysis.HiForestInfo_cfi")
process.HiForestInfo.info = cms.vstring("HiForest, miniAOD, 132X, data")

#####################################################################################
# Input source
#####################################################################################

process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/MINIAOD/PromptReco-v2/000/374/668/00000/06179488-b7e6-44f6-bec9-eb242a290ffd.root'
    ),
    secondaryFileNames = cms.untracked.vstring(
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/2c2a7732-f798-4476-a5ad-4cad128814bc.root',
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/319cfb86-b372-4bca-bdb1-10cab9e8f35c.root',
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/31f0c344-fc8c-4caa-b626-e554f33922e7.root',
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/55301665-73c4-4705-aacc-057c418b018c.root',
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/644068c1-6704-4541-b418-d5bf9386821f.root',
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/6471fc98-d2dd-44c8-949b-2b784f0a9d6b.root',
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/92f8ee2d-6568-41da-9d3c-801932b2b21e.root',
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/a413413e-8846-4ce3-8942-77b64a0d70b4.root',
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/b7ad0780-4ab4-465d-a798-04305d937414.root',
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/d8367394-0671-4900-bbc3-ab9f8c1b8286.root',
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/f4223860-5628-452b-b5cb-77821e9ce393.root',
        '/store/hidata/HIRun2023A/HIPhysicsRawPrime0/RAW/v1/000/374/668/00000/fdcebb4b-ea2c-4402-9480-ab964605f1e7.root'
    ),
    dropDescendantsOfDroppedBranches=cms.untracked.bool(False),
    inputCommands = cms.untracked.vstring(
        'keep *',
        'drop *_gtStage2Digis_*_RECO',
    )
)

# Number of events we want to process, -1 = all events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

#####################################################################################
# Load Global Tag, geometry, etc.
#####################################################################################

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.RawToDigi_DataMapper_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

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
    holdsReferencesToDeleteEarly = cms.untracked.VPSet(),
    makeTriggerResults = cms.obsolete.untracked.bool,
    modulesToIgnoreForDeleteEarly = cms.untracked.vstring(),
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(0),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '132X_dataRun3_Prompt_v4', '')
process.HiForestInfo.GlobalTagLabel = process.GlobalTag.globaltag

# process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
# process.GlobalTag.toGet.extend([
#     cms.PSet(record = cms.string("HeavyIonRcd"),
#         tag = cms.string("CentralityTable_HFtowers200_DataPbPb_periHYDJETshape_run3v1302x04_offline_374289"),
#         connect = cms.string("sqlite_file:CentralityTable_HFtowers200_DataPbPb_periHYDJETshape_run3v1302x04_offline_374289.db"),
#         label = cms.untracked.string("HFtowers")
#     ),
# ])

process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")

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

#####################################################################################
# Define tree output
#####################################################################################

# process.TFileService = cms.Service("TFileService",
#     fileName = cms.string("HiForestMiniAOD.root"))

# process.output = cms.OutputModule(
#     "PoolOutputModule",
#     fileName = cms.untracked.string('HiForestEDM.root'),
#     outputCommands = cms.untracked.vstring(
#         'keep *',
#     )
# )

# process.output_path = cms.EndPath(process.output)

#########################
# Event analysis
#########################

process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_data_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.skimanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hltobject_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.l1object_cfi')

# process.hiEvtAnalyzer.doCentrality = cms.bool(False)
# process.hiEvtAnalyzer.doHFfilters = cms.bool(False)

from HeavyIonsAnalysis.EventAnalysis.hltobject_cfi import trigger_list_data_2023_skimmed
process.hltobject.triggerNames = trigger_list_data_2023_skimmed

process.load('HeavyIonsAnalysis.EventAnalysis.particleFlowAnalyser_cfi')

#########################
# Photons, electrons, and muons
#########################

process.load('HeavyIonsAnalysis.EGMAnalysis.ggHiNtuplizer_cfi')
process.ggHiNtuplizer.doMuons = cms.bool(False)
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

process.load("HeavyIonsAnalysis.MuonAnalysis.unpackedMuons_cfi")
process.load("HeavyIonsAnalysis.MuonAnalysis.muonAnalyzer_cfi")

#########################
# Jets
#########################

process.load('HeavyIonsAnalysis.JetAnalysis.akCs4PFJetSequence_pponPbPb_data_cff')
process.load('HeavyIonsAnalysis.JetAnalysis.akPu4CaloJetSequence_pponPbPb_data_cff')
process.akPu4CaloJetAnalyzer.doHiJetID = True

#########################
# Tracks
#########################

process.load("HeavyIonsAnalysis.TrackAnalysis.TrackAnalyzers_cff")

#########################
# ZDC
#########################

process.load('HeavyIonsAnalysis.ZDCAnalysis.QWZDC2018Producer_cfi')
process.load('HeavyIonsAnalysis.ZDCAnalysis.QWZDC2018RecHit_cfi')
process.load('HeavyIonsAnalysis.ZDCAnalysis.zdcanalyzer_cfi')

process.zdcdigi.SOI = cms.untracked.int32(2)
process.zdcanalyzer.doZDCRecHit = False
process.zdcanalyzer.doZDCDigi = True
process.zdcanalyzer.zdcRecHitSrc = cms.InputTag("QWzdcreco")
process.zdcanalyzer.zdcDigiSrc = cms.InputTag("hcalDigis", "ZDC")
process.zdcanalyzer.calZDCDigi = False
process.zdcanalyzer.verbose = False
process.zdcanalyzer.nZdcTs = cms.int32(6)

from CondCore.CondDB.CondDB_cfi import *

process.es_pool = cms.ESSource("PoolDBESSource",
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string("HcalElectronicsMapRcd"),
            tag = cms.string("HcalElectronicsMap_2021_v2.0_data")
        )
    ),
    connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
    authenticationMethod = cms.untracked.uint32(1)
)
process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')
process.es_ascii = cms.ESSource('HcalTextCalibrations',
    input = cms.VPSet(
        cms.PSet(
            object = cms.string('ElectronicsMap'),
            file = cms.FileInPath("L1Trigger/L1TZDC/data/emap_2023_newZDC_v3.txt")
        )
    )
)

#########################
# Main analysis list
#########################

process.forest = cms.Path(
    process.HiForestInfo +
    process.centralityBin +
    process.hiEvtAnalyzer +
    process.hltanalysis +
    process.hltobject +
    process.l1object +
    process.trackSequencePbPb +
    # process.particleFlowAnalyser +
    process.ggHiNtuplizer +
    # process.zdcdigi +
    # process.QWzdcreco +
    process.zdcanalyzer +
    process.unpackedMuons +
    process.muonAnalyzer +
    process.akPu4CaloJetAnalyzer
)

#########################
# Customization
#########################

addR3Jets = False
addR3FlowJets = False
addR4Jets = True
addR4FlowJets = True
addUnsubtractedR4Jets = True

# Choose which additional information is added to jet trees
doHIJetID = True             # Fill jet ID and composition information branches
doWTARecluster = True        # Add jet phi and eta for WTA axis

# This is only for non-reclustered jets
addCandidateTagging = False

if addR3Jets or addR3FlowJets or addR4Jets or addR4FlowJets or addUnsubtractedR4Jets :
    process.load("HeavyIonsAnalysis.JetAnalysis.extraJets_cff")
    from HeavyIonsAnalysis.JetAnalysis.clusterJetsFromMiniAOD_cff import setupHeavyIonJets
    process.load("HeavyIonsAnalysis.JetAnalysis.candidateBtaggingMiniAOD_cff")

    if addR3Jets :
        process.jetsR3 = cms.Sequence()
        setupHeavyIonJets('akCs3PF', process.jetsR3, process, isMC = 0, radius = 0.30, JECTag = 'AK3PF', doFlow = False)
        process.akCs3PFpatJetCorrFactors.levels = ['L2Relative', 'L2L3Residual']
        process.akCs3PFJetAnalyzer = process.akCs4PFJetAnalyzer.clone(jetTag = "akCs3PFpatJets", jetName = 'akCs3PF', doHiJetID = doHIJetID, doWTARecluster = doWTARecluster)
        process.forest += process.extraJetsData * process.jetsR3 * process.akCs3PFJetAnalyzer

    if addR3FlowJets :
        process.jetsR3flow = cms.Sequence()
        setupHeavyIonJets('akCs3PFFlow', process.jetsR3flow, process, isMC = 0, radius = 0.30, JECTag = 'AK3PF', doFlow = True)
        process.akCs3PFFlowpatJetCorrFactors.levels = ['L2Relative', 'L2L3Residual']
        process.akFlowPuCs3PFJetAnalyzer = process.akCs4PFJetAnalyzer.clone(jetTag = "akCs3PFFlowpatJets", jetName = 'akCs3PFFlow', doHiJetID = doHIJetID, doWTARecluster = doWTARecluster)
        process.forest += process.extraFlowJetsData * process.jetsR3flow * process.akFlowPuCs3PFJetAnalyzer

    if addR4Jets :
        # Recluster using an alias "0" in order not to get mixed up with the default AK4 collections
        process.jetsR4 = cms.Sequence()
        setupHeavyIonJets('akCs0PF', process.jetsR4, process, isMC = 0, radius = 0.40, JECTag = 'AK4PF', doFlow = False)
        process.akCs0PFpatJetCorrFactors.levels = ['L2Relative', 'L2L3Residual']
        process.akCs4PFJetAnalyzer.jetTag = 'akCs0PFpatJets'
        process.akCs4PFJetAnalyzer.jetName = 'akCs0PF'
        process.akCs4PFJetAnalyzer.doHiJetID = doHIJetID
        process.akCs4PFJetAnalyzer.doWTARecluster = doWTARecluster
        process.forest += process.extraJetsData * process.jetsR4 * process.akCs4PFJetAnalyzer

    if addR4FlowJets :
        process.jetsR4flow = cms.Sequence()
        setupHeavyIonJets('akCs4PFFlow', process.jetsR4flow, process, isMC = 0, radius = 0.40, JECTag = 'AK4PF', doFlow = True)
        process.akCs4PFFlowpatJetCorrFactors.levels = ['L2Relative', 'L2L3Residual']
        process.akFlowPuCs4PFJetAnalyzer.jetTag = 'akCs4PFFlowpatJets'
        process.akFlowPuCs4PFJetAnalyzer.jetName = 'akCs4PFFlow'
        process.akFlowPuCs4PFJetAnalyzer.doHiJetID = doHIJetID
        process.akFlowPuCs4PFJetAnalyzer.doWTARecluster = doWTARecluster
        process.forest += process.extraFlowJetsData * process.jetsR4flow * process.akFlowPuCs4PFJetAnalyzer

    if addUnsubtractedR4Jets:
        process.load('HeavyIonsAnalysis.JetAnalysis.ak4PFJetSequence_ppref_data_cff')
        from HeavyIonsAnalysis.JetAnalysis.clusterJetsFromMiniAOD_cff import setupPprefJets
        process.unsubtractedJetR4 = cms.Sequence()
        setupPprefJets('ak04PF', process.unsubtractedJetR4, process, isMC = 0, radius = 0.40, JECTag = 'AK4PF')
        process.ak04PFpatJetCorrFactors.levels = ['L2Relative', 'L2L3Residual']
        process.ak4PFJetAnalyzer.jetTag = "ak04PFpatJets"
        process.ak4PFJetAnalyzer.jetName = "ak04PF"
        process.forest += process.extraJetsData * process.unsubtractedJetR4 * process.ak4PFJetAnalyzer


if addCandidateTagging:
    process.load("HeavyIonsAnalysis.JetAnalysis.candidateBtaggingMiniAOD_cff")

    from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
    updateJetCollection(
        process,
        jetSource = cms.InputTag('slimmedJets'),
        jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
        btagDiscriminators = ['pfCombinedSecondaryVertexV2BJetTags', 'pfDeepCSVDiscriminatorsJetTags:BvsAll', 'pfDeepCSVDiscriminatorsJetTags:CvsB', 'pfDeepCSVDiscriminatorsJetTags:CvsL'], ## to add discriminators,
        btagPrefix = 'TEST',
    )

    process.updatedPatJets.addJetCorrFactors = False
    process.updatedPatJets.discriminatorSources = cms.VInputTag(
        cms.InputTag('pfDeepCSVJetTags:probb'),
        cms.InputTag('pfDeepCSVJetTags:probc'),
        cms.InputTag('pfDeepCSVJetTags:probudsg'),
        cms.InputTag('pfDeepCSVJetTags:probbb'),
    )

    process.akCs4PFJetAnalyzer.jetTag = "updatedPatJets"

    process.forest.insert(1,process.candidateBtagging*process.updatedPatJets)

#########################
# Event selection
#########################

process.load('HeavyIonsAnalysis.EventAnalysis.collisionEventSelection_cff')
process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)

process.load('HeavyIonsAnalysis.EventAnalysis.hffilter_cfi')
process.pphfCoincFilter4Th2 = cms.Path(process.phfCoincFilter4Th2)
process.pphfCoincFilter1Th3 = cms.Path(process.phfCoincFilter1Th3)
process.pphfCoincFilter2Th3 = cms.Path(process.phfCoincFilter2Th3)
process.pphfCoincFilter3Th3 = cms.Path(process.phfCoincFilter3Th3)
process.pphfCoincFilter4Th3 = cms.Path(process.phfCoincFilter4Th3)
process.pphfCoincFilter5Th3 = cms.Path(process.phfCoincFilter5Th3)
process.pphfCoincFilter1Th4 = cms.Path(process.phfCoincFilter1Th4)
process.pphfCoincFilter2Th4 = cms.Path(process.phfCoincFilter2Th4)
process.pphfCoincFilter3Th4 = cms.Path(process.phfCoincFilter3Th4)
process.pphfCoincFilter4Th4 = cms.Path(process.phfCoincFilter4Th4)
process.pphfCoincFilter5Th4 = cms.Path(process.phfCoincFilter5Th4)
process.pphfCoincFilter1Th5 = cms.Path(process.phfCoincFilter1Th5)
process.pphfCoincFilter2Th5 = cms.Path(process.phfCoincFilter2Th5)
process.pphfCoincFilter3Th5 = cms.Path(process.phfCoincFilter3Th5)
process.pphfCoincFilter4Th5 = cms.Path(process.phfCoincFilter4Th5)
process.pphfCoincFilter5Th5 = cms.Path(process.phfCoincFilter5Th5)
process.pphfCoincFilter1Th6 = cms.Path(process.phfCoincFilter1Th6)
process.pphfCoincFilter2Th6 = cms.Path(process.phfCoincFilter2Th6)
process.pphfCoincFilter3Th6 = cms.Path(process.phfCoincFilter3Th6)
process.pphfCoincFilter4Th6 = cms.Path(process.phfCoincFilter4Th6)
process.pphfCoincFilter5Th6 = cms.Path(process.phfCoincFilter5Th6)
process.pAna = cms.EndPath(process.skimanalysis)

process.Trigger = cms.EDFilter("TriggerResultsFilter",
    triggerConditions = cms.vstring(                                                                                                                                                                               
        'HLT_HIZeroBias_HighRate_*',           # Example HLT path
        # 'HLT_HIMinimumBiasHF1ANDZDC1nOR_*',
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

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.forest,process.raw2digi_step,process.endjob_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

#####################################################################################
# L1 emulation
#####################################################################################

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAW

# Call to customisation function L1TReEmulFromRAW imported from L1Trigger.Configuration.customiseReEmul
process = L1TReEmulFromRAW(process)

# Automatic addition of the customisation function from L1Trigger.L1TNtuples.customiseL1Ntuple
from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleRAWEMU 

# Call to customisation function L1NtupleRAWEMU imported from L1Trigger.L1TNtuples.customiseL1Ntuple
process = L1NtupleRAWEMU(process)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseSettings
from L1Trigger.Configuration.customiseSettings import L1TSettingsToCaloParamsHI_2023_v0_4_2

# Call to customisation function L1TSettingsToCaloParamsHI_2023_v0_4_2 imported from L1Trigger.Configuration.customiseSettings
process = L1TSettingsToCaloParamsHI_2023_v0_4_2(process)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseUtils
from L1Trigger.Configuration.customiseUtils import L1TGlobalMenuXML 

# Call to customisation function L1TGlobalMenuXML imported from L1Trigger.Configuration.customiseUtils
process = L1TGlobalMenuXML(process)

#########################
# Customization
#########################

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)

process.l1UpgradeTree.sumZDCTag = cms.untracked.InputTag("etSumZdcProducer")
process.l1UpgradeEmuTree.sumZDCTag = cms.untracked.InputTag("etSumZdcProducer")

process.etSumZdcProducer = cms.EDProducer('L1TZDCProducer',
  zdcDigis = cms.InputTag("hcalDigis", "ZDC")
)

process.etSumZdc = cms.Path(process.etSumZdcProducer)
process.schedule.append(process.etSumZdc)

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