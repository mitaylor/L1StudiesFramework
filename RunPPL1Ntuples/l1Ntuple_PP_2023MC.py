# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: l1Ntuple -s RAW2DIGI --no_exec --python_filename=l1Ntuple_PP_2023MC.py -n 10000 --no_output --era=Run3 --mc --conditions=auto:phase1_2023_realistic --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMU --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParamsHI_2023_v0_3 --filein=/store/group/phys_heavyions/dileptons/junseok/RECO_AOD_MC_MB_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v3/MB_TuneCP5_5p36TeV_ppref_NOTPU-pythia8/RECO_AOD_MC_MB_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v3/230713_004450/0000/step3_RAW2DIGI_L1Reco_RECO_RECOSIM_PAT_1.root --secondfilein=/store/group/phys_heavyions/dileptons/junseok/DIGIRAW_MC_MBPU_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v1/MB_TuneCP5_5p36TeV_ppref_NOTPU-pythia8/DIGIRAW_MC_MBPU_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v1/230712_062651/0000/JME-RunIIAutumn18DR-00003_step1_1.root
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process('RAW2DIGI',Run3)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/group/phys_heavyions/dileptons/junseok/RECO_AOD_MC_MB_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v3/MB_TuneCP5_5p36TeV_ppref_NOTPU-pythia8/RECO_AOD_MC_MB_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v3/230713_004450/0000/step3_RAW2DIGI_L1Reco_RECO_RECOSIM_PAT_1.root'),
    secondaryFileNames = cms.untracked.vstring('/store/group/phys_heavyions/dileptons/junseok/DIGIRAW_MC_MBPU_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v1/MB_TuneCP5_5p36TeV_ppref_NOTPU-pythia8/DIGIRAW_MC_MBPU_forPPRef_CMSSW_13_2_0_pre1_12Jul2023_v1/230712_062651/0000/JME-RunIIAutumn18DR-00003_step1_1.root')
)

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

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('l1Ntuple nevts:10000'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2023_realistic', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.endjob_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAW 

#call to customisation function L1TReEmulFromRAW imported from L1Trigger.Configuration.customiseReEmul
process = L1TReEmulFromRAW(process)

# Automatic addition of the customisation function from L1Trigger.L1TNtuples.customiseL1Ntuple
from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleRAWEMU 

#call to customisation function L1NtupleRAWEMU imported from L1Trigger.L1TNtuples.customiseL1Ntuple
process = L1NtupleRAWEMU(process)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseSettings
from L1Trigger.Configuration.customiseSettings import L1TSettingsToCaloParamsHI_2023_v0_3 

#call to customisation function L1TSettingsToCaloParamsHI_2023_v0_3 imported from L1Trigger.Configuration.customiseSettings
process = L1TSettingsToCaloParamsHI_2023_v0_3(process)

# End of customisation functions


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
