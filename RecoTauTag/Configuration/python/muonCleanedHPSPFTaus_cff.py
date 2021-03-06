import FWCore.ParameterSet.Config as cms

'''

Sequences for reconstructing muon cleaned taus using the HPS algorithm

'''


# import jet cleaning configuration
from RecoJets.JetProducers.ak4PFJetsMuonCleaned_cfi import *

# create task
muonCleanedHPSPFTausSequence = cms.Sequence(
    recoMuonsForJetCleaning*
    ak4PFJetsMuonCleaned
)
