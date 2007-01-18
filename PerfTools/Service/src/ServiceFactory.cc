#include "PerfTools/Service/interface/ServiceFactory.h"


namespace perftools {

  StorageFactory::StorageFactory & get (void) {
    return instance();
  }
  StorageFactory::~StorageFactory () {}
    
  boost::any StorageFactory::getAny(std::string & const name) {
    boost::any & h = m_services[name];
    if (h.empty()) h = create(name);
    if (h.empty()) reportErrorNoService(name);
    return h;
  }
  
  void  StorageFactory::reportErrorNoService(std::string & const name) const {

  }

  void  StorageFactory::reportWrongType(std::string & const name, 
					char const * type) const {
    
  }
    
  ServiceFactory &  StorageFactory::instance() {
    static ServiceFactory local;
    return local;
  }
    
}
