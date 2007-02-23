#ifndef PerfTools_SamplerUDQ_H
#define PerfTools_SamplerUDQ_H


#include "PerfTools/Sampler/interface/SamplerBase.h"

#include <vector>
#include <string>
#include <sstream>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>


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
  
  struct RegistrerUDQI {
    RegistrerUDQI(std::vector<std:string> const & inames) : 
      names(inames) {}
    RegistrerUDQI(){}
    virtual ~RegistrerUDQI(){}
    bool virtual dont() const=0;
    std::vector<std::string> names; 
    
  };
  
  struct DoNotRegistrerUDQ : public  RegistrerUDQI {
    bool virtual dont() const { return true;}
  };
  
  template<typename UDQ>
  struct RegistrerUDQ  : public RegistrerUDQI {
    RegistrerUDQ( std::vector<std:string> const & inames) : 
      RegistrerUDQI(inames) {
    }
    
    bool virtual dont() const { return false; }
    
  };
  
  /*  abstract class toward the SamplerImplementation
   */
  class UDQBase {
    virtual ~UDQBase(){}
    virtual void toDouble(std::vector<double>& v) const=0;
    virtual void toString(std::vector<std::string>& v) const=0;
    
  };
  
  /*
   * a sampler with UserDefinedQuantities
   */
  template<typename UDQ>
  class SamplerUDQ : public UDQBase {
  public:
    
    SamplerUDQ() {}

    virtual void toDouble(std::vector<double>& v) const {
      convert(v);
    }
    virtual void toString(std::vector<std::string>& v) const {
      convert(v);
    }

    // FIXME faster, templated?
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
  
  template<typename UDQ>
  inline SamplerUDQ<UDQ> * asUDQ(boost::any& ba) {
    return dynamic_cast<SamplerUDQ<UDQ> *>(boost::unsafe_any_cast<perftools::UDQBase>(&ba));
  }


}

#endif //  PerfTools_SampleUDQ_H
