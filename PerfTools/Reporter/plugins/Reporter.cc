#include "PerfTools/Service/interface/ServiceFactory.h"
#include "PluginManager/ModuleDef.h"
#include "PerfTools/Reporter/interface/Reporter.h"
#include <boost/function.hpp>


DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (perftools::ServiceFactory, 
		    perftools::ServiceMaker<perftools::Reporter>
		    "PerfTools:Reporter");
