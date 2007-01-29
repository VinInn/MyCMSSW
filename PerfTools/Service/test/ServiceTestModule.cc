#include "PerfTools/Service/interface/ServiceFactory.h"
#include "PluginManager/ModuleDef.h"

namespace perftools{

  namespace serviceTest {
    struct Dummy1{};
    struct Dummy{};
    struct Dummy2 : public Dummy{};

  }
  

}

typedef perftools::ServiceMaker<perftools::serviceTest::Dummy, 
				perftools::serviceTest::Dummy2> Dummy2Service;

DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (perftools::ServiceFactory, 
		    perftools::ServiceMaker<perftools::serviceTest::Dummy1>, "PerfDummy1");
DEFINE_SEAL_PLUGIN (perftools::ServiceFactory, Dummy2Service, "PerfDummy2");
