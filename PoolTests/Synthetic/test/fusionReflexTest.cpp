#include "PoolTests/Synthetic/interface/Synthesis.h"
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>

#include "Reflex/Reflex.h"

#include<iostream>

#include <typeinfo>
#include<vector>
#include<boost/shared_ptr.hpp>
#include <boost/assign/std/vector.hpp>


int main() {

  std::cout << ROOT::Reflex::Type::ByTypeInfo(typeid(int)).Name()  << std::endl;
  std::cout << ROOT::Reflex::Type::ByTypeInfo(typeid(std::string)).Name()  << std::endl;
  std::cout << ROOT::Reflex::Type::ByTypeInfo(typeid(std::vector<int>)).Name()  << std::endl;



  typedef boost::fusion::vector<int, char, double, std::string, std::vector<int> > seq_type;

  std::vector<std::string> names; names += "id", "dir";
  
  synthetic::Dict<seq_type>("MyClass",names.begin(),names.end());
  
  
  seq_type t(1, 'x', 3.3, "hello", std::vector<int>(4,0));
  
  ROOT::Reflex::Type t1 = ROOT::Reflex::Type::ByName("::synthetic::MyClass");

  if (t1) {
    if (t1.IsClass()) std::cout << "Class ";
    std::cout << t1.Name();
  }
  else std::cout << "not there ";
  std::cout << std::endl;

  for (ROOT::Reflex::Member_Iterator mi = t1.DataMember_Begin(); mi != t1.DataMember_End(); ++mi) {
    std::cout << (*mi).Name(ROOT::Reflex::SCOPED) << " " 
	      << (*mi).TypeOf().Name(ROOT::Reflex::QUALIFIED) << std::endl;
  }

  ROOT::Reflex::Object o1(t1,&t);

  {
    ROOT::Reflex::Object mem_obj = o1.Get("m_0");
    int real_value = 0;
    if (mem_obj.TypeOf().Name() == "int")
      real_value = ROOT::Reflex::Object_Cast<int>(mem_obj);
    std::cout << mem_obj.TypeOf().Name() <<  " " << real_value << std::endl;
  }
  {
    ROOT::Reflex::Object mem_obj = o1.Get("m_2");
    double real_value = 0.;
    if (mem_obj.TypeOf().Name() == "double")
      real_value = ROOT::Reflex::Object_Cast<double>(mem_obj);
    std::cout  << mem_obj.TypeOf().Name() <<  " " << real_value << std::endl;
  }


  return 0;

}
