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

template<typename T>
void verify(T&c) {
  
  std::stringstream file;
  
  ROOT::Reflex::Type type = ROOT::Reflex::Type::ByTypeInfo(typeid(T));
  
  ROOT::Reflex::Object ob(type,&c);
  //  boost::archive::binary_oarchive oa(file);
  boost::archive::text_oarchive oa(file);
  
  oa << ob;
  
  T co;
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
  (*c.b).b = 1234567890123LL;
  dynamic_cast<shevsp::A1*>(c.b)->a=14.56;
  
  verify(c);
  c.b=0;
  verify(c);

  shevsp::V v;
  verify(v);

  v.ac[3] = 'b';
  v.aa[2] = c.m1;
  v.vf.resize(3,4.340);
  v.va.resize(5);
  v.va[1].a = 1.98;
  v.va[1].b = 33;
  v.vb.resize(6,(shevsp::B*)(0));
  v.vb[2] = new shevsp::A1();
  v.vb[2]->b = 9876543210LL;
  dynamic_cast<shevsp::A1*>(v.vb[2])->a=6.28;
  v.vb[4] = new shevsp::A2();
  v.vb[4]->b = 76543210LL;
  dynamic_cast<shevsp::A2*>(v.vb[2])->a=-6.28;
  v.vb[6] = new shevsp::A1();
  v.vb[6]->b = 1876543210LL;
  dynamic_cast<shevsp::A1*>(v.vb[2])->a=4.28;
  v.vb[8] = new shevsp::A2();
  v.vb[8]->b = 6543210LL;
  dynamic_cast<shevsp::A2*>(v.vb[2])->a=-4.28;

  verify(v);


  return 0;

}
