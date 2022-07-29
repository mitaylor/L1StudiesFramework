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
config.JobType.psetName = 'L1Ntuple_Data2018.py'
config.JobType.maxMemoryMB = 2000
config.JobType.allowUndistributedCMSSW = True

#### Data ####
config.section_('Data')
config.Data.inputDataset = '/HIForward/shuaiy-SkimedZeroBias_HIRun2018A_v1_RAW_run326776-94cbf4add1123a786b867f646bf0f0da/USER' # your dataset
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.outLFNDirBase = OutputBase
config.Data.outputDatasetTag = Tag
config.Data.allowNonValidInputDataset = True
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/HI/PromptReco/Cert_326381-327564_HI_PromptReco_Collisions18_JSON.txt'

#### Site ####
config.section_('Site')
config.Site.storageSite = '' # your storage site