#include "PerfTools/Service/interface/ServiceFactory.h"
#include<memory>

#include <cppunit/extensions/HelperMacros.h>


// FIXME
// check by features....
class TestServiceFactory : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestServiceFactory);
  CPPUNIT_TEST(check_Maker);
  CPPUNIT_TEST(check_getAny);
  CPPUNIT_TEST(check_getService);
  CPPUNIT_TEST(check_NameError);
  CPPUNIT_TEST(check_TypeError);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown() {}
  void check_Maker();
  void check_getAny();
  void check_getService();
  void check_NameError();
  void check_TypeError();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestServiceFactory);

namespace perftools{

  namespace serviceTest {
    struct Dummy1{};
    struct Dummy2{};

  }
  

}


void TestServiceFactory::check_Maker() {
  typedef perftools::serviceTest::Dummy2 D2;
  std::auto_ptr<perftools::BaseServiceMaker> m(new perftools::ServiceMaker<D2>);
  
  boost::any a = (*m)();
  CPPUNIT_ASSERT(!a.empty());
  try {
    boost::shared_ptr<D2> d2 = boost::any_cast<boost::shared_ptr<D2> >(a);
    CPPUNIT_ASSERT(d2);
  } catch (boost::bad_any_cast const &) {
    bool badAnyCast = false;
    CPPUNIT_ASSERT(badAnyCast);
  }

}

void TestServiceFactory::check_getAny() {

}
void TestServiceFactory::check_getService() {

}

void TestServiceFactory::check_NameError(){
}

void TestServiceFactory::check_TypeError() {
}
