// later in cppUnit

#include "PoolTests/BoostStreamer/interface/Streamer.h"
#include "PoolTests/SchemaEvolution/interface/SimpleTest.h"

#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/binary_iarchive.hpp>

#include<boost/archive/text_oarchive.hpp>
#include<boost/archive/binary_oarchive.hpp>




#include <iostream>
#include <sstream>
#include <string>
#include <memory>


void verify(shevsp::C&c) {
  
  std::stringstream file;
  
  ROOT::Reflex::Type type = ROOT::Reflex::Type::ByTypeInfo(typeid(shevsp::C));
  
  ROOT::Reflex::Object ob(type,&c);
  //  boost::archive::binary_oarchive oa(file);
  boost::archive::text_oarchive oa(file);
  
  oa << ob;
  
  shevsp::C co;
  ROOT::Reflex::Object ib(type,&co);
  boost::archive::text_iarchive ia(file);
  ia >> ib;
  
  if (co!=c) std::cout << "error" << std::endl;

  std::cout << file.str() << std::endl;
}

int main() {

  shevsp::C c;
  c.i = 2;
  c.m1.a = 3.14;
  c.m2.a = -1.23567E15;
  c.b = new shevsp::A1();
  *(c.b).b = 1234567890123LL;
  dynamic_cast<shevsp::A1*>(c.b)->a=14.56;
  
  verify(c);
  c.b=0;
  verify(c);


  return 0;

}
