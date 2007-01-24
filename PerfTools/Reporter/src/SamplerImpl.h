#ifndef PerfTools_SamplerImpl_H
#define PerfTools_SamplerImpl_H

#include <boost/function.hpp>


namespace perftools {

  template <typename T, typename D=T>
  class SamplerImpl {
  public:
    typedef T Value;
    typedef D Difference;
    typedef boost::function<T(void)> Source;
    typedef boost::function<void(D)> Report;

  public:
    template<typename S, typename R>
    SamplerImpl(S isource, R ireport, bool doReport=true, bool isTemplate=false) : 
      m_source(isource),
      m_report(ireport), 
      m_firstValue(doReport ? m_source() : Value()), 
      m_doReport(doReport),
      m_templ(isTemplate){
    }

    SamplerImpl() : m_doReport(false), m_templ(false){}

    /* copy constructor
     *  restart sampling and inhibit report from right hand
     */
    SamplerImpl(SamplerImpl const & rh) :
      m_doReport(rh.m_templ ? rh.m_doReport : true),
      m_templ(false),
      m_source(rh.m_source),
      m_report(rh.m_report), 
      m_firstValue(m_doReport ? m_source() : Value())
    {
      if (!rh.m_templ) rh.m_doReport=false;
    }

    /* same behaviour as copy-constr
     *
     */
    SamplerImpl & operator=(SamplerImpl const & rh) {
      m_doReport = rh.m_templ ? rh.m_doReport : true;
      m_templ = false;
      if (!rh.m_templ) rh.m_doReport =false;
      m_source =rh.m_source;
      m_report = rh.m_report; 
      m_firstValue= m_doReport ? m_source() : Value(); 
      return *this;
    }


    ~SamplerImpl() {
      if((!rh.m_temp) && m_doReport) m_report(sample());
    }

    Difference sample() const {
      return m_source()-m_firstValue;
    }

  private:
    mutable bool m_doReport;
    mutable bool templ; // if true object is just a "template"
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


#endif // PerfTools_SamplerImpl_H
