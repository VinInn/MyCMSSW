#include "PerfTools/Reporter/interface/SamplerBuilder.h"
#include "PerfTools/Reporter/interface/Sample.h"
#include "PerfTools/Reporter/interface/Sampler.h"

#include "PerfTools/Service/interface/ServiceFactory.h"
#include "PerfTools/Reporter/interface/Reporter.h"


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

  inline void nap(int s=1, int n=0) {
    ::timespec req;
    req.tv_sec = s;
    req.tv_nsec = n;
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
  sources += "WallClock", "CPUClock";
  std::vector<std::string> reporters;
  reporters += "Immediate";
  perftools::Sample s1(builder("check_Timers",sources,reporters));
  reporters[0] = "Summary";
  perftools::Sample s2(builder("check_Timers",sources,reporters));

  {	   
    perftools::Sampler a(s1);
    waiste();
  }
  {	   
    perftools::Sampler a(s1);
    waiste();
    nap();
  }

  for (int i=0;i<10;++i) {
    {	   
      perftools::Sampler a(s2);
      waiste();
    }
    {	   
      perftools::Sampler a(s2);
      waiste();
      nap(0,i*50000);
    }
  }

  perftools::ServiceFactory::get()->getService<perftools::Reporter>("PerfTools:Reporter")->report();

}


