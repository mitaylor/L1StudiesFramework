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
config.JobType.psetName = 'L1Ntuple_Data2022_MB.py' # or 'L1Ntuple_Data2022_ZB.py'
config.JobType.maxMemoryMB = 2000
config.JobType.allowUndistributedCMSSW = True

#### Data ####
config.section_('Data')
config.Data.inputDataset = '/HITestRaw1/HIRun2022A-v1/RAW' # your dataset
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.outLFNDirBase = OutputBase
config.Data.outputDatasetTag = Tag
config.Data.allowNonValidInputDataset = True
config.Data.runRange = '362321' # this is a good run, included since lumiMask is not available yet
# config.Data.lumiMask = '' # not available yet

#### Site ####
config.section_('Site')
config.Site.storageSite = '' # your storage site