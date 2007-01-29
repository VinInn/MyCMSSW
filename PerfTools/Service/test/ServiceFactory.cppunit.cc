#include "PerfTools/Service/interface/ServiceFactory.h"
#include<memory>
#include<typeinfo>

#include <cppunit/extensions/HelperMacros.h>


class TestServiceFactory : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestServiceFactory);
  CPPUNIT_TEST(check_Maker);
  CPPUNIT_TEST(check_create);
  CPPUNIT_TEST(check_getAny);
  CPPUNIT_TEST(check_getService);
  CPPUNIT_TEST(check_getTwice);
  CPPUNIT_TEST(check_getBase);
  CPPUNIT_TEST(check_NameError);
  CPPUNIT_TEST(check_TypeError);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown() {}
  void check_Maker();
  void check_create();
  void check_getAny();
  void check_getService();
  void check_getTwice();
  void check_getBase();
  void check_NameError();
  void check_TypeError();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestServiceFactory);

namespace perftools{

  namespace serviceTest {
    struct Dummy1{};
    struct Dummy{};
    struct Dummy2 : public Dummy{};

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

void TestServiceFactory::check_create() {
  typedef perftools::serviceTest::Dummy2 D2;
  std::auto_ptr<perftools::BaseServiceMaker> m(perftools::ServiceFactory::get()->create("PerfDummy2"));
  CPPUNIT_ASSERT(m.get());
  CPPUNIT_ASSERT(dynamic_cast<perftools::ServiceMaker<D2>*>(m.get()));
}

void TestServiceFactory::check_getAny() {
  typedef perftools::serviceTest::Dummy2 D2;
  boost::any a = perftools::ServiceFactory::get()->getAny("PerfDummy2");
  CPPUNIT_ASSERT(!a.empty());
  try {
    boost::shared_ptr<D2> d2 = boost::any_cast<boost::shared_ptr<D2> >(a);
    CPPUNIT_ASSERT(d2);
  } catch (boost::bad_any_cast const &) {
    bool badAnyCast = false;
    CPPUNIT_ASSERT(badAnyCast);
  }
 
}
void TestServiceFactory::check_getService() {
  typedef perftools::serviceTest::Dummy2 D2;
  boost::shared_ptr<D2> d2 = perftools::ServiceFactory::get()->getService<D2>("PerfDummy2");
  CPPUNIT_ASSERT(d2);
  CPPUNIT_ASSERT(d2.use_count()==2);
}

void TestServiceFactory::check_getTwice() {
  typedef perftools::serviceTest::Dummy2 D2;
  boost::shared_ptr<D2> d2 = perftools::ServiceFactory::get()->getService<D2>("PerfDummy2");
  CPPUNIT_ASSERT(d2);

  typedef perftools::serviceTest::Dummy1 D1;
  boost::shared_ptr<D1> d1 = perftools::ServiceFactory::get()->getService<D1>("PerfDummy1");
  CPPUNIT_ASSERT(d1);

  boost::shared_ptr<D2> d2_2 = perftools::ServiceFactory::get()->getService<D2>("PerfDummy2");
  CPPUNIT_ASSERT(d2_2);
  CPPUNIT_ASSERT(d2_2==d2);
  CPPUNIT_ASSERT(d2.use_count()==3);
}

void TestServiceFactory::check_getBase() {
  typedef perftools::serviceTest::Dummy D2;
  boost::shared_ptr<D2> d2 = perftools::ServiceFactory::get()->getService<D2>("PerfDummy2");
  CPPUNIT_ASSERT(d2);
  CPPUNIT_ASSERT(d2.use_count()==2);
}


void TestServiceFactory::check_NameError(){
  try {
    typedef perftools::serviceTest::Dummy2 D2;
    boost::shared_ptr<D2> d2 = perftools::ServiceFactory::get()->getService<D2>("PerfNone");
    CPPUNIT_ASSERT(!d2);
  }
  catch(...) {
    bool nameError = false;
    CPPUNIT_ASSERT(nameError);
  }
}

void TestServiceFactory::check_TypeError() {
  try {
  typedef perftools::serviceTest::Dummy2 D2;
  boost::shared_ptr<D2> d2 = perftools::ServiceFactory::get()->getService<D2>("PerfDummy1");
  CPPUNIT_ASSERT(!d2);
  }
  catch(...) {
    bool typeError = false;
    CPPUNIT_ASSERT(typeError);
  }
}
