#include "PerfTools/Reporter/src/SamplerImpl.h"

#include <cppunit/extensions/HelperMacros.h>


// FIXME
// check by features....
class TestSamplerI : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestSamplerI);
  CPPUNIT_TEST(check_defconstr);
  CPPUNIT_TEST(check_constr);
  CPPUNIT_TEST(check_constrF);
  CPPUNIT_TEST(check_copy);
  CPPUNIT_TEST(check_assign);
  CPPUNIT_TEST(check_any);
  CPPUNIT_TEST(check_Sampler);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
  void tearDown() {}
  void check_defconstr();
  void check_constr();
  void check_constrF();
  void check_copy();
  void check_assign();
  void check_any();
  void check_Sampler();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSamplerI);



namespace {

 
  int a=0;

  int what() {
    return a;
  }
  
  int last=0;
  
  void tell(int i){
    last = i;
  }
}


void TestSamplerI::setUp(){
  a=0;
  last=0;
}
 

void TestSamplerI::check_defconstr() {
  {
    perftools::SamplerImpl<int>  s;
    CPPUNIT_ASSERT(last==0);
  }
  CPPUNIT_ASSERT(last==0);
}


void TestSamplerI::check_constr() {
  {
    perftools::SamplerImpl<int>  s(&what,&tell);
    a++;
  }
  CPPUNIT_ASSERT(last==1);
}

void TestSamplerI::check_constrF() {
  {
    perftools::SamplerImpl<int>  s(&what,&tell,false);
    a++;
  }
  CPPUNIT_ASSERT(last==0);
}

void TestSamplerI::check_copy() {
  {
    perftools::SamplerImpl<int>  s1(&what,&tell);
    a++;
    CPPUNIT_ASSERT(s1.sample()==1);
    {
      perftools::SamplerImpl<int>  s2(s1);
      CPPUNIT_ASSERT(s2.sample()==0);
    }
    CPPUNIT_ASSERT(last==0);
  }
  CPPUNIT_ASSERT(last==0);  
}

void TestSamplerI::check_assign() {
  {
    perftools::SamplerImpl<int>  s1(&what,&tell);
    a++;
    CPPUNIT_ASSERT(s1.sample()==1);
    {
      perftools::SamplerImpl<int>  s2;
      a++;
      CPPUNIT_ASSERT(s2.sample()==1);
      s2=s1;
      CPPUNIT_ASSERT(s2.sample()==0);
      CPPUNIT_ASSERT(s1.sample()==2);
      a++;
    }
    CPPUNIT_ASSERT(last==1);
  }
  CPPUNIT_ASSERT(last==1);  

}

#include<boost/any.hpp>
void TestSamplerI::check_any() {
  {
    boost::any ba = perftools::SamplerImpl<int>(&what,&tell,false);
    a++;
  }
  CPPUNIT_ASSERT(last==1);
  last=0;
  {
    boost::any ba = perftools::SamplerImpl<int>(&what,&tell,false,true);
    a++;
  }
  CPPUNIT_ASSERT(last==0);
  last=0;
  {
    boost::any ba = perftools::SamplerImpl<int>(&what,&tell,false,true);
    a++;
    {
      boost::any bb = ba;
      a++;
    }
    CPPUNIT_ASSERT(last==1);
  }
  last=0;
  {
    boost::any ba = perftools::SamplerImpl<int>(&what,&tell,false,true);
    // now is a false, false
    a++;
    {
      // still false,false
      boost::any bb; bb.swap(ba);
      a++;
      { 
	// now is a true, false (start counting)
	boost::any bc = ba;
	a++;
      }
      CPPUNIT_ASSERT(last==1);
      last=0;
    }
    CPPUNIT_ASSERT(last==0);
    last=0;
  }
  CPPUNIT_ASSERT(last==0);
}
 

void  TestSamplerI::check_Sampler() {
  
  {
    perftools::SamplerImpl<int>  s(&what,&tell);
    
    a++;
    CPPUNIT_ASSERT(s.sample()==1);
    
    a++;
    CPPUNIT_ASSERT(s.sample()==2);
    
    CPPUNIT_ASSERT(last==0);
    a++;
  }    
  
  CPPUNIT_ASSERT(last==3);
  
  {
    perftools::SamplerImpl<int>  s(&what,&tell);
    a++;
    CPPUNIT_ASSERT(last==3);
  }
  
  CPPUNIT_ASSERT(last==1);

}
