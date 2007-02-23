#ifndef PerfTools_SamplerUDQImpl_H
#define PerfTools_SamplerUDQImpl_H

#include "PerfTools/Sampler/interface/SamplerUDQ.h"

#include <boost/function.hpp>
#include <functional>
#include<vector>
#include<algorithm>
#include<boost/bind.hpp>
#include<boost/bind/apply.hpp>
 
namespace perftools {

  class SamplerUDQImpl : public SamplerUDQ {
  public:
    typedef T Value;
    typedef boost::function<void(UDQBase const&)> Report;

  public:
    template<typename R>
    SamplerUDQImpl(boost::any value, R ireport, bool doReport=true, bool aTemplate=false) :
      SampleUDQ(value, aTemplate),
      m_doReport(doReport),
      m_report(ireport) { 
    }
    
    SamplerUDQImpl() : m_doReport(false){}
    
    /* copy constructor
     *  restart sampling and inhibit report from right hand
     */
    SamplerUDQImpl(SamplerUDQImpl const & rh) : 
      SamplerUDQ(rh),
      m_doReport(rh.isTemplate() ? rh.m_doReport : true),
      m_report(rh.m_report)
    {
      if (!rh.isTemplate()) rh.m_doReport=false;
    }

    /* same behaviour as copy-constr
     *
     */
    SamplerUDQImpl & operator=(SamplerUDQImpl const & rh) {
      SamplerUDQ::operator=(rh);
      m_doReport = rh.isTemplate() ? rh.m_doReport : true;
      if (!rh.isTemplate()) rh.m_doReport =false;
      m_report = rh.m_report;
      return *this;
    }


    ~SamplerUDQImpl() {
      if((!empty()) && (!isTemplate()) && m_doReport) m_report(*sample());
    }


  private:
    mutable bool m_doReport;
    Report m_report;
  };


}


#endif // PerfTools_SamplerUDQImpl_H
