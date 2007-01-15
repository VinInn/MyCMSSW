#include "Utilities/PerfTools/interface/FakeCPP.h"

#include "Utilities/PerfTools/interface/Sampler.h"


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

int main() {
  
  cppUnit::Dump a;
  {
    perftools::Sampler<int>  s(&more,&tell);
    
    CPPUNIT_ASSERT(s.sample()==1);
    
    
    CPPUNIT_ASSERT(s.sample()==2);

    CPPUNIT_ASSERT(last==0);

  }    
  
  CPPUNIT_ASSERT(last==3);

  {
    perftools::Sampler<int>  s(&more,&tell);
    CPPUNIT_ASSERT(last==3);
  }

  CPPUNIT_ASSERT(last==1);
 
  return 0;
}
