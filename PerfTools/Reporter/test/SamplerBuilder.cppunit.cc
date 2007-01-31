#include "PerfTools/Reporter/interface/SamplerBuilder.h"
#include "PerfTools/Reporter/interface/Sample.h"
#include "PerfTools/Reporter/interface/Sampler.h"



#include <ctime>
#include <cmath>
#include <typeinfo> 
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
using namespace boost::assign;


#include <cppunit/extensions/HelperMacros.h>



namespace {

  double gcrap=0;
  void waiste() {
    for (double i=1;i<100000;i++)
      gcrap+=std::log(std::sqrt(i));
  }

  inline void nap() {
    ::timespec req;
    req.tv_sec = 1;
    req.tv_nsec = 00000;
    ::timespec rem;
    ::nanosleep(&req,&rem);
  }


}

// FIXME
// I would have preferred check by features....
class TestSamplerBuilder : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestSamplerBuilder);
  CPPUNIT_TEST(check_Timers);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown() {}
  void check_Timers();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSamplerBuilder);


//FIXME this is an integration test!
void TestSamplerBuilder::check_Timers() {
  perftools::SamplerBuilder builder;
  std::vector<std::string> sources;
  sources += "Wall", "CPU";
  std::vector<std::string> reporters;
  perftools::Sample s1(builder(sources,reporters));

  {	   
    perftools::Sampler a(s1);
    waiste();
  }
  {	   
    perftools::Sampler a(s1);
    waiste();
    nap();
  }

}


