#include "PerfTools/Service/interface/ServiceFactory.h"


namespace perftools {

  ServiceFactory::ServiceFactory & get (void) {
    return instance();
  }

  ServiceFactory::~ServiceFactory () {}
    
  boost::any ServiceFactory::getAny(std::string const & name) {
    boost::any & h = m_services[name];
    if (h.empty()) {
      auto_ptr<Maker> m(create(name));
      if (m) h = (*m)();
    }
    if (h.empty()) reportErrorNoService(name);
    return h;
  }
  
  void  ServiceFactory::reportErrorNoService(std::string const & name) const {

  }

  void  ServiceFactory::reportWrongType(std::string const & name, 
					char const * type) const {
    
  }
    
  ServiceFactory &  ServiceFactory::instance() {
    static ServiceFactory local;
    return local;
  }