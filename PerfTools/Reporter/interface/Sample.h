#ifndef PerfTools_Sample_H
#define PerfTools_Sample_H

#include <boost/function.hpp>
#include <boost/any.hpp>


namespace perftools {


  class Sample {
  public:
    Sample(){}
    // avoid copy of payload...
    explict Sample( boost_any & isample) {
      m_sampler.swap(isample);
    }
    ~Sample(){}

    template<typename Vec, typename IT>
    void advancedRegistration(Vec const &, IT names_b, IT names_e) {
    }

    inline boost_any const & sampler() const {
      return m_sampler;
    } 

  private:
    Sampler(const Sampler &) {}
    Sampler & operaror =(const Sampler &r) {
      return *this;
    }

  private:
    boost_any m_sampler;
  };


}


#endif PerfTools_Sample_H
