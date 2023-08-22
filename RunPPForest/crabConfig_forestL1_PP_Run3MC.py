OutputBase = '' # your storage area
Tag = '' # your job name

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
config.JobType.psetName = 'forestL1_PP_Run3MC.py'
config.JobType.maxMemoryMB = 2000
config.JobType.allowUndistributedCMSSW = True

#### Data ####
config.section_('Data')
config.Data.inputDataset = '' # your dataset
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.outLFNDirBase = OutputBase
config.Data.outputDatasetTag = Tag
config.Data.useParent = True # uses the parent RAW dataset as secondary source
config.Data.allowNonValidInputDataset = True

#### Site ####
config.section_('Site')
config.Site.storageSite = '' # your storage site