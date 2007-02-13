#include "PerfTools/FWBinding/interface/PerfTools.h"

#include "PerfTools/Service/interface/ServiceFactory.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"

#include "PerfTools/Sampler/interface/SamplerBuilder.h"


PerfTools::PerfTools(edm::ParameterSet const& pset, edm::ActivityRegistry  & activity) {


}


PerfTools::Payload & PerfTools::get(edm::ParameterSet const& pset) {

  boost::shared_ptr<perftools::SamplerBuilder> creator = 
    perftools::ServiceFactory::get()->getService<perftools::SamplerBuilder>("PerfTools:SamplerBuilder");
 
  std::string const & name = pset.getUntrackedParameter<std::string>("Name");
  std::vector<std::string> const & sources = 
    pset.getUntrackedParameter<std::vector<std::string> >("Sources");

  std::vector<std::string> const & reporters = 
    pset.getUntrackedParameter<std::vector<std::string> >("Reporters");

  boost::shared_ptr<perftools::SamplerBuilder> builder = creator->create(name,sources,reporters);
  m_builder = builder;
  return *builder;
}

DEFINE_FWK_SERVICE (PerfTools);
