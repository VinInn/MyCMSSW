#ifndef PerfTools_Sampler_H
#define PerfTools_Sampler_H

#include "PerfTools/Sampler/interface/Sample.h"
#include "PerfTools/Sampler/interface/SamplerUDQ.h"


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
    
    template<typename UDQ>
    bool fill(UDQ & udq) {
      SamplerUDQ * s = asSamplerUDQ(m_sampler.back());
      if (s) (*s).fill(udq);
      return s;
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
