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

    std::auto_ptr<Container> makeCont() {
      std::auto_ptr<Container> o(new Container);
      populate(*o);
      return o;
    }

    edm::EDProduct * make() {
      return new Wrapper(makeCont()); 
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


struct addEDM {
  void operator()(OneBranchTreeNaive<edm::EDProduct>& tree, Maker & maker) {
    tree.add(maker.make());
  }
};

template<typename Product, typename Wrapper>
struct addNaive {
  void operator()(OneBranchTreeNaive<Product>& tree, WMaker<Wrapper> & maker) {
    tree.add(maker.makeCont().release());
  }
};

template<typename Product, typename Wrapper, typename Adder>
void go() {
  Adder adder;

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
    OneBranchTreeNaive<Product> tree(&cat);
    std::auto_ptr<WMaker<Wrapper> > 
      maker(new WMaker<Wrapper>());

    std::cout << std::endl;
    
    for (int i=0;i<2000;i++) {
      adder(tree, *maker);
    }
  
  }
  timer.stop();
  std::cout << "elapsed time " << timer.lap() << std::endl;

  cat.commit();


  //  std::cout << "the end" << std::endl;
}

int main(int argc, char * argv[]) {
  bool naive = argc>1 && argv[1][1]=='n';
  bool dv = argc>2;
  
  if (naive) {
    if (dv) 
      go<detVectorPerformance::DVAI10, DVWrapper, addNaive<detVectorPerformance::DVAI10,DVWrapper> >();
    else
      go<detVectorPerformance::IVAI10, IVWrapper, addNaive<detVectorPerformance::IVAI10,IVWrapper> >();
  } else{
   if (dv) 
      go<edm::EDProduct, DVWrapper, addEDM>();
    else
      go<edm::EDProduct, IVWrapper, addEDM>();
  }
      
  return 0;

}
