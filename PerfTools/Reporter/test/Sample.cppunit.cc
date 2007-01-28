#include "PerfTools/Reporter/interface/Sample.h"

#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include <boost/bind.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
using namespace boost::assign;




class TestSample : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestSample);
  CPPUNIT_TEST(check_constr);
  CPPUNIT_TEST(check_Sampler);
  CPPUNIT_TEST_SUITE_END();
public:
  TestSample();
  void setUp();
  void tearDown() {}
  void check_constr();
  void check_Sampler();

  std::vector<int> zero;
  std::vector<int> one;
  std::vector<int> oneTwoThree;


};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSample);

namespace {


  std::vector<int> a(3,0);

  int what(int i) {
    return a[i];
  }

  std::vector<int> last(3,0);

  void tell(int j, int i){
    last[j] = i;
  }
}


void TestSample::setUp(){
  {  std::vector<int> l(3,0);
    a.swap(l);
  }
  {
    std::vector<int> l(3,0);
    last.swap(l);
  }
}
 
#include "PerfTools/Reporter/src/SamplerImpl.h"

void TestSample::check_constr() {
  // a "template"
  Sample::Payload ba(1);
  ba[0] = perftools::SamplerImpl<int>(boost::bind(what,0),boost::bind(tell,0,_2),false,true);
  {
    perftools::Sample s1(ba);
    a[0]++;
  }
  CPPUNIT_ASSERT(last[0]==0);

}

namespace {
  struct Baf {
    Sample::Payload l;
    inline
    Sample::Payload & operator()  {
      l.resize(1);
      return l[0]=perftools::SamplerImpl<int>(boost::bind(what,0),boost::bind(tell,0,_2),false,true);
    }
  };  
}

TestSample::TestSample(): zero(3,0), one(3,1) {
  oneTwoThree +=1,2,3;
}


void  TestSample::check_Sampler() {
  // a "template"
  // boost::any ba = perftools::SamplerImpl<int>(&what,&tell,false,true);
  {
    Baf baf;
    perftools::Sample s1(baf());
    a[0]++;
    {
      // this start sampling
      boost::any b1 = s1.sampler();
      a[0]++; 
      {
	boost::any b2 = s1.sampler();
	a[0]++;
      }
      CPPUNIT_ASSERT(last[0]==1);
    }
    CPPUNIT_ASSERT(last[0]==2);
    last[0]=0;
  }
  CPPUNIT_ASSERT(last[0]==0);
  
}
