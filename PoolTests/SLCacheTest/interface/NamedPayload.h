#ifndef NamedPayload_H
#define NamedPayload_H

#include "PoolTest/SLCacheTest/interface/Protocol.h"
#include <string>

// here in order to biuld dictionary (does not work in test...)

class NamedPayload : public Persil::Protocol {
public:
  NamedPayload(){}
  NamedPayload(const std::string & iname);

  const std::string & name() const { return m_name;}

public:
  virtual void action(Persil::ProtocolDecoder & decoder);


private:

  std::string m_name; 
};

class NamedPayloadConsumer : public Persil::Consumer {
  public:
  void operator()(const ROOT::Reflex::Object &) {}
  virtual void operator()(const std::string &name, const ROOT::Reflex::Object & meta, 
			const ROOT::Reflex::Object & ob)=0;
};


namespace ProtocalEx {
  struct Meta {
    virtual ~Meta(){}
    std::string meta; 
  };

  struct Product {
    virtual ~Product(){}
  };

  template<typename T>
  struct ProductT : public Product{
    
    T t;
  };

}


#endif
