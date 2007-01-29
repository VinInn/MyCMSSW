#include "PerfTools/Service/interface/ServiceFactory.h"
#include "PluginManager/ModuleDef.h"
#include <ctime>
#include <boost/function.hpp>

namespace {

  struct CPUClock : public boost::function<long long(void)> {
    CPUClock() : boost::function<long long(void)>(std::clock){} 
  };


}

typedef perftools::ServiceMaker<boost::function<long long(void)>, CPUClock> CPUClockService;

DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (perftools::ServiceFactory, 
		    CPUClockService,
		    "PerfTools:CPUClock");


