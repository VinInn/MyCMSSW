#ifndef PerfTools_SamplerImpl_H
#define PerfTools_SamplerImpl_H

#include "PerfTools/interface/interface/SamplerBase.h"

#include <boost/function.hpp>
#include <functional>
#include<vector>
#include<algorithm>
#include<boost/bind.hpp>
#include<boost/bind/apply.hpp>
 
namespace perftools {

  template <typename T, typename D=T, typename M=std::minus<T> >
  class SamplerImpl : public SamplerBase {
  public:
    typedef T Value;
    typedef D Difference;
    typedef M Minus;
    typedef boost::function<T(void)> Source;
    typedef boost::function<void(D)> Report;

  public:
    template<typename S, typename R>
    SamplerImpl(S isource, R ireport, bool doReport=true, bool aTemplate=false) :
      SamplerBase(aTemplate),
      m_doReport(doReport),
      m_source(isource),
      m_report(ireport), 
      m_firstValue( (!aTemplate) && doReport ? m_source() : Value()) { 
    }

    SamplerImpl() : m_doReport(false){}

    /* copy constructor
     *  restart sampling and inhibit report from right hand
     */
    SamplerImpl(SamplerImpl const & rh) : SamplerBase(rh),
      m_doReport(rh.isTemplate() ? rh.m_doReport : true),
      m_source(rh.m_source),
      m_report(rh.m_report), 
      m_firstValue((!rh.isTemplate()) && m_doReport ? m_source() : Value())
    {
      if (!rh.isTemplate()) rh.m_doReport=false;
    }

    /* same behaviour as copy-constr
     *
     */
    SamplerImpl & operator=(SamplerImpl const & rh) {
      SamplerBase::operator=(rh);
      m_doReport = rh.isTemplate() ? rh.m_doReport : true;
      if (!rh.isTemplate()) rh.m_doReport =false;
      m_source =rh.m_source;
      m_report = rh.m_report; 
      m_firstValue= (!rh.isTemplate()) && m_doReport ? m_source() : Value(); 
      return *this;
    }


    ~SamplerImpl() {
      if((!isTemplate()) && m_doReport) m_report(sample());
    }

    Difference sample() const {
      return minus(m_source(),m_firstValue);
    }

  private:
    mutable bool m_doReport;
    Source m_source;
    Report m_report;
    Value m_firstValue;
    Minus minus;
  };

 /* implemented over a vector
   */
  template <typename T, typename D=T, typename M=std::minus<T> >
  class SamplerImplVec {
  public:
    typedef std::vector<T> Value;
    typedef std::vector<D> Difference;
    typedef M Minus;
    typedef std::vector<boost::function<T(void)> > Source;
    typedef boost::function<void(Difference)> Report;

  public:
    template<typename SIT, typename R>
    SamplerImplVec(SIT source_b, SIT source_e, R ireport, bool doReport=true, bool isTemplate=false) : 
      m_doReport(doReport),
      m_templ(isTemplate),
      m_source(source_b,source_e),
      m_report(ireport), 
      m_firstValue(m_source.size()) {
      if ((!isTemplate) && doReport) init();
    }

    SamplerImplVec() : m_doReport(false), m_templ(false){}

    /* copy constructor
     *  restart sampling and inhibit report from right hand
     */
    SamplerImplVec(SamplerImplVec const & rh) :
      m_doReport(rh.m_templ ? rh.m_doReport : true),
      m_templ(false),
      m_source(rh.m_source),
      m_report(rh.m_report), 
      m_firstValue(m_source.size()) {
      if (m_doReport) init();
      if (!rh.m_templ) rh.m_doReport=false;
    }

    /* same behaviour as copy-constr
     *
     */
    SamplerImplVec & operator=(SamplerImplVec const & rh) {
      m_doReport = rh.m_templ ? rh.m_doReport : true;
      m_templ = false;
      if (!rh.m_templ) rh.m_doReport =false;
      m_source =rh.m_source;
      m_report = rh.m_report;
      Value temp(m_source.size());
      m_firstValue.swap(temp);
      if(m_doReport) init(); 
      return *this;
    }

    void init() {
      std::transform(m_source.begin(), m_source.end(),
		     m_firstValue.begin(),
		     boost::bind(boost::apply<T>(),_1));
    }

    ~SamplerImplVec() {
      if((!m_templ) && m_doReport) m_report(sample());
    }

    Difference sample() const {
      Difference d(m_source.size());
      std::transform(m_source.begin(), m_source.end(),
		     m_firstValue.begin(),
		     d.begin(),
		     boost::bind(minus,
				 boost::bind(boost::apply<T>(),_1),_2));
      
      return d;
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
