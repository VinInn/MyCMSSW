#include "PluginManager/PluginManager.h"
#include "POOLCore/Token.h"
#include "FileCatalog/URIParser.h"
#include "FileCatalog/FCSystemTools.h"
#include "FileCatalog/IFileCatalog.h"
#include "StorageSvc/DbType.h"
#include "PersistencySvc/DatabaseConnectionPolicy.h"
#include "PersistencySvc/ISession.h"
#include "PersistencySvc/ITransaction.h"
#include "PersistencySvc/IDatabase.h"
#include "PersistencySvc/Placement.h"
#include "DataSvc/DataSvcFactory.h"
#include "DataSvc/IDataSvc.h"
#include "DataSvc/ICacheSvc.h"
#include "DataSvc/Ref.h"

#include "StorageSvc/DbReflex.h"

#include "Collection/Collection.h"

#include <boost/shared_ptr.hpp>

#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/EventAux.h"

#include "PoolTests/PolyPerformance/interface/DetVectorPerformanceStubs.h"


#include "Utilities/Timing/interface/PentiumTimer.h"


template<typename Obj> 
void act(const Obj&) {
}

template<typename Obj> 
void go(std::string const & fname) {

  try {

    seal::PluginManager::get()->initialise();
    
    pool::DbReflex::setDictionaryAutoLoading( true );
//    pool::DbReflex::allowAutoLoadingFor("DataFormatsEcalRecHit");


    pool::URIParser p;
    p.parse();
    
    pool::IFileCatalog lcat;
    pool::IFileCatalog * cat = &lcat;
    cat->setWriteCatalog(p.contactstring());
    cat->connect();
    cat->start();
    
    
    boost::shared_ptr<pool::IDataSvc> svc(pool::DataSvcFactory::instance(cat));
    svc->transaction().start();


    std::string containerName = "Events(Prod)";
    if (argc>1) url = std::string("PFN:") + fname;


    pool::Collection<Obj> 
      collection(&(*svc),
		 "ImplicitCollection", url, containerName, 
		 pool::ICollection::READ);
    

    PentiumTimer timer; 
    timer.start();
    
    {
      pool::Collection<Obj>::Iterator iter = collection.select();
      int n=0;
      pool::Ref<Obj> aObj;
      while(iter.next()) {
	aObj = iter.ref();
	n++;
	//         aObj = itr;
	if (aObj.token()==0)
	  std::cout << "found invalid object " 
	    // << CMSPool::sprint(aObj) 
	    //                << " of type " << CMSPool::classname(aObj) 
		    << std::endl;
	else
	  act(*aObj);
      }
      std::cout << "It contains " << n << " objects of type "
		<<  typeid(*aObj).name()<< std::endl;
      std::cout << "like " 
		<< aObj.toString() 
		<< std::endl;
      
    }    
    timer.stop();
    std::cout << "elapsed time " << timer.lap() << std::endl;
    
    svc->transaction().commit();
    svc->session().disconnectAll();
    std::cout << "cache size at end " << svc->cacheSvc().cacheSize() << std::endl;

  }
  catch (const std::exception & ce) {
    std::cout << ce.what() << std::endl;
  }


}
  

int main(int argc, char * argv[]) {

  bool naive = argc>1 && argv[1][0]=='n';
  bool dv = argc>2;
  
  std::string fname =  std::string(naive ? "naive_" :"edm_") +  std::string(dv ? "doublevector" : "indexed") +".root";

  std::cout << fname << std::endl;

  if (naive) {
    if (dv) 
      go<detVectorPerformance::DVAI10>(fname);
    else
      go<detVectorPerformance::IVAI10>(fname);
  } else{
    go<edm::EDProduct>(fname);
  }
      
  return 0;

}

