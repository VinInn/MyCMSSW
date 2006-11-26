#include "Reflex/Reflex.h"
#include "Reflex/Builder/CollectionProxy.h"
#include "PluginManager/PluginManager.h"
#include <map>
#include<utility>
#include<vector>
#include<string>
#include "PoolTests/NavigationTests/interface/ABC.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include<typeinfo>
#include<boost/archive/text_oarchive.hpp>
#include<boost/archive/text_iarchive.hpp>
#include<cmath>

#include "PoolTest/SLCacheTest/interface/Streamer.h"



using namespace NavigationTests;

int main() {

  try {
    seal::PluginManager::get()->initialise();
    
    using ROOT::Reflex::Type;
    using ROOT::Reflex::Member;

  //  const Class * simple = Class::forName("NavigationTests::Simple");
  Type simple = Type::byTypeInfo(typeid(NavigationTests::Simple));

  for (size_t i=0; i<simple.dataMemberCount(); i++) {
    Member m = simple.dataMember(i);
    std::cout << m.name() << " " << m.type().name() << std::endl;
  }
  std::stringstream file;

  {
    
    NavigationTests::Simple s; s.a=3.14; s.b=sqrt(3.); s.j=12;
    NavigationTests::Compound c; c.q = 99; c.name="Me"; c.i=-45; c.a=s; c.b=s; c.b.b=sqrt(5);
    c.v.push_back(s); c.v.push_back(s);
    c.m[1]="one"; c.m[10]="ten"; c.m[5]="five"; c.m[20]="2ten";
    //    std::sstream file("bha.txt");
    boost::archive::text_oarchive oa(file);
    {
      ROOT::Reflex::Object rc(simple, (void*)(&s));
      oa << rc;
    }
    {
      ROOT::Reflex::Object rc(Type::byTypeInfo(typeid(c)), (void*)(&c));
      oa << rc;
    }
    //file.close();
      
    std::cout << s.a << " " << s.b << " " << s.j << std::endl; 
    std::cout << c.q << " " << c.name << " "  << c.v.size() << " " 
	      << c.a.a << " " << c.a.b << " " << c.a.j << std::endl; 
    std::cout << c.m[1] << " " << c.m[5] << " " << c.m[10] << " " << c.m[20] << std::endl; 
  }
  std::cout << file.str() << std::endl;
  {
  
    NavigationTests::Simple s;
    NavigationTests::Compound c;    
    // std::ifstream file("bha.txt");
    boost::archive::text_iarchive oa(file);
    {
      ROOT::Reflex::Object rc(simple, (void*)(&s));
      oa >> rc;
    }
    {
      ROOT::Reflex::Object rc(Type::byTypeInfo(typeid(c)), (void*)(&c));
      oa >> rc;
    }
    //    file.close();
    std::cout << s.a << " " << s.b << " " << s.j << std::endl; 
    std::cout  << c.q << " "<< c.name << " " << c.v.size() << " " 
	       << c.a.a << " " << c.a.b << " " << c.a.j << std::endl; 
    std::cout << c.v[0].a << " " << c.v[0].b << " " << c.v[0].j << std::endl; 
    std::cout << c.m[1] << " " << c.m[5] << " " << c.m[10] << " " << c.m[20] << std::endl; 

  }
  } catch (std::exception & ex) {
    std::cout << "error!!!" << ex.what() << std::endl;
  }

  return 0;

};
