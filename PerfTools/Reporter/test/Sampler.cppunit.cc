#include "PerfTools/Sampler/interface/Sampler.h"

#include <cppunit/extensions/HelperMacros.h>
#include <stack>

// FIXME
// check by features....
class TestSampler : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestSampler);
  CPPUNIT_TEST(check_constr);
  CPPUNIT_TEST(check_copy);
  CPPUNIT_TEST(check_stack);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown() {}
  void check_constr();
  void check_copy();
  void check_stack();

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
