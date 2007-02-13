#include "PerfTools/Service/interface/ServiceFactory.h"
#include<memory>
#include<typeinfo>
#include "PerfTools/Sampler/interface/SamplerBuilder.h"
#include "PerfTools/Reporter/interface/Reporter.h"


#include <cppunit/extensions/HelperMacros.h>

class TestPlugins : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestPlugins);
  CPPUNIT_TEST(check_Reporter);
  CPPUNIT_TEST(check_SamplerCreator);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown() {}
  void check_Reporter();  
  void check_SamplerCreator();
};


CPPUNIT_TEST_SUITE_REGISTRATION(TestPlugins);


void TestPlugins::check_Reporter() {
  boost::shared_ptr<perftools::Reporter> reporter = 
    perftools::ServiceFactory::get()->getService<perftools::Reporter>("PerfTools:Reporter");
  CPPUNIT_ASSERT(reporter);
}

void TestPlugins::check_SamplerCreator() {
  boost::shared_ptr<perftools::SamplerBuilder> creator = 
    perftools::ServiceFactory::get()->getService<perftools::SamplerBuilder>("PerfTools:SamplerBuilder");

  CPPUNIT_ASSERT(creator);
  std::vector<std::string> dummy;
  CPPUNIT_ASSERT(creator->create("",dummy,dummy));
}
