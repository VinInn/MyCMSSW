#include "PerfTools/Reporter/src/SamplerImpl.h"

#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include <boost/bind.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
using namespace boost::assign;


// FIXME
// check by features....
class TestSamplerIV : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestSamplerIV);
  CPPUNIT_TEST(check_defconstr);
  CPPUNIT_TEST(check_constr);
  CPPUNIT_TEST(check_constrF);
  CPPUNIT_TEST(check_copy);
  CPPUNIT_TEST(check_assign);
  CPPUNIT_TEST(check_any);
  CPPUNIT_TEST(check_any_copy);
  CPPUNIT_TEST(check_any_swap);
  CPPUNIT_TEST(check_Sampler);
  CPPUNIT_TEST_SUITE_END();
public:
  TestSamplerIV();
  void setUp();
  void tearDown() {}
  void check_defconstr();
  void check_constr();
  void check_constrF();
  void check_copy();
  void check_assign();
  void check_any();
  void check_any_copy();
  void check_any_swap();
  void check_Sampler();

  std::vector<int> zero;
  std::vector<int> one;
  std::vector<int> oneTwoThree;

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSamplerIV);



namespace {

 
  std::vector<int> a(3,0);

  int what(int i) {
    return a[i];
  }

  typedef boost::function<int(void)> Fu;

  std::vector<Fu> whatV = boost::assign::list_of(boost::bind(what,0))(boost::bind(what,1))(boost::bind(what,2));  
 
  


  std::vector<int> last(3,0);
  
  void tell(std::vector<int> i){
    last = i;
  }
}

TestSamplerIV::TestSamplerIV(): zero(3,0), one(3,1) {
  oneTwoThree +=1,2,3;
}


void TestSamplerIV::setUp(){
  {  std::vector<int> l(3,0);
    a.swap(l);
  }
  {
    std::vector<int> l(3,0);
    last.swap(l);
  }
}
 

void TestSamplerIV::check_defconstr() {
  {
    perftools::SamplerImplVec<int>  s;
    CPPUNIT_ASSERT(last==zero);
  }
  CPPUNIT_ASSERT(last==zero);
}


void TestSamplerIV::check_constr() {
  {
    perftools::SamplerImplVec<int>  s(whatV.begin(),whatV.end(),&tell);
    CPPUNIT_ASSERT(s.sample()==zero);
    a=oneTwoThree;
  }
  CPPUNIT_ASSERT(last==oneTwoThree);
}

void TestSamplerIV::check_constrF() {
  {
    perftools::SamplerImplVec<int>  s(whatV.begin(),whatV.end(),&tell,false);
    a=oneTwoThree;
  }
  CPPUNIT_ASSERT(last==zero);
}

void TestSamplerIV::check_copy() {
  {
    perftools::SamplerImplVec<int>  s1(whatV.begin(),whatV.end(),&tell);
    a=oneTwoThree;
    CPPUNIT_ASSERT(s1.sample()==oneTwoThree);
    {
      perftools::SamplerImplVec<int>  s2(s1);
      CPPUNIT_ASSERT(s2.sample()==zero);
    }
    CPPUNIT_ASSERT(last==zero);
  }
  CPPUNIT_ASSERT(last==zero);  
}

void TestSamplerIV::check_assign() {
  {
    perftools::SamplerImplVec<int>  s1(whatV.begin(),whatV.end(),&tell);
    a=oneTwoThree;
    CPPUNIT_ASSERT(s1.sample()==oneTwoThree);
    {
      perftools::SamplerImplVec<int>  s2;
      std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());
      s2=s1; // s1 will not report anymore
      CPPUNIT_ASSERT(s2.sample()==zero);
      CPPUNIT_ASSERT(s1.sample()==a);
      std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());     
    }
    CPPUNIT_ASSERT(last==one);
  }
  CPPUNIT_ASSERT(last==one);  
}

#include<boost/any.hpp>
void TestSamplerIV::check_any() {
  {
    boost::any ba = perftools::SamplerImplVec<int>(whatV.begin(),whatV.end(),&tell,false);
    a=oneTwoThree;
  }
  CPPUNIT_ASSERT(last==oneTwoThree);
  last=zero;
  {
    boost::any ba = perftools::SamplerImplVec<int>(whatV.begin(),whatV.end(),&tell,false,true);
    std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());
  }
  CPPUNIT_ASSERT(last==zero);
}

void TestSamplerIV::check_any_copy() {

  {
    boost::any ba = perftools::SamplerImplVec<int>(whatV.begin(),whatV.end(),&tell,false,true);
    std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());
    {
      boost::any bb = ba;
      std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());
    }
    CPPUNIT_ASSERT(last==one);
  }
}

void TestSamplerIV::check_any_swap() {
  {
    boost::any ba = perftools::SamplerImplVec<int>(whatV.begin(),whatV.end(),&tell,false,true);
    // now is a false, false
    std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());
    {
      // still false,false
      boost::any bb; bb.swap(ba);
      std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());
      { 
	// now is a true, false (start counting)
	boost::any bc = bb;
	std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());
      }
      CPPUNIT_ASSERT(last==one);
      last=zero;
    }
    CPPUNIT_ASSERT(last==zero);
    last=zero;
  }
  CPPUNIT_ASSERT(last==zero);
}
 

void  TestSamplerIV::check_Sampler() {
  
  {
    perftools::SamplerImplVec<int>  s(whatV.begin(),whatV.end(),&tell);
    
    a=oneTwoThree;
    CPPUNIT_ASSERT(s.sample()==oneTwoThree);
    
    std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());

    CPPUNIT_ASSERT(s.sample()==a);
    
    CPPUNIT_ASSERT(last==zero);
    std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());

  }    
  
  CPPUNIT_ASSERT(last==a);
  
  {
    perftools::SamplerImplVec<int>  s(whatV.begin(),whatV.end(),&tell);
    std::transform(a.begin(),a.end(),one.begin(),a.begin(),std::plus<int>());
  }
  
  CPPUNIT_ASSERT(last==one);

}
