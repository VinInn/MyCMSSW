#include "PoolTests/BoostStreamer/interface/Streamer.h"
#include "PoolTests/SchemaEvolution/interface/MyTests.h"

#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/binary_iarchive.hpp>

#include<boost/archive/text_oarchive.hpp>
#include<boost/archive/binary_oarchive.hpp>




#include <iostream>
#include <fstream>
#include <string>
#include <memory>

template<typename T>
void writeIt(char const * fname,  std::auto_ptr<T> p) {
  std::ofstream file(fname);
  if (!file) { 
    std::cout << " error in opening file "
	      << fname << std::endl;
  }
  
  ROOT::Reflex::Type type = ROOT::Reflex::Type::ByTypeInfo(typeid(T));
  ROOT::Reflex::Object ob(type,p.get());
  //  boost::archive::binary_oarchive oa(file);
  boost::archive::text_oarchive oa(file);
  
  oa << ob;
}

template<typename T>
void readIt(char const * fname, std::auto_ptr<T> & p) {
  std::ifstream file(fname);
  
  if (file) {
    // we need to know what we are reading...
    ROOT::Reflex::Type type = ROOT::Reflex::Type::ByTypeInfo(typeid(T));
    ROOT::Reflex::Object ob = type.Construct();
    // boost::archive::binary_iarchive ia(file);
    boost::archive::text_iarchive ia(file);
    ia >> ob;
    p.reset((T*)ob.Address());
    
    std::cout << "good and eof " << file.good() << " " << file.eof() <<std:: endl;
    std::cout << "object " << type.Name() <<" at " << ob.Address() << std::endl;
  }
  else {
    std::cout << "ERROR: good and eof " << file.good() << " " << file.eof() <<std:: endl;
  }
}

void set1(shevts::Foo1& f) {
  static int i=100;
  f.i=i+=5;
  f.j=2*i+0.3;
}

void set2(shevts::Foo2& f) {
  static int i=500;
  f.i=i+=5;
  f.j=2*i;
}


void fill(shevts::Pub & p) {
  p.data1.resize(10);
  std::for_each(p.data1.begin(),p.data1.end(),&set1);
  p.data2.resize(5);
  std::for_each(p.data2.begin(),p.data2.end(),&set2);
}

void dumpFoo1(shevts::Foo1 const & f) {
  std::cout << f.i <<"," << f.j <<" "; 
}
void dumpFoo2(shevts::Foo2 const & f) {
  std::cout << f.i <<"," << f.j <<" "; 
}

void dump(shevts::Pub const & p) {
  std::cout << p.data1.size() << " " << p.data2.size() << std::endl;
  std::for_each(p.data1.begin(),p.data1.end(),&dumpFoo1);
  std::cout << std::endl;
  std::for_each(p.data2.begin(),p.data2.end(),&dumpFoo2);
  std::cout << std::endl;
}

int main(int npar, char * parv[]) {

  if (npar<3) 
    std::cout << "provide read/write option and file name"
	      << std::endl;

  std::auto_ptr<shevts::Pub> p;
  if (parv[1][0]=='w') {
    p.reset(new shevts::Pub);
    fill(*p);
    writeIt(parv[2],p);
  }
  else {
    readIt(parv[2],p);
    dump(*p);
  }

  return 0;
}
