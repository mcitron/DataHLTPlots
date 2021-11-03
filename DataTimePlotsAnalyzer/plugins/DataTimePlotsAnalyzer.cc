// -*- C++ -*-
//
// Package:    DataTimePlots/DataTimePlotsAnalyzer
// Class:      DataTimePlotsAnalyzer
//
/**\class DataTimePlotsAnalyzer DataTimePlotsAnalyzer.cc DataTimePlots/DataTimePlotsAnalyzer/plugins/DataTimePlotsAnalyzer.cc

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
#include "TH2.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

using reco::TrackCollection;

class DataTimePlotsAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit DataTimePlotsAnalyzer(const edm::ParameterSet&);
  ~DataTimePlotsAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

    edm::InputTag jetLabel_;
    edm::InputTag jetTimeLabel_;
    edm::InputTag jetEmLabel_;
    edm::InputTag jetCellsLabel_;
    std::string triggerString_;
    edm::EDGetTokenT<reco::CaloJetCollection> jetInputToken;
    edm::EDGetTokenT<edm::ValueMap<float>> jetTimesInputToken;
    edm::EDGetTokenT<edm::ValueMap<int>> jetCellsInputToken;
    edm::EDGetTokenT<edm::ValueMap<float>> jetEmInputToken;
    edm::Service<TFileService> fs;
    TH1D * timeHist;
    TH1D * timeHistHighHad;
    TH1D * maxTimeHist;
    TH1D * maxTimeHistHighHad;
    TH1D * secondTimeHist;
    TH1D * secondTimeHistHighHad;
    TH2D * timeVsJetPtHist;
    TH2D * timeVsJetEmHist;
    TH2D * timeVsJetCellsHist;
    TH2D * timeVsJetEtaHist;
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
DataTimePlotsAnalyzer::DataTimePlotsAnalyzer(const edm::ParameterSet& iConfig)
{ 
    jetLabel_= iConfig.getParameter<edm::InputTag>("jets");
    jetTimeLabel_= iConfig.getParameter<edm::InputTag>("jetTimes");
    jetCellsLabel_= iConfig.getParameter<edm::InputTag>("jetCells");
    jetEmLabel_= iConfig.getParameter<edm::InputTag>("jetEmEnergy");
    triggerString_ = iConfig.getParameter<std::string>("triggerString");
    jetInputToken = consumes<std::vector<reco::CaloJet>>(jetLabel_);
    jetTimesInputToken = consumes<edm::ValueMap<float>>(jetTimeLabel_);
    jetCellsInputToken = consumes<edm::ValueMap<int>>(jetCellsLabel_);
    jetEmInputToken = consumes<edm::ValueMap<float>>(jetEmLabel_);
    timeHist = fs->make<TH1D>("jetTimeHist","",200,-25,25);
    timeHistHighHad = fs->make<TH1D>("jetTimeHistHighHad","",200,-25,25);
    maxTimeHist = fs->make<TH1D>("maxJetTimeHist","",200,-25,25);
    maxTimeHistHighHad = fs->make<TH1D>("maxJetTimeHistHighHad","",200,-25,25);
    secondTimeHist = fs->make<TH1D>("secondJetTimeHist","",200,-25,25);
    secondTimeHistHighHad = fs->make<TH1D>("secondJetTimeHistHighHad","",200,-25,25);
    timeVsJetPtHist = fs->make<TH2D>("jetTimeVsPtHist","",200,-25,25,200,0,400);
    timeVsJetEtaHist = fs->make<TH2D>("jetTimeVsEtaHist","",200,-25,25,60,-1.5,1.5);
    timeVsJetCellsHist = fs->make<TH2D>("jetTimeVsnCellsHist","",200,-25,25,100,0,100);
    timeVsJetEmHist = fs->make<TH2D>("jetTimeVsEmHist","",200,-25,25,200,0,200);
    triggerResultsToken = consumes<edm::TriggerResults>(edm::InputTag("TriggerResults","","HLTX"));
  //now do what ever initialization is needed
}

DataTimePlotsAnalyzer::~DataTimePlotsAnalyzer() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called for each event  ------------
void DataTimePlotsAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
    edm::Handle<reco::CaloJetCollection> jets;
    iEvent.getByToken(jetInputToken, jets);
    edm::Handle<edm::ValueMap<float>> jetTimes;
    iEvent.getByToken(jetTimesInputToken, jetTimes);
    edm::Handle<edm::ValueMap<int>> jetCells;
    iEvent.getByToken(jetCellsInputToken, jetCells);
    edm::Handle<edm::ValueMap<float>> jetEm;
    iEvent.getByToken(jetEmInputToken, jetEm);
    Handle<edm::TriggerResults> triggerResults; //our trigger result object
    iEvent.getByToken(triggerResultsToken, triggerResults);
    unsigned int njets = 0;
    double maxTime = -25.;
    double maxTimeHad = -25.;
    double secondTime = -25.;
    double secondTimeHad = -25.;
    int ijet = 0;
    bool trigger = false;
    if (triggerResults.isValid()) {
	const edm::TriggerNames & triggerNames = iEvent.triggerNames(*triggerResults);
	for (unsigned int i = 0; i < triggerResults->size(); ++i) {
	    const auto &trigname = triggerNames.triggerName(i);
	    if (trigname.find(triggerString_ + "_v") != std::string::npos) {
		trigger   = triggerResults->accept(i);
	    }
	}
	if (trigger){
	    for (auto const& c : *jets) {
		reco::CaloJetRef calojetref(jets, ijet);
		timeHist->Fill((*jetTimes)[calojetref]);
		if (maxTime < (*jetTimes)[calojetref]) {
		    secondTime = maxTime;
		    maxTime = (*jetTimes)[calojetref];
		}
		else if (secondTime < (*jetTimes)[calojetref]){
		    secondTime = (*jetTimes)[calojetref];
		}
		if (c.energy()*c.energyFractionHadronic() > 20) {
		    timeHistHighHad->Fill((*jetTimes)[calojetref]);
		    if (maxTimeHad < (*jetTimes)[calojetref]) {
			secondTimeHad = maxTimeHad;
			maxTimeHad = (*jetTimes)[calojetref];
		    }
		    else if (secondTimeHad < (*jetTimes)[calojetref]){
			secondTimeHad = (*jetTimes)[calojetref];
		    }
			
		}
		timeVsJetPtHist->Fill((*jetTimes)[calojetref],c.pt());
		timeVsJetEmHist->Fill((*jetTimes)[calojetref],(*jetEm)[calojetref]);
		timeVsJetCellsHist->Fill((*jetTimes)[calojetref],(*jetCells)[calojetref]);
		timeVsJetEtaHist->Fill((*jetTimes)[calojetref],c.eta());
		ijet++;
	    }
	    maxTimeHist->Fill(maxTime);
	    maxTimeHistHighHad->Fill(maxTimeHad);
	    secondTimeHist->Fill(secondTime);
	    secondTimeHistHighHad->Fill(secondTimeHad);
	}
    }
}

// ------------ method called once each job just before starting event loop  ------------
void DataTimePlotsAnalyzer::beginJob() {
    // please remove this method if not needed
}

// ------------ method called once each job just after ending the event loop  ------------
void DataTimePlotsAnalyzer::endJob() {
    // please remove this method if not needed
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void DataTimePlotsAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
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
DEFINE_FWK_MODULE(DataTimePlotsAnalyzer);
