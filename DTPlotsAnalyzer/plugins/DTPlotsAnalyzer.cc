// -*- C++ -*-
//
// Package:    DTPlots/DTPlotsAnalyzer
// Class:      DTPlotsAnalyzer
//
/**\class DTPlotsAnalyzer DTPlotsAnalyzer.cc DTPlots/DTPlotsAnalyzer/plugins/DTPlotsAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Matthew Citron <mcitron@ucsb.edu> 10/19/2017
//         Created:  Fri, 20 Aug 2021 19:40:29 GMT
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/JetReco/interface/BasicJet.h"
#include "TH2.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

using reco::TrackCollection;

class DTPlotsAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit DTPlotsAnalyzer(const edm::ParameterSet&);
  ~DTPlotsAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

    std::string triggerString_;
    edm::EDGetTokenT<reco::BasicJetCollection> dtClusterInputToken;
    edm::InputTag dtClusterLabel_;
    edm::Service<TFileService> fs;
    TH1D * dtClusterSizeHist;
    TH1D * totalClusterSizeHist;
    edm::EDGetTokenT<edm::TriggerResults> triggerResultsToken;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
DTPlotsAnalyzer::DTPlotsAnalyzer(const edm::ParameterSet& iConfig)
{ 
    dtClusterLabel_ = iConfig.getParameter<edm::InputTag>("dtClusters");
    triggerString_ = iConfig.getParameter<std::string>("triggerString");
    dtClusterInputToken = consumes<std::vector<reco::BasicJet>>(dtClusterLabel_);
    dtClusterSizeHist = fs->make<TH1D>("dtClusterSize","",400,0,400);
    totalClusterSizeHist = fs->make<TH1D>("totalDtClusterSizeAbove20","",400,0,400);
    triggerResultsToken = consumes<edm::TriggerResults>(edm::InputTag("TriggerResults","","HLTX"));
  //now do what ever initialization is needed
}

DTPlotsAnalyzer::~DTPlotsAnalyzer() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called for each event  ------------
void DTPlotsAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
    edm::Handle<reco::BasicJetCollection> dtClusters;
    iEvent.getByToken(dtClusterInputToken, dtClusters);
    Handle<edm::TriggerResults> triggerResults; //our trigger result object
    iEvent.getByToken(triggerResultsToken, triggerResults);
    bool trigger = false;
    int totalClusterSize = 0;
    if (triggerResults.isValid()) {
	const edm::TriggerNames & triggerNames = iEvent.triggerNames(*triggerResults);
	for (unsigned int i = 0; i < triggerResults->size(); ++i) {
	    const auto &trigname = triggerNames.triggerName(i);
	    if (trigname.find(triggerString_ + "_v") != std::string::npos) {
		trigger   = triggerResults->accept(i);
	    }
	}

	if (trigger){
	    for (auto const& c : *dtClusters) {
		dtClusterSizeHist->Fill(c.energy());
		if (c.energy() > 20){
		totalClusterSize += c.energy();
		}
	    }
	    totalClusterSizeHist->Fill(totalClusterSize);
	}
    }
}

// ------------ method called once each job just before starting event loop  ------------
void DTPlotsAnalyzer::beginJob() {
    // please remove this method if not needed
}

// ------------ method called once each job just after ending the event loop  ------------
void DTPlotsAnalyzer::endJob() {
    // please remove this method if not needed
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void DTPlotsAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);

    //Specify that only 'tracks' is allowed
    //To use, remove the default given above and uncomment below
    //ParameterSetDescription desc;
    //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
    //descriptions.addWithDefaultLabel(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DTPlotsAnalyzer);
