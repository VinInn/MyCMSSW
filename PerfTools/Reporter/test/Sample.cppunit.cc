#include "PerfTools/Reporter/interface/Sample.h"

#include <cppunit/extensions/HelperMacros.h>


// FIXME
// check by features....
class TestSample : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestSample);
  CPPUNIT_TEST(check_constr);
  CPPUNIT_TEST(check_Sampler);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
  void tearDown() {}
  void check_constr();
  void check_Sampler();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSample);

namespace {

 
  int a=0;

  int what() {
    return a;
  }
  
  int last=0;
  
  void tell(int i){
    last = i;
  }
}


void TestSample::setUp(){
  a=0;
}
 
#include "PerfTools/Reporter/src/SamplerImpl.h"

void TestSample::check_constr() {
  // a "template"
  boost::any ba = perftools::SamplerImpl<int>(&what,&tell,false,true);
  {
    perftools::Sample s1(ba);
    a++;
  }
  CPPUNIT_ASSERT(last==0);

}

void  TestSample::check_Sampler() {
  // a "template"
  boost::any ba = perftools::SamplerImpl<int>(&what,&tell,false,true);
  {
    perftools::Sample s1(ba);
    a++;
    {
      // this start sampling
      boost::any b1 = s1.sampler();
      a++; 
      {
	boost::any b2 = s1.sampler();
	a++;
      }
      CPPUNIT_ASSERT(last==1);
    }
    CPPUNIT_ASSERT(last==2);
    last=0;
  }
  CPPUNIT_ASSERT(last==0);
  
}
