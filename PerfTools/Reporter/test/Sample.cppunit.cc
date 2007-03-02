#include "PerfTools/Sampler/interface/Sample.h"
#include "PerfTools/Sampler/interface/Sampler.h"

#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include <boost/bind.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
using namespace boost::assign;


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



class TestSample : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestSample);
  CPPUNIT_TEST(check_SampleConstr);
  CPPUNIT_TEST(check_Sample);
  CPPUNIT_TEST(check_SampleVec);

  CPPUNIT_TEST(check_SamplerConstr);
  CPPUNIT_TEST(check_SamplerCopy);
  CPPUNIT_TEST(check_SamplerStack);

  CPPUNIT_TEST_SUITE_END();
public:
  TestSample();
  void add1() {
    std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());
  }
  void setUp();
  void tearDown() {}
  void check_SampleConstr();
  void check_Sample();
  void check_SampleVec();
  void check_SamplerConstr();
  void check_SamplerCopy();
  void check_SamplerStack();


  std::vector<int> zero;
  std::vector<int> one;
  std::vector<int> two;
  std::vector<int> oneTwoThree;


};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSample);


void TestSample::setUp(){
  {  std::vector<int> l(3,0);
    a.swap(l);
  }
  {
    std::vector<int> l(3,0);
    last.swap(l);
  }
}


TestSample::TestSample(): zero(3,0), one(3,1), two(3,2) {
  oneTwoThree +=1,2,3;
}


 
#include "PerfTools/Reporter/src/SamplerImpl.h"

void TestSample::check_SampleConstr() {
  // a "template"
  perftools::Sample::Payload ba(1);
  ba[0] = perftools::SamplerImpl<int>(boost::bind(what,0),boost::bind(tell,0,_1),false,true);
  {
    perftools::Sample s1(ba);
    a[0]++;
  }
  CPPUNIT_ASSERT(last[0]==0);

}

namespace {
  struct Baf {
    perftools::Sample::Payload l;
    inline
    perftools::Sample::Payload & operator()()  {
      l.resize(3);
      l[0]=perftools::SamplerImpl<int>(boost::bind(what,0),boost::bind(tell,0,_1),false,true);
      l[1]=perftools::SamplerImpl<int>(boost::bind(what,1),boost::bind(tell,1,_1),false,true);
      l[2]=perftools::SamplerImpl<int>(boost::bind(what,2),boost::bind(tell,2,_1),false,true);
      return l;
    }
  };  
}


void  TestSample::check_Sample() {
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


void  TestSample::check_SampleVec() {
  // a "template"
  // boost::any ba = perftools::SamplerImpl<int>(&what,&tell,false,true);
  {
    Baf baf;
    perftools::Sample s1(baf());
    add1();
    {
      // this start sampling
      boost::any b1 = s1.sampler();
      add1(); 
      {
	boost::any b2 = s1.sampler();
	add1();
      }
      CPPUNIT_ASSERT(last==one);
    }
    CPPUNIT_ASSERT(last[0]==2);
    last=zero;
  }
  CPPUNIT_ASSERT(last==zero);
  
}

void TestSample::check_SamplerConstr() {
  {
    Baf baf;
    perftools::Sample s1(baf());
    a[0]++;
    {
      // this start sampling
      perftools::Sampler b1(s1);
      a[0]++; 
      {
	perftools::Sampler b2(s1);
	a[0]++;
      }
      CPPUNIT_ASSERT(last[0]==1);
    }
    CPPUNIT_ASSERT(last[0]==2);
    last[0]=0;
  }
  CPPUNIT_ASSERT(last[0]==0);  
}


void TestSample::check_SamplerCopy() {
  {
    Baf baf;
    perftools::Sample s1(baf());
    a[0]++;
    {
      // this start sampling
      perftools::Sampler b1(s1);
      a[0]++; 
      {
	perftools::Sampler b2(b1);
	a[0]++;
      }
      CPPUNIT_ASSERT(last[0]==1);
    }
    CPPUNIT_ASSERT(last[0]==1);
    last[0]=0;
  }
  CPPUNIT_ASSERT(last[0]==0); 
}

#include <stack>

void TestSample::check_SamplerStack() {
    Baf baf;
    perftools::Sample s1(baf()); 
    {
      std::stack<perftools::Sampler> b;
      a[0]++;
      // this start sampling
      b.push(s1);
      a[0]++; 
      
      b.push(s1);
      a[0]++;
      
      CPPUNIT_ASSERT(last[0]==0);
      b.pop();
      CPPUNIT_ASSERT(last[0]==1);
      b.pop();
      CPPUNIT_ASSERT(last[0]==2);
      last[0]=0;
    }
    CPPUNIT_ASSERT(last[0]==0); 
}
