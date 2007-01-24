#ifndef PerfTools_Sampler_H
#define PerfTools_Sampler_H

#include "PerfTools/Reporter/interface/Sample.h"


#include <boost/function.hpp>
#include <boost/any.hpp>


namespace perftools {

  /* a Sentry
   */
  class Sampler {
  public:
    Sampler(const Sample & sample) : 
      m_sample(sample),
      m_sampler(sample.sampler()){}

    ~Sampler(){}
    
    template<typename Vec>
    bool fill(Vec vec) {
      return true;
    }
    
  private:
    const Sample & m_sample;
    boost::any m_sampler;
    
  private:
    Sampler(const Sampler &rh) :
      m_sample(rh.m_sample) {}
    
    Sampler & operaror =(const Sampler &rh) {
      m_sample = rh.m_sample;
      return *this;
    }
  };
  
}


#endif // PerfTools_Sampler_H
