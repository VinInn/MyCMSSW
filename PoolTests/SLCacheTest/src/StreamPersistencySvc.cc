#include "PoolTest/SLCacheTest/interface/StreamPersistencySvc.h"
#include "PoolTest/SLCacheTest/interface/Getter.h"
#include "PoolTest/SLCacheTest/interface/Streamer.h"
#include "POOLCore/Token.h"
#include "StorageSvc/DbReflex.h"
#include "Utilities/Notification/interface/TimerProxy.h"
// #include "Utilities/Persistency/interface/Streams.h"
#include <iostream>
#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/binary_iarchive.hpp>
#include <sstream>
#include<utility>
#include<memory>
#include "Reflection/Class.h"

StreamPersistencySvc::StreamPersistencySvc(super & it, Persil::GetterFactory & igetf, LockMutex::Mutex & m):
  me(it),getterFact(igetf), mutex(m),
  searchIt("PersistencySvc:search"),
  readIt("PersistencySvc:read") {}

StreamPersistencySvc::~StreamPersistencySvc(){}

std::pair< void*, seal::reflex::Type > 
StreamPersistencySvc::readObject( const pool::Token& token,
				  pool::DataHandler* handler )    {
  LockMutex gl(mutex);
  
  
  //  if (Persil::cout().debugOut)
  //  Persil::cout().debugOut 
  std::cout  << "reading Object " << token.toString() << std::endl;
  // TimeMe a(readIt,true);
  
 
  seal::reflex::Type type = pool::DbReflex::forGuid(token.classID());
  //  const seal::reflect::Class* cl = seal::reflect::Class::forGuid(token.classID().toString());
  // seal::reflex::Type type = seal::reflex::Type::byTypeInfo(cl->typeinfo());
  seal::reflex::Object ob = type.construct();
  // std::istringstream file; // (std::ios::binary); // file.setf( std::ios::skipws );
  std::istream file(0);
  std::auto_ptr<Persil::Getter> getter(getterFact.create());
  (*getter)(file,token,type.name(seal::reflex::SCOPED));
  if (file) {
    boost::archive::binary_iarchive ia(file);
    // boost::archive::text_iarchive ia(file);
    ia >> ob;
    std::cout << "good and eof " << file.good() << " " << file.eof() <<std:: endl;
    std::cout << "object " << type.name() <<" at " << ob.address() << std::endl;
    return std::make_pair((void*)ob.address(),type);
  }
  else {
    std::cout << "ERROR: good and eof " << file.good() << " " << file.eof() <<std:: endl;
    return std::make_pair(0,seal::reflex::Type());
  }
}
