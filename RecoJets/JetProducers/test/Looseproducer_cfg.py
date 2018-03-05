# Import CMS python class definitions such as Process, Source, and EDProducer
import FWCore.ParameterSet.Config as cms



# Set up a process, named RECO in this case
process = cms.Process("USER")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('USER')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    limit = cms.untracked.int32(-1)
    )

#process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


#Include the number of events to run over
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20000) )











# Configure the object that reads the input file
process.source = cms.Source("PoolSource", 
                            fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80Premix/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/110000/001AC973-60E2-E611-B768-001E67586A2F.root','root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80Premix/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/110000/00376B09-8EE2-E611-A6F8-003048F5ADF6.root','root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80Premix/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/110000/005AE20E-25E3-E611-90E1-FA163ECDDD3D.root','root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80Premix/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/110000/00B73B82-8DE2-E611-9BE1-0025907D2212.root','root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80Premix/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/110000/00C690D9-6DE2-E611-8377-0CC47A0093EC.root')
                            )

# Configure an object that produces a new data object
process.Loose = cms.EDProducer("ElectronLooseProducer",
                               vertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
                               Rho = cms.InputTag("fixedGridRhoFastjetAll"),
                               electrons = cms.InputTag("gedGsfElectrons"),
                               conv = cms.InputTag("conversions"),
                               BM = cms.InputTag("offlineBeamSpot"),
                               Tracks = cms.InputTag("electronGsfTracks"),
                               
                               
                               
                              
                               
                               
                               
                               )

# Configure the object that writes an output file
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string("fourth.root")
                               )



# Configure a path and endpath to run the producer and output modules
process.p = cms.Path(process.Loose)
process.ep = cms.EndPath(process.out)
