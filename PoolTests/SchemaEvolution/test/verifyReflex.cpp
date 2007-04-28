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
    std::cout << tc.Name(ROOT::Reflex::SCOPED) << " " 
	      << tc.RawType().Name(ROOT::Reflex::SCOPED) 
	      << " at " <<  ob.Addess() << endl;

    for (size_t i=0; i<tc.BaseSize(); i++) {
      std:: cout <<"base :";
      ROOT::Reflex::Object rc(tc.BaseAt(i).ToType(),(void*)((size_t)(ob.Address())+tc.BaseAt(i).Offset(ob.Address())));
      dump(rc);
    }
    
    for (size_t i=0; i<tc.DataMemberSize(); i++) {
      Member m = tc.DataMemberAt(i);
      std::cout << "member " << m.Name();
      ROOT::Reflex::Object lo = m.Get(ob); 
      dump(lo);
    }
}


int main() {

  shevsp::C c;
  c.b = new shevsp::A1();
  ROOT::Reflex::Type type = ROOT::Reflex::Type::ByTypeInfo(typeid(C));
  ROOT::Reflex::Object ob(type,&c);
  dump(ob);

  return 0;

}
