#include "PerfTools/Service/interface/ServiceFactory.h"
#include "PluginManager/PluginManager.h"

#include <memory>
#include <iostream>
namespace perftools {

  ServiceFactory * ServiceFactory::get (void) {
    return &instance();
  }
  
  ServiceFactory::ServiceFactory() : 
    seal::PluginFactory<BaseServiceMaker *(void)> ("CMS PerfToos Service Maker") {}
  
  ServiceFactory::~ServiceFactory () {}
  
  boost::any ServiceFactory::getAny(std::string const & name) {
    boost::any & h = m_services[name];
    if (h.empty()) {
      std::auto_ptr<Maker> m(create(name));
      if (m.get()) h = (*m)();
    }
    if (h.empty()) reportErrorNoService(name);
    return h;
  }
  
  void  ServiceFactory::reportErrorNoService(std::string const & name) const {
    std::cerr << "PerfTools Service named " << name 
	      << "not found" << std::endl;
  }
  
  void  ServiceFactory::reportWrongType(std::string const & name, 
					char const * type) const {
    std::cerr << "PerfTools Service named " << name 
	      << " is not of type " << type << std::endl;
    
  }
  
  ServiceFactory & ServiceFactory::instance() {
    seal::PluginManager::get ()->initialise ();
    static ServiceFactory local;
    return local;
  }
  
}
