import FWCore.ParameterSet.Config as cms

'''

Sequences for reconstructing electron cleaned taus using the HPS algorithm

'''


# import jet cleaning configuration
from RecoJets.JetProducers.ak4PFJetsElectronCleaned_cfi import *

# create task
electronCleanedHPSPFTausSequence = cms.Sequence(
    recoElectronsForJetCleaning*
    ak4PFJetsElectronCleaned
)
