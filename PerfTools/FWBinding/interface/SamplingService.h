#ifndef SamplingService_H
#define SamplingService_H


//FIXME only forward declarations???
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "DataFormats/Common/interface/ModuleDescription.h"
// #include "DataFormats/Provenance/interface/ModuleDescription.h"

#include "PerfTools/Sampler/interface/Sample.h"
#include "PerfTools/Sampler/interface/Sampler.h"
#include "PerfTools/Sampler/interface/SamplerBuilder.h"

#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>
#include<stack>
//FIXME later hash-map
#include<map> 


/* A Service to start and stop calgrind profiling on demand...
 * act also as profiler watchdog 
 * (in the same service to avoid dependency between service)
 */
class SamplingService {
public:
  SamplingService(edm::ParameterSet const& pset, 
		  edm::ActivityRegistry  & activity);
  
  ~SamplingService();
  
  bool startSampling(std::string const & name);
  bool stopSampling();
  bool forceStop();
  void dumpStat();
  
  void beginEventI(const edm::EventID&, const edm::Timestamp&) {
    static const std::string myName("Full Event");
    startSampling(myName);
  }
  
  void endEventI(const edm::Event&, const edm::EventSetup&) {
    stopSampling();
  }
  
  void beginPathI(std::string const & path) {
    startSampling(path);
  }
  
  void endPathI(std::string const &,  const edm::HLTPathStatus&) {
    stopSampling();
  }
  
  void beginModuleI(const edm::ModuleDescription& md) {
    startSampling(md.moduleLabel());
  }
  
  void endModuleI(const edm::ModuleDescription&) {
    stopSampling();
  }
  
  
  
private:
  
  // configurable
  std::vector<std::string> m_sources;
  std::vector<std::string> m_reporters;
  
  
  // internal state
  std::map<std::string, perftools::Sample> m_samples;
  std::stack<perftools::Sampler> m_samplers;
  boost::shared_ptr<perftools::SamplerBuilder> m_creator;
  
}; 

#endif // SamplingService_H
