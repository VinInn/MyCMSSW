#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"



#include <iostream>

namespace {
  class AnalyzerStub : public edm::EDAnalyzer {
  public:
    explicit  AnalyzerStub(edm::ParameterSet const & parameters);
    ~AnalyzerStub();
    
    
  private:
    virtual void beginJob(edm::EventSetup const&) ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    
    // ----------member data ---------------------------
    int m_firstEvent;
    int m_lastEvent;
    int m_action;
    int m_evtCount;
  };


  AnalyzerStub::AnalyzerStub(edm::ParameterSet const & parameters) {
    edm::ParameterSet const & ps = 
      parameters.getParameter<edm::ParameterSet>("Sampler");
    std::cout << "Sampler Quantities " 
	      << ps.getParameter<std::vector<std::string> >("Quantities").size()
	      << std::endl;
  }


  AnalyzerStub::~AnalyzerStub() {
  }

  void AnalyzerStub::beginJob(const edm::EventSetup&) {
  }
 
  void AnalyzerStub::analyze(const edm::Event&, const edm::EventSetup&) {
  }

  void AnalyzerStub::endJob() {
  }
 


}

//define this as a plug-in
DEFINE_FWK_MODULE(AnalyzerStub);
