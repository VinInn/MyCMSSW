#ifndef PerfTools_Sampler_H
#define PerfTools_Sampler_H

#include <boost/function.hpp>


namespace perftools {

  template <typename T, typename D=T>
  class Sampler {
  public:
    typedef T Value;
    typedef D Difference;
    typedef boost::function<T(void)> Source;
    typedef boost::function<void(D)> Report;

  public:
    template<typename S, Typename R>
    Sampler(S isource, R ireport) : 
      m_source(isource),
      m_report(ireport), 
      m_firstValue(m_source()) {
      
    }

    ~Sampler() {
      m_report(sample());
    }

    Difference sample() const {
      return m_source()-m_firstValue;
    }

  private:
    Source m_source;
    Report m_report;
    Value m_firstValue;
  };

  /*
  namespace detail {
    
    template <typename Source, typename Report> 
    struct HelpMaker {
      typedef typename function_object_result<Source>::type type;
    };
    
  }
  
  template <typename Source, typename Report> 
  Sampler<typename detail::HelpMaker<Source,Report>::type, Source, Report>
  makeSampler(Source isource, Report ireport) {
    return Sampler<typename detail::HelpMaker<Source,Report>::type,Source,Report>(isource,ireport);
  }
  */

}


#endif // PerfTools_Sampler_H
