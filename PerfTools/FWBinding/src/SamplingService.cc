#include "PerfTools/FWBinding/interface/SamplingService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "PerfTools/Service/interface/ServiceFactory.h"
#include "PerfTools/Sampler/interface/SamplerBuilder.h"


#include <algorithm>
#include <limits>


SamplingService::SamplingService(edm::ParameterSet const& pset, 
				 edm::ActivityRegistry  & activity) :
  m_sources(pset.getUntrackedParameter<std::vector<std::string> >("Sources")),
  m_reporters( pset.getUntrackedParameter<std::vector<std::string> >("Reporters")),
  m_creator(perftools::ServiceFactory::get()->getService<perftools::SamplerBuilder>("PerfTools:SamplerBuilder")),
  m_active(false) {
  
 
    activity.watchPreProcessEvent(this,&SamplingService::beginEventI);
    activity.watchPostProcessEvent(this,&SamplingService::endEventI);
    activity.watchPreProcessPath(this,&SamplingService::beginPathI);
    activity.watchPostProcessPath(this,&SamplingService::endPathI);
    activity.watchPreModule(this,&SamplingService::beginModuleI);
    activity.watchPostModule(this,&SamplingService::endModuleI);


}

SamplingService::~SamplingService(){}

bool SamplingService::startSampling(std::string const & name) {
  if (!m_active) return false;
  perftools::Sample & s = m_samples[name];
  if (s.empty()) {
    boost::shared_ptr<perftools::SamplerBuilder> builder = 
    m_creator->create(name,m_sources,m_reporters);  
    s.init(*builder);
  }
  m_samplers.push(s);
  return true;
}
 
bool SamplingService::stopSampling() {
  if (!m_active) return false;
  m_samplers.pop();
  return true;
}

bool SamplingService::forceStop(){
  return true;
}
void SamplingService::dumpStat(){}
  
 
