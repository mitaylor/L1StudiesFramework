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
config.JobType.psetName = 'L1Ntuple_Run3MC.py'
config.JobType.maxMemoryMB = 2000
config.JobType.allowUndistributedCMSSW = True

#### Data ####
config.section_('Data')
config.Data.inputDataset = '/QCDPhoton_pThat15_Run3_HydjetEmbedded/mnguyen-QCDPhoton_pThat15_Run3_HydjetEmbedded_DIGI-752b0cc9d22f4a7f20eccfe0d5df0682/USER' # your dataset
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.outLFNDirBase = OutputBase
config.Data.outputDatasetTag = Tag
config.Data.allowNonValidInputDataset = True

#### Site ####
config.section_('Site')
config.Site.storageSite = '' # your storage site