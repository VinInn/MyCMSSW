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


  template<typename A> 
  inline void fill(PolyWrapper::value_type& cont, A& a) {
    static int c=0;
    int i=c%A::SIZE;
    a.data[i] = i;
    c++;
    cont.push_back(a);
  }

  template<typename A> 
  inline void fill(OwnWrapper::value_type& cont, A& a) {
    static int c=0;
    int i=c%A::SIZE;
    a.data[i] = i;
    c++;
    cont.push_back(a.clone());
  }

  template<typename Wrapper>
  struct WMaker : public Maker {

    typedef typename Wrapper::value_type Container;
    
    edm::EDProduct * make() {
      std::auto_ptr<Container> o(new Container);
      populate(*o);
      return new Wrapper(o); 
    }
    
    void populate(Container& cont) {
      // fill with 240 of those
      for (int i=0; i<20;i++) {
	fill(cont,dummy1);
	fill(cont,dummy1);
	fill(cont,dummy2);
	fill(cont,dummy2);
	fill(cont,dummy11);
	fill(cont,dummy11);
	fill(cont,dummy12);
	fill(cont,dummy12);
	fill(cont,dummy21);
	fill(cont,dummy21);
	fill(cont,dummy22);
	fill(cont,dummy22);
      }
    }
 
    polyPerformance::Array<int,3> dummy1;
    polyPerformance::Array<int,6> dummy2;
    polyPerformance::Array<float,3> dummy11;
    polyPerformance::Array<float,6> dummy12;
    polyPerformance::Array<double,3> dummy21;
    polyPerformance::Array<double,6> dummy22;


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
      maker(argc >1 ? (Maker*)(new WMaker<OwnWrapper>()) : 
	    (Maker*)(new WMaker<PolyWrapper>()));

    std::cout << std::endl;
    
    for (int i=0;i<10000;i++) {
      tree.add((*maker).make());
    }
  
  }
  timer.stop();
  std::cout << "elapsed time " << timer.lap() << std::endl;

  cat.commit();


  //  std::cout << "the end" << std::endl;



  return 0;

}
