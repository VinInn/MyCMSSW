#ifndef PerfTools_ServiceFactory_H
#define PerfTools_ServiceFactory_H


# include "PluginManager/PluginFactory.h"


#include<map>
#include<string>
#include<boost/shared_ptr.hpp>
#include<boost/any.hpp>
#include<typeinfo>

namespace perftools {

  /*
   * most probably there is a more elegant solution....
   */
  struct BaseServiceMaker {
    virtual ~BaseServiceMaker(){}
    virtual boost::any operator()()=0;
  };

  template<typename Service, typename Instance=Service>
  struct ServiceMaker : public BaseServiceMaker {
    boost::any operator()() {
      return boost::shared_ptr<Service>(new Instance);
    }
  };
  

  /* actually a very generic factory of singletons...
   * 
   */
  class ServiceFactory : public seal::PluginFactory<BaseServiceMaker * (void)>
  {
  public:
    typedef BaseServiceMaker Maker;
    static ServiceFactory * get (void);
    ~ServiceFactory ();
    
    template<typename Service>
    boost::shared_ptr<Service> getService(std::string const & name) {
      try {
	return boost::any_cast<boost::shared_ptr<Service> >(getAny(name));
      } catch (boost::bad_any_cast const &) {
	reportWrongType(name,typeid(Service).name());
      }
      return boost::shared_ptr<Service>();
    }

    boost::any getAny(std::string const & name);

    
  private:
    ServiceFactory();
    void reportErrorNoService(std::string const & name) const;
    void reportWrongType(std::string const & name, char const * type) const;
    
    static ServiceFactory & instance();
    typedef std::map<std::string, boost::any> Repository;
    Repository m_services;
    
    
  };

}

#endif  // PerfTools_ServiceFactory_H
