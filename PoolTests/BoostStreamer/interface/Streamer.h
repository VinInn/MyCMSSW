#ifndef Persil_Streamer_H
#define Persil_Streamer_H

#include "Reflex/Reflex.h"
#include "Reflex/Builder/CollectionProxy.h"
#include<string>
#include<typeinfo>
#include <iostream>
#include <iosfwd>
#include<map>
#include<memory>

// #include "DataSvc/RefBase.h"
// #include "POOLCore/Token.h"

#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

/*
namespace pool {
  class Token;
}
*/
/*
namespace pool {
  class  ICacheSvc;
}
*/

namespace Persil {

  inline bool & debug() {
    static bool me=false;
    return me;
  }


/*

  //need to move to context...
  inline pool::ICacheSvc* & currentCache() {
    static pool::ICacheSvc * local=0;
    return local;
  }

*/

  struct TypeNotRegistered {};
  
  template<class T>
  class RTTmap {
  public:
    typedef const std::type_info & rinfo;
    typedef const std::type_info * pinfo;
    typedef std::map<pinfo, T> RTTMAP; 
    typedef typename RTTMAP::const_iterator const_iterator;
    
    void regist(rinfo p, T t) {
      m_map.insert(std::make_pair(&p,t));
    }
    
    const T & operator[](rinfo p) const {
      const_iterator i = m_map.find(&p);
      if (i==m_map.end()) throw TypeNotRegistered();
      return (*i).second;      
    }
    
    const T * find(rinfo p) const {
      const_iterator i = m_map.find(&p);
      if (i==m_map.end()) return 0;
      return &(*i).second;      
    }

    RTTMAP m_map;
  };


  template<typename Archive, typename T>
  struct  Serializer {
    static  void serialize(Archive & ar, const std::type_info &, void * p) {
      ar & *(T*)(p);
      // if (debug())  std::cout << p << " ||" << *(T*)(p) << "|| ";
    }
  };




  
  template<typename Archive>
  struct RTTio {
    typedef void (*Any2Ar)(Archive & ar, const std::type_info & ti, void * p);
    
    RTTio() {
      regist<bool>();
      regist<int>();
      regist<size_t>();
      regist<unsigned int>();
      regist<std::string>();
      regist<float>();
      regist<double>();
      // regist<std::vector<bool> >();
      // regist<std::vector<int> >();
      // regist<std::vector<double> >();
      // regist<std::vector<std::string> >();
      // regist<std::map<int,std::string> >();
    }
    
    template<typename T> void regist() {
      iomap.regist(typeid(T), Serializer<Archive,T>::serialize);
    }
    
    bool serializePrimitive(Archive & ar, const std::type_info & ti, void * p) {
      const Any2Ar * s = iomap.find(ti);
      if (!s) return false;
      (*s)(ar,ti,p);
      return true;
    }
    
    
    
    RTTmap<Any2Ar> iomap;
  };



/*
  template<typename Archive>
  struct  Serializer<Archive,pool::RefBase> {
    static  void serialize(Archive & ar, const std::type_info &, void * p){
      if (Archive::is_saving::value) 
	ar & ((pool::RefBase*)(p))->toString();
      else {
	pool::Token* tok = new pool::Token();
	std::string stoken;
	ar & stoken;
	tok->fromString(stoken);
	((pool::RefBase*)(p))->setToken(*tok);
	((pool::RefBase*)(p))->setCacheSvc(currentCache());
	tok->release();
      }
      
    }
  };

*/

  template<typename Archive>
  bool isContainer(Archive & ar, ROOT::Reflex::Object & ob) {
    
    const std::type_info * ti=0;
    ROOT::Reflex::Type tc = ob.Type();
    
    
    for (size_t i=0; i<tc.SubTypeCount(); i++) {
      if (tc.SubType(i).name()=="value_type") {
	ti= &tc.SubType(i).typeInfo();
	break;
      }
    }
    
    if (!ti) return false;

    //    std::cout << "value_type " << ti->name() << std::endl;
    
    ROOT::Reflex::Type t =  ROOT::Reflex::Type::ByTypeInfo(*ti);
    
    // std::vector<void*> v; 
    std::auto_ptr<ROOT::Reflex::CollFuncTable> cft( 
						   (ROOT::Reflex::CollFuncTable*)(ob.Invoke("createCollFuncTable").Address())
						   );
    ROOT::Reflex::Environ<long> env;
    env.object = ob.Address();

    if (Archive::is_saving::value) {
      size_t s = *(size_t*)cft->size_func(&env);
      // std::cout << "size " << s << " value_type " << ti->name() << std::endl;     
      ar & s;
      if(s>0)
	for ( void* o = cft->first_func(&env); o; o = cft->next_func(&env)) {
	  ar & ROOT::Reflex::Object(t,o); env.idx=1;
	}
    } else {    
      size_t s;
      ar & s;
      // std::cout << "size " << s << " value_type " << ti->name() << std::endl;     
      if (s>0) {
	if (tc.TemplateFamily().name(ROOT::Reflex::SCOPED)=="std::vector"){
	  //	  std::cout << "reserve for vector " << s << std::endl;
	  std::vector<void *> v(1); v[0] = (void*)(&s);
	  ob.Invoke("reserve",v);
	}
	 
	env.size=1;
	for (size_t i=0; i<s; i++) {
	  ROOT::Reflex::Object v = t.Construct(); 
	  env.start = v.Address();
	  ar & v;
	  cft->feed_func(&env);	
	  t.Destruct(v.Address());
	}
	

      }
      if (s!= *(size_t*)ob.Invoke("size").address()) 
	std::cout << "error!!! read " << s << " instead of " 
		  << *(size_t*)ob.invoke("size").address() << std::endl;
    }
    return true;
  }

  template<typename Archive>
  RTTio<Archive> & rttio() {
    static  RTTio<Archive> local;
    return local;
  }

}


namespace boost {
  namespace serialization {
    
    
    template<typename Archive>
    void serialize(Archive & ar, ROOT::Reflex::Object & ob, unsigned int) {
      using ROOT::Reflex::Type;
      using ROOT::Reflex::Member;
      Type tc = ob.Type();
    
      bool locdeb=false;
      
      /*
      if (tc.TemplateFamily().name(ROOT::Reflex::SCOPED)=="std::pair"){
	 std::cout << "serializing " << tc.name() << " at " <<  ob.address() << std::endl;
	 Persil::debug()=true;
	 locdeb=true;
      }
      */

      if ( Persil::rttio<Archive>().serializePrimitive(ar, ob.type().typeInfo(), ob.address()) );
      else if (ob.type().IsPointer() )  std::cout << "pointers not supported yet" << std::endl;
      else if (ob.type().IsArray() )  std::cout << "arrays not supported yet" << std::endl;
      else if (Persil::isContainer(ar, ob) );
      else { // class or struct

	if (Persil::debug()) {
	  for (size_t i=0; i<tc.DataMemberCount(); i++) {
	    Member m = tc.DataMember(i);
	    std::cout << m.Name() << " " << m.Type().Name() << ", ";
	  }     
	  std::cout <<  std::endl;
	}
	
	for (size_t i=0; i<tc.BaseCount(); i++) {
	  ROOT::Reflex::Object rc(tc.Base(i).ToType(),ob.Address()+tc.Base(i).Offset(ob.Address()));
	  ar & rc;
	}
	for (size_t i=0; i<tc.DataMemberCount(); i++) {
	  Member m = tc.DataMember(i);
	  if (!m.IsTransient()) ar & m.Get(ob);
	}
      }

      if (locdeb) {
	std::cout << std::endl;
	Persil::debug()=false;
      }
    }
  }
  
}

#endif // Persil_Streamer_H
