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

#include<iostream>

typedef  edm::Wrapper<edm::PolyVector<polyPerformance::Base> > TheWrapper;

namespace {

  struct Maker {

    typedef  TheWrapper::value_type Container;
    edm::EDProduct * make() {
      std::auto_ptr<Container> o(new Container);
      populate(*o);
      return new TheWrapper(o); 
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

}

int main(int argc, char * argv[]) {


  seal::PluginManager::get()->initialise();

  pool::URIParser p;
  p.parse();
  
  pool::IFileCatalog cat;
  cat.setWriteCatalog(p.contactstring());
 
  cat.connect();
  cat.start();
  OneBranchTree tree(&cat);
  Maker maker;
  std::cout << std::endl;
 
  for (int i=0;i<50;i++) {
    tree.add(maker.make());
  }
  

  cat.commit();

  //  std::cout << "the end" << std::endl;



  return 0;

}
