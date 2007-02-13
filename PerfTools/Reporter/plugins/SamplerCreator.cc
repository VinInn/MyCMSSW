#include "PerfTools/Service/interface/ServiceFactory.h"
#include "PluginManager/ModuleDef.h"
#include "PerfTools/Reporter/interface/SamplerBuilderImpl.h"
#include <boost/function.hpp>


typedef perftools::ServiceMaker<perftools::SamplerBuilder, perftools::SamplerBuilderImpl> SamplerCreator;


DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (perftools::ServiceFactory, 
		    SamplerCreator,
		    "PerfTools:SamplerBuilder");
