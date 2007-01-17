#ifndef SamplerUseCaseWD_H
#define SamplerUseCaseWD_H
/*
 *  Use case demonstrator with dummys...
 */

#include<vector>
#include<map>
#include<string>

namespace usecaseWD {


  /* a Dummy Sample
   * encapsulate the whole sampling machinary
   *  implements just the interface no backhand...
   */
  class Sample {
  public:
    Sample(){}
    ~Sample(){}
    template<typename Vec, typename IT>
    void advancedRegistration(Vec const &, IT names_b, IT names_e) {
    }
  };

   typedef std::map<std::string, std::string> Config;

 /* a Dummy SampleFactory
   * creates a Sample given a configuration
   * it is implemented as a service
   *  implements just the interface no backhand...
   */
  class SampleFactory {
  public:
    Sample create(std::string const & name,
		  std::string const & reportCategories,
		  std::string const & samplingQuantities) {
      return Sample();
    }
  };

  /* a Dummy Sampler
   *  implements just the interface no backhand...
   */
  class Sampler {
  public:
    Sampler(const Sample & sample) : m_sample(sample){}
    ~Sampler(){}
    template<typename Vec>
      bool fill(Vec vec) {
      return true;
    }
    const Sample & m_sample;
  };

  typedef std::map<std::string, std::string> Config;

  /* a typical Client of the Sampler
   *  it is assumed to be configurable using a system similar to
   *  ParameterSet
   *  The whole Sampler behaviour should configurable from
   *  the client configuration
   * the Client should depend only on the Sampler interface
   * changes of Sampler behaviour shall not require recompilation of Client
   * changes to the payload shall not require changes to Client header
   * it shall be possible to add new sampling without changing Client header
   * even if it may be not optimal
   */
  class Client {
  public:
    Client(const Config& config);

    void act();
    void f1();
    void f2() const;
    void f3();

  private:
    Sample f1sample;
    Sample f2sample;
    Sample f3sample;

  };

  void globalClient();

}


#endif
