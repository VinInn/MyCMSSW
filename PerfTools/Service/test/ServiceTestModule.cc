#include "PerfTools/Service/interface/ServiceFactory.h"
#include "PluginManager/ModuleDef.h"

namespace perftools{

  namespace serviceTest {
    struct Dummy1{};
    struct Dummy2{};

  }
  

}


DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (perftools::ServiceFactory, 
		    perftools::ServiceMaker<perftools::serviceTestDummy1>, "PerfDummy1");
DEFINE_SEAL_PLUGIN (perftools::ServiceFactory, 
		    perftools::ServiceMaker<perftools::serviceTestDummy2>, "PerfDummy2");
