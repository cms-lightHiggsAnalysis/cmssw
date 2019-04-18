import FWCore.ParameterSet.Config as cms
import PhysicsTools.PatAlgos.tools.helpers as configtools

def lowerTauPt(process,postfix='',tauPt=8, jetPt=5):
    #from FWCore.ParameterSet.MassReplace import massSearchReplaceParam ->This line will work when we move to combination, but for the CMSSW version CMSSW_8_0_30, the desired function is located  in PhysicsTools/PatAlgos/python/tools/helpers.py
    from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceParam
    massSearchReplaceParam(getattr(process,'PATTauSequence'+postfix),'minJetPt',14,jetPt)
    getattr(process,'selectedPatTaus'+postfix).cut = cms.string("pt > {} && tauID(\'decayModeFindingNewDMs\')> 0.5".format(tauPt))
    




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
    lowerTauPt(process,'ElectronCleaned')
    #patAlgosToolsTask.add(process.slimmedTausElectronCleaned)
    
    return process
