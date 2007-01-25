#ifndef PerfTools_SamplerImpl_H
#define PerfTools_SamplerImpl_H

#include <boost/function.hpp>
#include <functional>

namespace perftools {

  template <typename T, typename D=T, typename M=std::minus<T> >
  class SamplerImpl {
  public:
    typedef T Value;
    typedef D Difference;
    typedef M Minus;
    typedef boost::function<T(void)> Source;
    typedef boost::function<void(D)> Report;

  public:
    template<typename S, typename R>
    SamplerImpl(S isource, R ireport, bool doReport=true, bool isTemplate=false) : 
      m_doReport(doReport),
      m_templ(isTemplate),
      m_source(isource),
      m_report(ireport), 
      m_firstValue( (!isTemplate) && doReport ? m_source() : Value()) { 
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
      if((!m_templ) && m_doReport) m_report(sample());
    }

    Difference sample() const {
      return minus(m_source(),m_firstValue);
    }

  private:
    mutable bool m_doReport;
    mutable bool m_templ; // if true object is just a "template"
    Source m_source;
    Report m_report;
    Value m_firstValue;
    Minus minus;
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
