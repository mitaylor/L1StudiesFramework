OutputBase = '' # your storage area
Tag = '' # your job name

import CRABClient
from CRABAPI.RawCommand import crabCommand
from CRABClient.UserUtilities import config
config = config()

#### General ####
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True

#### JobType ####
config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'forestL1_miniAOD_2022Data_MB.py' # or use 'forestL1_miniAOD_2022Data_ZB.py'
config.JobType.maxMemoryMB = 2000
config.JobType.allowUndistributedCMSSW = True

#### Data ####
config.section_('Data')
config.Data.inputDBS = 'global'
config.Data.splitting = 'Automatic'
config.Data.publication = False
config.Data.outputDatasetTag = Tag
config.Data.allowNonValidInputDataset = True
config.Data.useParent = True # uses the parent RAW dataset as secondary source
config.Data.lumiMask = '/eos/user/c/cmsdqm/www/CAF/certification/Collisions22/Collisions2022HISpecial/Cert_Collisions2022HISpecial_362293_362323_Golden.json'

#### Site ####
config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'
config.Site.whitelist = ['T2_US_Vanderbilt']

def submit(config):
    crabCommand('submit', config = config, dryrun=False)

dataMap = {
        "HITestRaw0": { "mDataset": "/HITestRaw0/HIRun2022A-PromptReco-v1/AOD"}
        # "HITestRaw1": { "mDataset": "/HITestRaw1/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw2": { "mDataset": "/HITestRaw2/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw3": { "mDataset": "/HITestRaw3/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw4": { "mDataset": "/HITestRaw4/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw5": { "mDataset": "/HITestRaw5/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw6": { "mDataset": "/HITestRaw6/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw7": { "mDataset": "/HITestRaw7/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw8": { "mDataset": "/HITestRaw8/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw9": { "mDataset": "/HITestRaw9/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw10": { "mDataset": "/HITestRaw10/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw11": { "mDataset": "/HITestRaw11/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw12": { "mDataset": "/HITestRaw12/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw13": { "mDataset": "/HITestRaw13/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw14": { "mDataset": "/HITestRaw14/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw15": { "mDataset": "/HITestRaw15/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw16": { "mDataset": "/HITestRaw16/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw17": { "mDataset": "/HITestRaw17/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw18": { "mDataset": "/HITestRaw18/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw19": { "mDataset": "/HITestRaw19/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw20": { "mDataset": "/HITestRaw20/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw21": { "mDataset": "/HITestRaw21/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw22": { "mDataset": "/HITestRaw22/HIRun2022A-PromptReco-v1/AOD"},
        # "HITestRaw23": { "mDataset": "/HITestRaw23/HIRun2022A-PromptReco-v1/AOD"}
        }

## Submit job for different datasets 
for key, val in dataMap.items():
    config.General.requestName = key + "_" + Tag
    config.Data.inputDataset = val['mDataset']
    config.Data.outLFNDirBase = OutputBase + '%s' %  config.General.requestName
    print("Submitting CRAB job for: " + val["mDataset"] + ", " + key)
    submit(config)