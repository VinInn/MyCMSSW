#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PerfTools/FWBinding/interface/PerfTools.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "PerfTools/Sampler/interface/Sample.h"
#include "PerfTools/Sampler/interface/Sampler.h"

#include <iostream>
#include <cmath>

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
    perftools::Sample m_sampler;
    perftools::Sample m_overhead;
    perftools::Sample m_bias;
    int m_lastEvent;
    int m_action;
    int m_evtCount;
  };


  AnalyzerStub::AnalyzerStub(edm::ParameterSet const & parameters) :
    m_sampler(edm::Service<PerfTools>()->get(parameters.getUntrackedParameter<edm::ParameterSet>("Sampler"))),
    m_overhead(edm::Service<PerfTools>()->get(parameters.getUntrackedParameter<edm::ParameterSet>("Overhead"))),
    m_bias(edm::Service<PerfTools>()->get(parameters.getUntrackedParameter<edm::ParameterSet>("Bias"))),
  {
  }


  AnalyzerStub::~AnalyzerStub() {
  }

  void AnalyzerStub::beginJob(const edm::EventSetup&) {
  }
 
  void AnalyzerStub::analyze(const edm::Event&, const edm::EventSetup&) {
    {
      perftools::Sample s(m_sampler);
      static double gcrap=0;
      for (double i=1;i<100000;i++)
	gcrap+=std::log(std::sqrt(i));
    }
    {
      perftools::Sample o(m_overhead);
      perftools::Sample o(m_bias);
    }
  }
  void AnalyzerStub::endJob() {
  }
 


}

//define this as a plug-in
DEFINE_FWK_MODULE(AnalyzerStub);
