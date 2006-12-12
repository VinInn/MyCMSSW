#include "PluginManager/PluginManager.h"
#include "POOLCore/Token.h"
#include "FileCatalog/URIParser.h"
#include "FileCatalog/FCSystemTools.h"
#include "FileCatalog/IFileCatalog.h"

#include "PoolTests/DataFormatTests/interface/OneBranchTree.h"
#include "PoolTests/PolyPerformance/interface/PolyPerformanceStubs.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "VinInn/Test1/test/PolyVector.h"
#include "DataFormats/Common/interface/Wrapper.h"

#include "Utilities/Timing/interface/PentiumTimer.h"


#include<iostream>

typedef  edm::Wrapper<edm::PolyVector<polyPerformance::Base> > PolyWrapper;
typedef  edm::Wrapper<edm::OwnVector<polyPerformance::Base> > OwnWrapper;

namespace {

  struct Maker {
    virtual ~Maker(){}
    virtual edm::EDProduct * make()=0;
    
  };

  template<Wrapper>
  struct WMaker : public {

    typedef  typename Wrapper::value_type Container;
    edm::EDProduct * make() {
      std::auto_ptr<Container> o(new Container);
      populate(*o);
      return new Wrapper(o); 
    }
    
    void populate(Container& cont) {
      // fill with 120 of those
      for (int i=0; i<10;i++) {
	cont.push_back(fill(dummy1));
	cont.push_back(fill(dummy1));
	cont.push_back(fill(dummy2));
	cont.push_back(fill(dummy2));
	cont.push_back(fill(dummy11));
	cont.push_back(fill(dummy11));
	cont.push_back(fill(dummy12));
	cont.push_back(fill(dummy12));
	cont.push_back(fill(dummy21));
	cont.push_back(fill(dummy21));
	cont.push_back(fill(dummy22));
	cont.push_back(fill(dummy22));
      }
    }

    template<typename A> A& fill(A& a) {
      static int c=0;
      int i=c%A::SIZE;
      a.data[i] = i;
      c++;
      return a;
    }

    polyPerformance::Array<int,3> dummy1;
    polyPerformance::Array<int,6> dummy2;
    polyPerformance::Array<float,3> dummy11;
    polyPerformance::Array<float,6> dummy12;
    polyPerformance::Array<double,3> dummy21;
    polyPerformance::Array<double,6> dummy22;


  };

  template<typename A> std::auto_ptr<A> Maker<OwnWrapper>::fill(A& a) {
    static int c=0;
    int i=c%A::SIZE;
    a.data[i] = i;
    c++;
    return  std::auto_ptr<A>(a.clone());
  }



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
      maker(argc >1 ? new WMaker<OwnWrapper>() : 
	    new WMaker<PolyWrapper>());

    std::cout << std::endl;
    
    for (int i=0;i<1000;i++) {
      tree.add((*maker).make());
    }
  
  }
  timer.stop();
  std::cout << "elapsed time " << timer.lap() << std::endl;

  cat.commit();


  //  std::cout << "the end" << std::endl;



  return 0;

}
