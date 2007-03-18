#include "PluginManager/PluginManager.h"
#include "POOLCore/Token.h"
#include "FileCatalog/URIParser.h"
#include "FileCatalog/FCSystemTools.h"
#include "FileCatalog/IFileCatalog.h"

#include "PoolTests/DataFormatTests/interface/OneBranchTree.h"
#include "PoolTests/PolyPerformance/interface/DetVectorPerformanceStubs.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "VinInn/Test1/test/PolyVector.h"
#include "DataFormats/Common/interface/Wrapper.h"

#include "Utilities/Timing/interface/PentiumTimer.h"


#include<iostream>

typedef  edm::Wrapper<detVectorPerformance::DVAI10> DVWrapper;
typedef  edm::Wrapper<detVectorPerformance::IVAI10> IVWrapper;

namespace {

  struct Maker {
    virtual ~Maker(){}
    virtual edm::EDProduct * make()=0;
  };
  
  
  
  template<typename A> 
  inline void fill(std::vector<A>& cont) {
    static int c=0;
    int i=c%A::SIZE;
    int j=c%cont.size();
    cont[j].data[i] = i;
    c++;
  }
  
  template<typename Wrapper>
  struct WMaker : public Maker {
    
    typedef typename Wrapper::value_type Container;
    
    WMaker() : dvec(30) {} 

    edm::EDProduct * make() {
      std::auto_ptr<Container> o(new Container);
      populate(*o);
      return new Wrapper(o); 
    }
    
    void populate(Container& cont) {
      // fill with 6000 of those (200x30)
      for (int i=0; i<200;i++) {
	fill(dvec);
	cont.insert(i,dvec);
      }
    }
 
    std::vector<detVectorPerformance::AI10> dvec;

  };




}

int main(int argc, char * argv[]) {


  seal::PluginManager::get()->initialise();

  pool::URIParser p;
  p.parse();
  
  pool::IFileCatalog cat;
  cat.setWriteCatalog(p.contactstring());
 
  cat.connect();
  cat.start();

  PentiumTimer timer; 
  timer.start();
  
  {
    
    OneBranchTree tree(&cat);
    std::auto_ptr<Maker> 
      maker(argc >1 ? (Maker*)(new WMaker<DVWrapper>()) : 
	    (Maker*)(new WMaker<IVWrapper>()));

    std::cout << std::endl;
    
    for (int i=0;i<2000;i++) {
      tree.add((*maker).make());
    }
  
  }
  timer.stop();
  std::cout << "elapsed time " << timer.lap() << std::endl;

  cat.commit();


  //  std::cout << "the end" << std::endl;



  return 0;

}
