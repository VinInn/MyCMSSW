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
    ROOT::Reflex::Type tc = ob.TypeOf();
    
    
    for (size_t i=0; i<tc.SubTypeSize(); i++) {
      if (tc.SubTypeAt(i).Name()=="value_type") {
	ti= &tc.SubTypeAt(i).TypeInfo();
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
	   ROOT::Reflex::Object lo(t,o) ar & lo; env.idx=1;
	}
    } else {    
      size_t s;
      ar & s;
      // std::cout << "size " << s << " value_type " << ti->name() << std::endl;     
      if (s>0) {
	if (tc.TemplateFamily().Name(ROOT::Reflex::SCOPED)=="std::vector"){
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
      if (s!= *(size_t*)ob.Invoke("size").Address()) 
	std::cout << "error!!! read " << s << " instead of " 
		  << *(size_t*)ob.Invoke("size").Address() << std::endl;
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
      Type tc = ob.TypeOf();
    
      bool locdeb=false;
      
      /*
      if (tc.TemplateFamily().name(ROOT::Reflex::SCOPED)=="std::pair"){
	 std::cout << "serializing " << tc.name() << " at " <<  ob.address() << std::endl;
	 Persil::debug()=true;
	 locdeb=true;
      }
      */

      if ( Persil::rttio<Archive>().serializePrimitive(ar, ob.TypeOf().TypeInfo(), ob.Address()) );
      else if (ob.TypeOf().IsPointer() )  std::cout << "pointers not supported yet" << std::endl;
      else if (ob.TypeOf().IsArray() )  std::cout << "arrays not supported yet" << std::endl;
      else if (Persil::isContainer(ar, ob) );
      else { // class or struct

	if (Persil::debug()) {
	  for (size_t i=0; i<tc.DataMemberSize(); i++) {
	    Member m = tc.DataMemberAt(i);
	    std::cout << m.Name() << " " << m.TypeOf().Name() << ", ";
	  }     
	  std::cout <<  std::endl;
	}
	
	for (size_t i=0; i<tc.BaseSize(); i++) {
	  ROOT::Reflex::Object rc(tc.BaseAt(i).ToType(),(void*)((size_t)(ob.Address())+tc.BaseAt(i).Offset(ob.Address())));
	  ar & rc;
	}
	for (size_t i=0; i<tc.DataMemberSize(); i++) {
	  Member m = tc.DataMemberAt(i);
	  if (!m.IsTransient()) { 
	    ROOT::Reflex::Object lo = m.Get(ob); 
	    ar & lo;
	  }
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
