#ifndef PerfTools_Sample_H
#define PerfTools_Sample_H

#include "PerfTools/Reporter/interface/SampleBase.h"


#include <boost/function.hpp>
#include <boost/any.hpp>
#include <vector>

namespace perftools {


  class Sample {
  public:
    typedef SampleBase::Payload Payload;
    Sample(){}
    // avoid copy of payload...
    explicit Sample(Payload & isample) {
      m_sampler.swap(isample);
      // FIXME or in the Builder???
      foreach(m_sampler.begin(),m_sampler.end(),&perftools::activate);
    }

    ~Sample(){}

    template<typename Vec, typename IT>
    void advancedRegistration(Vec const &, IT names_b, IT names_e) {
    }

    inline Payload const & sampler() const {
      return m_sampler;
    } 

  private:
    Sample(Sample const &) {}
    Sample & operator=(Sample const &) {
      return *this;
    }

  private:
    Payload m_sampler;
  };


}


#endif // PerfTools_Sample_H
