#include "PerfTools/FWBinding/interface/PerfTools.h"

// #include "PerfTools/Service/interface/ServiceFactory.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"


PerfTools::PerfTools(edm::ParameterSet const& pset, edm::ActivityRegistry  & activity) {


}


PerfTools::Handle PerfTools::get(edm::ParameterSet const& pset) {
  return Handle();
}

DEFINE_FWK_SERVICE (PerfTools);
