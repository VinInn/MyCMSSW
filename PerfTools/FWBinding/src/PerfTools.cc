#include "PerfTools/FWBinding/interface/PerfTools.h"

// #include "PerfTools/Service/interface/ServiceFactory.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"

#include "PerfTools/Sampler/interface/SamplerBuilder.h"


PerfTools::PerfTools(edm::ParameterSet const& pset, edm::ActivityRegistry  & activity) {


}


PerfTools::Payload & PerfTools::get(edm::ParameterSet const& pset) {

  boost::shared_ptr<perftools::SamplerBuilder> creator = 
    perftools::ServiceFactory::get()->getService<perftools::SamplerBuilder>("PerfTools:SamplerBuilder");
 


  boost::shared_ptr<SamplerBuilder> builder = creator->create(name,sources,reporters);
  m_builder = builder;
  return *builder;
}

DEFINE_FWK_SERVICE (PerfTools);
