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
      regist<char>();
      regist<unsigned char>();
      regist<short>();
      regist<unsigned short>();
      regist<int>();
      regist<size_t>();  // shall we (it is not 64bit portable)???
      regist<unsigned int>();
      regist<long long>();
      regist<unsigned long long>();
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
  void serializeArray(Archive & ar,  ROOT::Reflex::Type type, void * start, size_t size) {
    using ROOT::Reflex::Type;
    size_t sz = type.SizeOf();
    for (size_t i=0; i<size; i++) {
      ROOT::Reflex::Object ob(type,start);
      ar & ob;
      (size_t&)(start) +=sz;
    }
  }


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

    //FIXME not real enough to caracterize a "container"
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
	  ROOT::Reflex::Object lo(t,o); ar & lo; env.idx=1;
	}
    } else {    
      size_t s;
      ar & s;
      // std::cout << "size " << s << " value_type " << ti->name() << std::endl;     
      if (s>0) {
	if (tc.TemplateFamily().Name(ROOT::Reflex::SCOPED)=="std::vector"){
	  //	  std::cout << "resize for vector " << s << std::endl;
	  std::vector<void *> v(1); v[0] = (void*)(&s);
	  ob.Invoke("resize", ROOT::Reflex::Type::ByName("void (size_t)"),v);
	  void* o = cft->first_func(&env);
	  serializeArray(ar,t,o,s);
	}
	else {
	  env.size=1;
	  for (size_t i=0; i<s; i++) {
	    ROOT::Reflex::Object v = t.Construct(); 
	    ar & v;
	    env.start = v.Address();
	    cft->feed_func(&env);	
	    t.Destruct(v.Address());
	  }
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


  template<typename Archive>
  void evolve(Archive & ar, ROOT::Reflex::Object & ob, std::string const & version) {
    using ROOT::Reflex::Type;
    using ROOT::Reflex::Member;
    Type tc = ob.TypeOf();
    std::string oldTypeName = "evolve::"+version+"::"+tc.Name(ROOT::Reflex::SCOPED);
    Type oldType = Type::ByName(oldTypeName);
    ROOT::Reflex::Object oldOb =  oldType.Construct();
    serializeMembers(ar,oldOb);
    std::vector<void *> v(1); v[0] = ob.Address();
    oldOb.Invoke("evolve",v);
    oldOb.Destruct();
  }


  template<typename Archive>
  void serializeMembers(Archive & ar, ROOT::Reflex::Object & ob) {
    using ROOT::Reflex::Type;
    using ROOT::Reflex::Member;
    Type tc = ob.TypeOf();

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

  template<typename Archive>
  bool serializePointer(Archive & ar, ROOT::Reflex::Object & ob) {
    static /* const */ std::string null("NULL");
    using ROOT::Reflex::Type;
    using ROOT::Reflex::Member;
    Type tc = ob.TypeOf();
    if (!tc.IsPointer() ) return false;
    Type raw = tc.RawType();
    void * & address = *(void**)(ob.Address());
    if (Archive::is_saving::value) {
      if (0==address) {
	ar & null;
	return true;
      }
      ROOT::Reflex::Object ro(raw,address);
      if (raw.IsClass()||raw.IsStruct()) {
	raw = ro.DynamicType();
	ro = ro.CastObject(raw);
	std::string name(raw.Name(ROOT::Reflex::SCOPED));
	ar & name;
      }
      ar & ro;
    } else {
      if (raw.IsClass()||raw.IsStruct()) {
	std::string tn;
	ar & tn;
	if (tn==null) {
	  address =0;
	  return true;
	}
	raw = Type::ByName(tn);
      }
      ROOT::Reflex::Object ro =  raw.Construct();
      ar & ro;
      address =ro.Address();
    } 
    return true;
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
      
      if ( Persil::rttio<Archive>().serializePrimitive(ar, tc.TypeInfo(), ob.Address()) );
      else if (Persil::serializePointer(ar,ob) );
      else if (tc.IsArray() ) Persil::serializeArray(ar,tc.RawType(),ob.Address(),tc.ArrayLength());
      else if (Persil::isContainer(ar, ob) );
      else { // class or struct
	
	// check version (later int...)
	std::string currentVersion="V0";
	ROOT::Reflex::PropertyList p = tc.Properties();
	if (p.HasProperty("Version")) {
	  currentVersion = p.PropertyAsString("Version");
	}
	std::string version=currentVersion;
	// there is a bug! it appears only in gcc4.  touch version force copy
	version+='a';
	version.resize(currentVersion.size());
	ar & version;
	
	if (Persil::debug()) {
	  std::cout <<  tc.Name() << " Versions " << currentVersion << " " << version << std::endl;
	}
	
	if (!Archive::is_saving::value) {
	  if (version !=currentVersion) { // shall evolve... 
	    Persil::evolve(ar, ob, version);
	    return;
	  }
	}
	Persil::serializeMembers(ar, ob);	
      }
      if (locdeb) {
	std::cout << std::endl;
	Persil::debug()=false;
      }
    }
  }
  
}

BOOST_CLASS_IMPLEMENTATION(
    ROOT::Reflex::Object, 
    boost::serialization::object_serializable
)

BOOST_CLASS_TRACKING(ROOT::Reflex::Object, boost::serialization::track_never) 



#endif // Persil_Streamer_H
