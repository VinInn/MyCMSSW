#include "PoolTest/SLCacheTest/interface/Protocol.h"
#include "PoolTest/SLCacheTest/interface/Streamer.h"
#include "PluginManager/PluginCapabilities.h"

#include<boost/archive/binary_iarchive.hpp>
#include<boost/archive/binary_oarchive.hpp>
#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>
#include<iostream>
#include "SealZip/MD5Digest.h"


namespace Persil {

  typedef boost::archive::binary_iarchive iarchive;
  // typedef boost::archive::text_iarchive iarchive;

  typedef boost::archive::binary_oarchive oarchive;
  // typedef boost::archive::text_oarchive oarchive;


  bool ProtocolDecoder::decode() {
    ROOT::Reflex::Object ob = get();
    std::vector<void *> v(1); v[0] = (void*)(this);
    std::cout << "call action on " << ob.type().name() << std::endl;
    ob.invoke("action",v); 
    ob.type().destruct(ob.address());
    return !m_eof;
  }
  
  ROOT::Reflex::Object ProtocolDecoder::get() {
    std::string className;
    // stream >> className;
    std::getline(stream,className);
    std::cout << "got " << className << std::endl;
    // load capability (sic)
    // seal::PluginCapabilities::get()->load(std::string("LCGReflex/")+className);
    ROOT::Reflex::Type t = ROOT::Reflex::Type::byName(className);
    ROOT::Reflex::Object ob = t.construct();
    iarchive ia(stream);
    ia >> ob;
    return ob;
  }
 
  void ProtocolDecoder::consume() {
    if (m_consumer) (*m_consumer)(get());
  }

  void Payload::action(ProtocolDecoder & decoder) {
    decoder.consume();
  }

  void Eof::action(ProtocolDecoder & decoder) {
    decoder.eof();
  }

  template<> 
  void ProtocolEncoder::encode<Protocol>( const Protocol & proc){
    ROOT::Reflex::Object ob(ROOT::Reflex::Type::byTypeInfo(typeid(proc)),
			    dynamic_cast<void*>(const_cast<Protocol*>(&proc)));
    encode(ob);
  }
  
  template<>
  void ProtocolEncoder::encode<ROOT::Reflex::Object>(const ROOT::Reflex::Object & ob) {
    stream << ob.type().name(ROOT::Reflex::SCOPED) <<std::endl;
    oarchive oa(stream);
    oa << ob;
    stream.flush();
  }
  
  void ProtocolEncoder::encodeNaked(const std::string& className, const std::string& payload) {
    encode<Protocol>(Payload());
    stream << className <<std::endl;
    stream.write(payload.c_str(),payload.size()); stream.flush();
  }

  void  ProtocolEncoder::encodeImpl(const std::type_info & ti, void * v) {
    encode<Protocol>(Payload());
    ROOT::Reflex::Object ob(ROOT::Reflex::Type::byTypeInfo(ti),v);
    encode(ob);
  }

  void  ProtocolEncoder::encodeDirect(const std::type_info & ti, void * v) {
    ROOT::Reflex::Object ob(ROOT::Reflex::Type::byTypeInfo(ti),v);
    encode(ob);
  }

  std::string MD5(const std::string & s) {
    seal::MD5Digest md5;
    md5.update((const void *)(s.c_str()), s.size());
    return md5.format();
  }

  std::string MD5(const std::type_info & ti, void * v) {
    std::stringstream ss;
    ProtocolEncoder e(ss);
    e.encodeImpl(ti,v);
    return MD5(ss.str());
  }
}
