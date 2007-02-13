#include "PerfTools/Sampler/interface/Sampler.h"

#include <cppunit/extensions/HelperMacros.h>


// FIXME
// check by features....
class TestSampler : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestSampler);
  CPPUNIT_TEST(check_Sampler);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown() {}
  void check_Sampler();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSampler);



namespace {

  int more() {
    static int a=0;
    return a++;
  }
  
  int last=0;
  
  void tell(int i){
    last = i;
  }
}

void  TestSampler::check_Sampler() {
  

}
