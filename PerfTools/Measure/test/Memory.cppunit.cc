#include "PerfTools/Service/interface/ServiceFactory.h"
#include <boost/function.hpp>

#include <cppunit/extensions/HelperMacros.h>


// FIXME
// I would have preferred check by features....
class TestMemory : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestMemory);
  CPPUNIT_TEST(check_Used);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown() {}
  void check_Used();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestMemory);


void TestMemory::check_Used() {

  typedef boost::function<int(void)> Mem;
  boost::shared_ptr<Mem> c = perftools::ServiceFactory::get()->getService<Mem>("PerfTools:UsedMemory");

  Mem memory= *c;

  int i = memory();
  int j = memory();
  CPPUNIT_ASSERT(!( i<0 ));
  CPPUNIT_ASSERT(i==j);

  {
    int ai = memory();
    int * a = new int(1000);
    int am = memory();
    CPPUNIT_ASSERT(am>ai);
    delete [] a;
    int ad = memory();
    CPPUNIT_ASSERT(ad==ai);
  } 
  {
    int ai = memory();
    int * a = new int(10000000);
    int am = memory();
    CPPUNIT_ASSERT(am>ai);
    delete [] a;
    int ad = memory();
    CPPUNIT_ASSERT(ad==ai);
  }


}
