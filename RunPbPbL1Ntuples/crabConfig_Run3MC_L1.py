OutputBase = "" # your storage area
Tag="" # your job name

from WMCore.Configuration import Configuration
config = Configuration()

#### General ####
config.section_('General')
config.General.requestName = Tag
config.General.transferOutputs = True
config.General.transferLogs = True

#### JobType ####
config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'l1Ntuple_Run3MC.py'
config.JobType.maxMemoryMB = 2000
config.JobType.allowUndistributedCMSSW = True

#### Data ####
config.section_('Data')
config.Data.inputDataset = '/MinBias_Hydjet_Drum5F_5p02TeV/Run3Winter22PbPbNoMixDIGI-122X_mcRun3_2021_realistic_HI_v10-v3/GEN-SIM-DIGI-RAW-HLTDEBUG' # your dataset
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.outLFNDirBase = OutputBase
config.Data.outputDatasetTag = Tag
config.Data.allowNonValidInputDataset = True

#### Site ####
config.section_('Site')
config.Site.storageSite = '' # your storage site