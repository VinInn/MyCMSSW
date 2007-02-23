#ifndef PerfTools_SamplerUDQ_H
#define PerfTools_SamplerUDQ_H


#include "PerfTools/Sampler/interface/SamplerBase.h"

#include <vector>
#include <string>
#include <sstream>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>
#include<boost/any.hpp>
#include<memory>

namespace perftools {
  
  namespace udqUtils {
    struct Convert {
      template <typename T, typename State>
      struct result {   typedef State type; };
      
      template<typename T, typename It>
      It  operator() (T const & t, It i) const {
	convert(t,*i);
	return ++i;
      }
      template<typename T, typename V>
      void convert(T const & t, V & v)  const {
	v = t;
      }
      template<typename T>
      void convert(T const & t, std::string & v) const {
	static std::ostringstream os;
	os.str("");
	os << t;
	v = os.str();
      }
      //  std::ostringstream os;
    };
    
  }
  
  struct RegisterUDQI {
    RegisterUDQI(std::vector<std::string> const & inames) : 
      names(inames) {}
    RegisterUDQI(){}
    virtual ~RegisterUDQI(){}
    bool virtual dont() const=0;
    std::vector<std::string> names; 
    
  };
  
  struct DoNotRegistrerUDQ : public  RegisterUDQI {
    bool virtual dont() const { return true;}
  };
  
  template<typename UDQ>
  struct RegisterUDQ  : public RegisterUDQI {
    RegisterUDQ( std::vector<std::string> const & inames) : 
      RegisterUDQI(inames) {
    }
    
    bool virtual dont() const { return false; }
    
  };
  
  /*  abstract class toward the SamplerImplementation
   */
  class UDQBase {
  public:
    virtual ~UDQBase(){}
    virtual void toDouble(std::vector<double>& v) const=0;
    virtual void toString(std::vector<std::string>& v) const=0;
    
  };
  
  /*
   * a sampler with UserDefinedQuantities
   */
  template<typename UDQ>
  class UDQWrapper : public UDQBase {
  public:
    
    UDQWrapper() {}

    virtual void toDouble(std::vector<double>& v) const {
      convert(v);
    }
    virtual void toString(std::vector<std::string>& v) const {
      convert(v);
    }

    // FIXME make it faster, no copy, templated?
    void fill(UDQ & udq) { m_udq = udq;}

    template<typename T> 
    void convert(std::vector<T>& v) const {
      std::vector<T> lv(boost::fusion::size(m_udq));
      boost::fusion::fold(m_udq,lv.begin(),udqUtils::Convert());
      lv.swap(v);
    }

    void print(std::ostream & co) const {
      co << m_udq;
    }

   private:

    UDQ m_udq;

  };
  

  class SamplerUDQ : public SamplerBase {
  public:
    typedef std::auto_ptr<UDQBase> Value;
    SamplerUDQ(bool aTemplate=false) : SamplerBase(aTemplate){}
    SamplerUDQ(Value value, bool aTemplate=false) : 
      SamplerBase(aTemplate),
      m_value(value) {}

    template<typename UDQ>
    void fill(UDQ & udq) {
      UDQWrapper<UDQ> * w = this->asUDQ<UDQ>();
      // FIXME striclty speaking we may change the type of the UDQ here...
      if (!w) {
	w =  new UDQWrapper<UDQ>;
	m_value.reset(w);
      }
      (*w).fill(udq);
    }

    template<typename UDQ>
    UDQWrapper<UDQ> * asUDQ() {
      return dynamic_cast<UDQWrapper<UDQ> *>(sample());
    }
    
    bool empty() const {
      return m_value.get()!=0;
    }

   UDQBase * sample() {
     return m_value.get();
   }
    
  private:
    Value m_value;
    
  };

  inline SamplerUDQ * asSamplerUDQ(boost::any& ba) {
    return dynamic_cast<SamplerUDQ *>(boost::unsafe_any_cast<perftools::SamplerBase>(&ba));
  }


}

#endif //  PerfTools_SampleUDQ_H
