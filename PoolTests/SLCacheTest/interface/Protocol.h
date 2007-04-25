#ifndef Persil_Protocol_H
#define Persil_Protocol_H

#include <string>
#include <iosfwd>
#include <typeinfo>
#include <sstream>

namespace ROOT {
  namespace Reflex {
    class Object;
  }

}

namespace Persil {

/**  ABI for a communication protocol using Reflex
 */

  class ProtocolDecoder;

  class Protocol {
  public:
    Protocol():dummy(0){}
    virtual ~Protocol(){}

  public:
    virtual void action(ProtocolDecoder & decoder)=0;

  private:
    int dummy;
  };

  class Payload : public Protocol {
  public:
    // in reading
    virtual void action(ProtocolDecoder & decoder);
  };

  struct Consumer {
    virtual ~Consumer(){}
    virtual void operator()(const ROOT::Reflex::Object &ob)=0;
  };

  class Eof : public Protocol {
  public:
    virtual void action(ProtocolDecoder & decoder);
  };

  class ProtocolDecoder {
  public:
    ProtocolDecoder(std::iostream & s, Consumer * c) : 
      m_eof(false), m_consumer(c), stream(s){}
    virtual ~ProtocolDecoder(){}

    bool decode();

    ROOT::Reflex::Object get();
    // call consumer...
    void consume();

    void eof() { m_eof=true;}
    void reset() { m_eof=false;}
    Consumer * consumer() { return m_consumer;}

  private:
    bool m_eof;
    Consumer * m_consumer;
    std::iostream & stream;

  };

  class ProtocolEncoder {
  public:
    explicit ProtocolEncoder(std::iostream & s) : stream(s){}
    virtual ~ProtocolEncoder(){}

    template<typename T> void encode(const T & t) {
      encodeImpl(typeid(t), (void*)(&t));
    }
 
    template<typename T> void encodePoly(const T & t) {
      encodeImpl(typeid(t), dynamic_cast<void*>(const_cast<T*>(&t)));
    }

    template<typename T> void encodeDirectPoly(const T & t) {
      encodeDirect(typeid(t), dynamic_cast<void*>(const_cast<T*>(&t)));
    }

    void encodeNaked(const std::string& className,  const std::string& payload);

    void encodeImpl(const std::type_info & ti, void * v);

    void encodeDirect(const std::type_info & ti, void * v);

  private:

    std::iostream & stream;

  };
 
  template<> 
  void ProtocolEncoder::encode<Protocol>(const Protocol & proc);
  template<>
  void ProtocolEncoder::encode<ROOT::Reflex::Object>(const ROOT::Reflex::Object & ob);


  /** not a protocol...
   */
  std::string MD5(const std::string & s);
  std::string MD5(const std::type_info & ti, void * v);

  template<typename T>
  std::string objectMD5(const T& t) {
    std::stringstream ss;
    ProtocolEncoder e(ss);
    e.encode(t);
    return MD5(ss.str());
  }
    
}


#endif //  Persil_Protocol_H
