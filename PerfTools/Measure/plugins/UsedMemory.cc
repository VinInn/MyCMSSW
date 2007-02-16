#include "PerfTools/Service/interface/ServiceFactory.h"
#include "PluginManager/ModuleDef.h"
#include<malloc.h>
#include <boost/function.hpp>

namespace {

  struct UsedMemoryImpl {
    UsedMemoryImpl() {} 
    mutable struct mallinfo mi;
    int operator()() const {
      mi = ::mallinfo();
      return  mi.hblkhd + mi.uordblks;
    }
  };

 struct UsedMemory : public boost::function<int(void)> {
   UsedMemory() : boost::function<int(void)>(UsedMemoryImpl()){} 
  };


}

typedef perftools::ServiceMaker< boost::function<int(void)>, UsedMemory> UsedMemoryService;

DEFINE_SEAL_MODULE ();
DEFINE_SEAL_PLUGIN (perftools::ServiceFactory, 
		    UsedMemoryService,
		    "PerfTools:UsedMemory");


