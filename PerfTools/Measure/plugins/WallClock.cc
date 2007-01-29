#include "PerfTools/Service/interface/ServiceFactory.h"
#include "PluginManager/ModuleDef.h"
#include "PerfTools/Measure/interface/RealTime.h"
#include <boost/function.hpp>

namespace {

  struct WallClock : public boost::function<long long(void)> {
    WallClock() : boost::function<long long(void)>(perftools::realTime){} 
  };


}

typedef perftools::ServiceMaker<boost::function<long long(void)>, WallClock> WallClockService;

DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (perftools::ServiceFactory, 
		    WallClockService,
		    "PerfTools:WallClock");
