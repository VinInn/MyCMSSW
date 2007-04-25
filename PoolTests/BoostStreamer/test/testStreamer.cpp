#include "PoolTests/BoostStreamer/interface/Streamer.h"
#include "PoolTests/SchemaEvolution/interface/MyTests.h"

#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/binary_iarchive.hpp>




#include <iostream>
#include <fstream>
#include <string>
#include <memory>

template<typename T>
void write(char const * fname,  std::auto_ptr<T> p) {
  ofstream file(fname);
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
void read(char const * fname, std::auto_ptr<T> & p) {
  ifstream file(fname);
  
  if (file) {
    // we need to know what we are reading...
    ROOT::Reflex::Type type = ROOT::Reflex::Type::ByTypeInfo(typeid(T));
    ROOT::Reflex::Object ob = type.construct();
    // boost::archive::binary_iarchive ia(file);
    boost::archive::text_iarchive ia(file);
    ia >> ob;
    p.reset((T*)ob.Address());
    
    std::cout << "good and eof " << file.good() << " " << file.eof() <<std:: endl;
    std::cout << "object " << type.Name() <<" at " << ob.Address() << std::endl;
  }
  else {
    t=0;
    std::cout << "ERROR: good and eof " << file.good() << " " << file.eof() <<std:: endl;
    
  }
}

void set(Foo1& f) {
  static int i=0;
  f.i=i++;
  f.j=2*i;
}

void fill(Pub & p) {
  p.data.resize(10);
  std::foreach(p.data.begin(),p.data.end(),&set);
}

void dump(Pub const & p) {
  std::cout << p.data.size() << std::endl;
  
}
int main(int npar, char * parv) {

  if (npar<3) 
    std::cout << "provide read/write option and file name"
	      << std::endl;

  std::auto_ptr<shevts::Pub> p=0;;
  if (parv[1]='w') {
    p.reset(new shevts::Pub);
    fill(*p);
    write(parv[2],p);
  }
  else {
    read(parv[2],p);
    dump(*p);
  }

  return 0;
}
