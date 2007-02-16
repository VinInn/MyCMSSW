#include "PerfTools/Service/interface/ServiceFactory.h"
#include<memory>
#include<typeinfo>
#include "PerfTools/Measure/interface/RealTime.h"
#include <boost/function.hpp>
#include <ctime>

#include <cppunit/extensions/HelperMacros.h>

class TestPlugins : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestPlugins);
  CPPUNIT_TEST(check_CPUClock);
  CPPUNIT_TEST(check_WallClock);
  CPPUNIT_TEST(check_UsedMemory);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown() {}
  void check_CPUClock();  
  void check_WallClock();
}  void check_UsedMemory();
};


CPPUNIT_TEST_SUITE_REGISTRATION(TestPlugins);


void TestPlugins::check_CPUClock() {
  typedef boost::function<long long(void)> Clock;
  boost::shared_ptr<Clock> c = perftools::ServiceFactory::get()->getService<Clock>("PerfTools:CPUClock");
  CPPUNIT_ASSERT(c);
  CPPUNIT_ASSERT((*c)==&std::clock);
  try {
    (*c)();
  }
  catch(...) {
    bool ErrorInInvokingCPUClock=false;
    CPPUNIT_ASSERT(ErrorInInvokingCPUClock);
  }
}

void TestPlugins::check_WallClock() {
  typedef boost::function<long long(void)> Clock;
  boost::shared_ptr<Clock> c = perftools::ServiceFactory::get()->getService<Clock>("PerfTools:WallClock");
  CPPUNIT_ASSERT(c);
  CPPUNIT_ASSERT((*c)==&perftools::realTime);
  try {
    (*c)();
  }
  catch(...) {
    bool ErrorInInvokingWallClock=false;
    CPPUNIT_ASSERT(ErrorInInvokingCPUClock);
  }
}
void TestPlugins::check_UsedMemory() {
  typedef boost::function<int(void)> Mem;
  boost::shared_ptr<Mem> c = perftools::ServiceFactory::get()->getService<Mem>("PerfTools:UsedMemory");
  CPPUNIT_ASSERT(c);
  try {
    (*c)();
  }
  catch(...) {
    bool ErrorInInvokingCPUClock=false;
    CPPUNIT_ASSERT(ErrorInInvokingUsedMemory);
  }

}
