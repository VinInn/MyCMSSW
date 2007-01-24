#ifndef PerfTools_Sample_H
#define PerfTools_Sample_H

#include <boost/function.hpp>
#include <boost/any.hpp>


namespace perftools {


  class Sample {
  public:
    Sample(){}
    // avoid copy of payload...
    explicit Sample(boost::any & isample) {
      m_sampler.swap(isample);
    }

    ~Sample(){}

    template<typename Vec, typename IT>
    void advancedRegistration(Vec const &, IT names_b, IT names_e) {
    }

    inline boost::any const & sampler() const {
      return m_sampler;
    } 

  private:
    Sample(Sample const &) {}
    Sample & operaror =(Sample const &) {
      return *this;
    }

  private:
    boost::any m_sampler;
  };


}


#endif PerfTools_Sample_H
