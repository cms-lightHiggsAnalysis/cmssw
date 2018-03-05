import FWCore.ParameterSet.Config as cms
import PhysicsTools.PatAlgos.tools.helpers as configtools

def addElectronCleanedTaus(process):
    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
    from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag

    process.load("RecoTauTag.Configuration.electronCleanedHPSPFTaus_cff")
    #patAlgosToolsTask = configtools.getPatAlgosToolsTask(process)
    #patAlgosToolsTask.add(process.electronCleanedHPSPFTausTask)
    
    
    process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")
    
    
    jetSrc = 'ak4PFJetsElectronCleaned'
    
    process.ptau = cms.Path( process.PFTau )
    process.PATTauSequence = cms.Sequence(process.PFTau+process.makePatTaus+process.selectedPatTaus)
    process.PATTauSequenceElectronCleaned = cloneProcessingSnippet(process,process.PATTauSequence, 'ElectronCleaned')
    process.recoTauAK4PFJets08RegionElectronCleaned.pfCandSrc = cms.InputTag(jetSrc,'particleFlowElectronCleaned')
    process.hpsPFTauChargedIsoPtSumElectronCleaned.particleFlowSrc = cms.InputTag(jetSrc,'particleFlowElectronCleaned')
    massSearchReplaceAnyInputTag(process.PATTauSequenceElectronCleaned,cms.InputTag('ak4PFJets'),cms.InputTag(jetSrc))  
    process.slimmedTausElectronCleaned = process.slimmedTaus.clone(src = cms.InputTag('selectedPatTausElectronCleaned'))
    
    #patAlgosToolsTask.add(process.slimmedTausElectronCleaned)
    
    return process
