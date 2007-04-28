#include "PoolTests/SchemaEvolution/interface/SimpleTest.h"


#include "Reflex/Reflex.h"
#include "Reflex/Builder/CollectionProxy.h"
#include<string>
#include<typeinfo>
#include <iostream>
#include <iosfwd>
#include<map>
#include<memory>



void dump(ROOT::Reflex::Object & ob) {
    using ROOT::Reflex::Type;
    using ROOT::Reflex::Member;
    Type tc = ob.TypeOf();
    Type raw = tc.RawType();
    std::cout<< tc.TypeTypeAsString() << " "  
	     << tc.Name(ROOT::Reflex::SCOPED) << " "
	     <<  raw.Name(ROOT::Reflex::SCOPED)
	     << " at " <<  ob.Address() << std::endl;
 
    if (tc.IsPointer()) {
      ROOT::Reflex::Object ro(raw,*(void**)(ob.Address()));
      if (raw.IsClass()||raw.IsStruct()) 
	ro = ro.CastObject(ro.DynamicType());
      dump(ro);
      return;
    }
    if (!(tc.IsClass()||tc.IsStruct())) return; 

    std::cout << ob.DynamicType().Name(ROOT::Reflex::SCOPED) << std::endl;

    for (size_t i=0; i<tc.BaseSize(); i++) {
      std:: cout <<"base :";
      ROOT::Reflex::Object rc(tc.BaseAt(i).ToType(),(void*)((size_t)(ob.Address())+tc.BaseAt(i).Offset(ob.Address())));
      dump(rc);
    }
    
    for (size_t i=0; i<tc.DataMemberSize(); i++) {
      Member m = tc.DataMemberAt(i);
      std::cout << "member " << m.Name() <<" ";
      ROOT::Reflex::Object lo = m.Get(ob); 
      dump(lo);
    }
}


int main() {

  shevsp::C c;
  std::cout << (void*)(&c) << std::endl; 
  c.b = new shevsp::A1();
  std::cout << (void*)(c.b) << std::endl; 
  std::cout << (void*)(&c.b) << std::endl; 
  ROOT::Reflex::Type type = ROOT::Reflex::Type::ByTypeInfo(typeid(c));
  ROOT::Reflex::Object ob(type,&c);
  dump(ob);

  return 0;

}
