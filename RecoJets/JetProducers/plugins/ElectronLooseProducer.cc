// -*- C++ -*-
//
// Package:    RecoJets/JetProducers
// Class:      ElectronLooseProducer
// 
/**\class ElectronLooseProducer ElectronLooseProducer.cc RecoJets/JetProducers/plugins/ElectronLooseProducer.cc

 Description: [one line class summary]
Creates a ccollection of electrons passing loose working point of Cut based electron ID.
 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Redwan Habibullah
//         Created:  Fri, 19 Jan 2018 00:24:47 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "PhysicsTools/SelectorUtils/interface/CutApplicatorBase.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "TLorentzVector.h"


#include "TMath.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"

#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"
#include "PhysicsTools/SelectorUtils/interface/CutApplicatorWithEventContentBase.h"

#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

using namespace edm;
using namespace std;





// class declaration
//

class ElectronLooseProducer : public edm::stream::EDProducer<> {
   public:
  explicit ElectronLooseProducer(const edm::ParameterSet&);
  ~ElectronLooseProducer();
  //  std::GsfEleDEtaInSeedCut etacut;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  float dEtaInSeed(reco::GsfElectronCollection::const_iterator ele);
  float GsfEleEInverseMinusPInverse(reco::GsfElectronCollection::const_iterator ele);

  double GsfEleMissingHitsCut(reco::GsfElectronCollection::const_iterator ele);

  double GsfEleEffAreaPFIsoCut(reco::GsfElectronCollection::const_iterator ele,edm::Event& iEvent);
  double GsfEleConversionVetoCut(reco::GsfElectronCollection::const_iterator ele,edm::Event& iEvent);
private:
  virtual void beginStream(edm::StreamID) override;
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endStream() override;
  ///
  // void setConsumes(edm::ConsumesCollector&) override final;
  // void getEventContent(const edm::EventBase&) override final;
  ///
  //bool  _isRelativeIso;
  // Effective area constants
  // EffectiveAreas _effectiveAreas;
  // The rho
  // edm::Handle< double > _rhoHandle;

  // constexpr static char rhoString_     [] = "rho";

  
  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  
  // ----------member data ---------------------------
  float dEtaInSeedCut;
  float GsfEleEInverseMinusPInverseCut;
  float effA;
  float rhos;
  //int countEB;
  //int countEE;
  //int counttotal;
  //Source of electrons destined to pass the loose Working point cut
  edm::EDGetTokenT<reco::GsfElectronCollection> electronSrc_;
  edm::EDGetTokenT<double>rho_;
  edm::EDGetTokenT<reco::ConversionCollection> convs_;
  edm::EDGetTokenT<reco::BeamSpot> thebs_;
  edm::EDGetTokenT<reco::VertexCollection> vtx_;
  edm::EDGetTokenT<reco::GsfTrack> trk_;

  //Collection to election reference vectors?
  //edm::EDGetTokenT<reco::GsfElectronRefVector> RefSrc_
  std::vector<double> Etamin = {0.0000,1.000,1.4790,2.000,2.2000,2.3000,2.4000};
  std::vector<double> Etamax ={1.0000,1.4790,2.0000,2.2000,2.3000,2.4000,5.000};
  std::vector<double> EA = {0.1703,0.1715,0.1213,0.1635,0.1937,0.2393};
  int EBcount= 0;
  int EEcount= 0;
  int Tcount= 0;

  math::XYZPointF p1;
  math::XYZPoint p2;
  double dz;
  double dxy;


  
};

///
//constexpr char ElectronLooseProducer::rhoString_[];
//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
ElectronLooseProducer::ElectronLooseProducer(const edm::ParameterSet& iConfig):
  electronSrc_(consumes<reco::GsfElectronCollection>(iConfig.getParameter<edm::InputTag>("electrons"))),
  rho_(consumes<double>(iConfig.getParameter<edm::InputTag>("Rho"))),
  convs_(consumes<reco::ConversionCollection>(iConfig.getParameter<edm::InputTag>("conv"))),
  thebs_(consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("BM"))),
  vtx_ (consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertex"))),
  trk_(consumes<reco::GsfTrack>(iConfig.getParameter<edm::InputTag>("Tracks")))
  //CutApplicatorWithEventContentBase(iConfig).getParameter<edm::InputTag>

//  RefSrc_(consumes<reco::GsfElectronRefVector>(iConfig.getParameter<edm::InputTag>("RefSrc")))
  // etacut = GsfEleDEtaInSeedCut(iConfig);
  //_isRelativeIso(iConfig.getParameter<bool>("isRelativeIso")),
  //_effectiveAreas( (iConfig.getParameter<edm::FileInPath>("effAreasConfigFile")).fullPath())


{
  //vtx_ (consumes<std::vector<reco::VertexCollection> >(iConfig.getParameter<edm::InputTag>("vertex")));
  //register your products
  // produces<reco::GsfElectronCollection>();
  produces<reco::GsfElectronCollection>( "LooseElectron" );
  //etacut = GsfEleDEtaInSeedCut(iConfig);
    ///
  //edm::InputTag rhoTag = iConfig.getParameter<edm::InputTag>("rho");
  //contentTags_.emplace(rhoString_,rhoTag);

  


  
}


ElectronLooseProducer::~ElectronLooseProducer()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions













float ElectronLooseProducer::dEtaInSeed (reco::GsfElectronCollection::const_iterator ele){
  return ele->superCluster().isNonnull() && ele->superCluster()->seed().isNonnull() ? 
    ele->deltaEtaSuperClusterTrackAtVtx() - ele->superCluster()->eta() + ele->superCluster()->seed()->eta() : std::numeric_limits<float>::max();
}



float ElectronLooseProducer::GsfEleEInverseMinusPInverse (reco::GsfElectronCollection::const_iterator ele)
{
  const float ecal_energy_inverse = 1.0/ele->ecalEnergy();
  const float eSCoverP = ele->eSuperClusterOverP();
 return std::abs(1.0 - eSCoverP)*ecal_energy_inverse;
}
double ElectronLooseProducer::GsfEleMissingHitsCut(reco::GsfElectronCollection::const_iterator ele)
{ 

  constexpr reco::HitPattern::HitCategory missingHitType =
    reco::HitPattern::MISSING_INNER_HITS;
    const unsigned mHits = 
    ele->gsfTrack()->hitPattern().numberOfHits(missingHitType);
  return mHits;
}
///
/*void ElectronLooseProducer::setConsumes(edm::ConsumesCollector& cc) {

  auto rho = cc.consumes<double>(contentTags_[rhoString_]);
  contentTokens_.emplace(rhoString_, rho);
  }*/




/*void ElectronLooseProducer::getEventContent(const edm::EventBase& ev) {  

  ev.getByLabel(contentTags_[rhoString_],_rhoHandle);
}*/





double ElectronLooseProducer::GsfEleEffAreaPFIsoCut(reco::GsfElectronCollection::const_iterator ele,edm::Event& iEvent)
{
  //Compute the combined isolation with effective area correction
  
  
  for(unsigned i=0;i<EA.size();i++)
    {
      if(ele->eta() > Etamin[i] && ele->eta() <Etamax[i])
	{
	  effA=EA[i];
	}
    }
  const reco::GsfElectron::PflowIsolationVariables& pfIso  = ele->pfIsolationVariables();
  const float chad = pfIso.sumChargedHadronPt;
  const float nhad = pfIso.sumNeutralHadronEt;
  const float pho = pfIso.sumPhotonEt;
  // float  eA = effA;
  //float rho = (float)(*_rhoHandle); // std::max likes float arguments
  edm::Handle<double>_rhoHandle;
  iEvent.getByToken(rho_,_rhoHandle);
  rhos = *(_rhoHandle.product());
  float iso = chad + std::max(0.0f, nhad + pho-effA*rhos);
  
  // Divide by pT if the relative isolation is requested
  //if( _isRelativeIso )
  iso /= ele->pt();
  
  // Apply the cut and return the result
  return iso;
  
}
  double ElectronLooseProducer::GsfEleConversionVetoCut(reco::GsfElectronCollection::const_iterator ele ,edm::Event& iEvent)
  {

    edm::Handle<reco::ConversionCollection> convs;
    iEvent.getByToken(convs_,convs);
    edm::Handle<reco::BeamSpot> thebs;
    iEvent.getByToken(thebs_,thebs);
    if(thebs.isValid() && convs.isValid() ) {
      return !ConversionTools::hasMatchedConversion(*ele,convs,
						    thebs->position());
    } else {
      edm::LogWarning("GsfEleConversionVetoCut")
	<< "Couldn't find a necessary collection, returning true!";
      return true;
    }





  }
   
    


    




  //
  
// ------------ method called to produce the data  ------------
void
ElectronLooseProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //using namespace edm;
  //using namespace std;
   using namespace reco;

   //Use the ExampleData to create an ExampleData2 which 
   // is put into the Event
   /*  std::unique_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(std::move(pOut));
      use later   */
   Handle<reco::GsfElectronCollection> electrons;
   iEvent.getByToken(electronSrc_,electrons);
   unique_ptr<reco::GsfElectronCollection> passedelectrons(new reco::GsfElectronCollection);
   Handle<reco::VertexCollection> Vertex;
   iEvent.getByToken(vtx_,Vertex);
   Handle<reco::GsfTrack> trk;
   iEvent.getByToken(trk_,trk);
   //dEtaInSeedCut =abs(dEtaInSeed(iele));
   /*Handle<double>_rhoHandle;
     iEvent.getByToken(rho_,_rhoHandle);
     
     rhos = *(_rhoHandle.product());
     edm::Handle<reco::ConversionCollection> convs;
`     iEvent.getByToken(_convs,convs);
     edm::Handle<reco::BeamSpots> thebs;
   iEvent.getByToken(_thebs,thebs);
   */  
   //cout<< "Before Electron loop" <<endl;     
     
   for(reco::GsfElectronCollection::const_iterator iele = electrons->begin() ; iele !=electrons->end(); ++iele)
     { ++Tcount;
       
       dEtaInSeedCut =abs(dEtaInSeed(iele));
       GsfEleEInverseMinusPInverseCut = GsfEleEInverseMinusPInverse(iele);
       p1 = iele->trackPositionAtVtx();


       for(unsigned j= 0;j< Vertex->size();j++)
	 {
	   p2=(Vertex->at(0)).position();



	 }
       dz = abs(p1.z()-p2.z());
       dxy = sqrt((p1.x()-p2.x())*(p1.x()-p2.x()) + (p1.y()-p2.y())*(p1.y()-p2.y()));
       // p2=Vertex->position();


       if( iele->isEB())
	 {++EBcount;
	   cout << "EBloop" <<endl;
	   
	   if( (iele->full5x5_sigmaIetaIeta()<0.011)  && (iele->hadronicOverEm()<0.289) && (abs(iele->deltaPhiSuperClusterTrackAtVtx()) <0.222) && (GsfEleEInverseMinusPInverseCut < 0.241) && (dEtaInSeedCut < 0.00477) && (GsfEleMissingHitsCut(iele) <= 1 ) && (GsfEleEffAreaPFIsoCut(iele,iEvent) < 0.0944 ) && (GsfEleConversionVetoCut(iele,iEvent) ))
	     {
	       passedelectrons->push_back(*iele);
	       //++EBcount;
	       cout<< "EBPushback" <<endl;
	     }
	   
	 }
       if(iele->isEE())
	 
	 {++EEcount;
	   cout<<" EEloop " << endl;
	   if((iele->full5x5_sigmaIetaIeta()<0.03) && (iele->hadronicOverEm()<0.101) && (abs(iele->deltaPhiSuperClusterTrackAtVtx()) <0.213) && (GsfEleEInverseMinusPInverseCut < 0.14) &&(dEtaInSeedCut <0.00868)&& (GsfEleMissingHitsCut(iele) <= 1 ) && (GsfEleEffAreaPFIsoCut(iele,iEvent) < 0.107 ) && (GsfEleConversionVetoCut(iele,iEvent)))
	     
	     {		   
	       
	       
   passedelectrons->push_back(*iele);
   cout<< "EEPushback" <<endl;
   //++EEcount;
	     }
	   
	 }
       
       
       
       
       
     }
   
	   
	   
	   
	   
	   
	   
	   iEvent.put(move(passedelectrons), "LooseElectron");

	   cout<<"total "<< Tcount << " EBele " << EBcount << " EEele " << EEcount <<endl;
	   
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
ElectronLooseProducer::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
ElectronLooseProducer::endStream() {
}







//void ElectronLooseProducer::GsfEleDEtaInSeedCut::operator()(const reco::GsfElectronPtr& cand) const{  
  



// ------------ method called when starting to processes a run  ------------
/*
void
ElectronLooseProducer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
ElectronLooseProducer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
ElectronLooseProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
ElectronLooseProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ElectronLooseProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ElectronLooseProducer);
