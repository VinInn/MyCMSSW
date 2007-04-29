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


void verify(C&c) {

  std::stringstream file;
  
  ROOT::Reflex::Type type = ROOT::Reflex::Type::ByTypeInfo(typeid(C));
  
  ROOT::Reflex::Object ob(type,&c);
  //  boost::archive::binary_oarchive oa(file);
  boost::archive::text_oarchive oa(file);
  
  oa << ob;
  
  shevsp::C co;
  ROOT::Reflex::Object ib(type,&co);
  boost::archive::text_iarchive ia(file);
  ia >> ib;
  
  if (co!=c) std::cout << "error" << std::endl;
}

int main() {

  shevsp::C c;
  c.i = 2;
  c.m1.a = 3.14;
  c.m2.a = -1.23567E15
  c.b = new shevsp::A1();
  *c.b = 1234567890123L;
  dynamic_cast<A1*>(c.b)->a=14.56;
  
  verify(c);
  c.b=0;
  verify(c);


  return 0;

}
