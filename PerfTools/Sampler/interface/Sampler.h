#ifndef PerfTools_Sampler_H
#define PerfTools_Sampler_H

#include "PerfTools/Sampler/interface/Sample.h"


#include <boost/function.hpp>
#include <boost/any.hpp>


namespace perftools {

  /* a Sentry
   */
  class Sampler {
  public:
    typedef Sample::Payload Payload;
 
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
    Payload m_sampler;
    
  private:
    Sampler(const Sampler &rh) :
      m_sample(rh.m_sample) {}
    
    Sampler & operator=(const Sampler &rh) {
      return *this;
    }
  };
  
}


#endif // PerfTools_Sampler_H
