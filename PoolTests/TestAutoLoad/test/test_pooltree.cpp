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

#include "Utilities/Timing/interface/PentiumTimer.h"


    // typedef edm::EventAux Obj;
    typedef  edm::EDProduct Obj;

void act(const Obj&) {
}

int main(int argc, char * argv[]) {

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
/*
    std::string oid = 
"[DB=F038EB17-6636-DB11-B7E4-00E08151E907][CNT=Events(EcalRecHitsSorted_ecalRecHitMaker__RECO.)][CLID=4B9F6001-FA5E-FFD1-F620-4DC0EEF423AE][TECH=00000202][OID=00000023-000003E7]";

    
    pool::Ref<edm::EDProduct> ref(&(*svc),oid);
    std::cout << "object "
              << ref.toString()
              << "\nof type "<<  typeid(*ref).name() << std::endl;

*/


    std::string url = "PFN:Events.root";
    std::string containerName = "Events(Prod)";
    if (argc>1) url = std::string("PFN:") + argv[1];
    if (argc>2) containerName std::string("Events(") + argv[2] + ".)";


    //    std::string url = "FID:F038EB17-6636-DB11-B7E4-00E08151E907";
    //    std::string url = "PFN:/afs/cern.ch/user/i/innocent/scratch0/h2.00029004.combined.OutServ_0.0-cmsswreco.root";
    // std::string containerName = "Events(EcalRecHitsSorted_ecalRecHitMaker__RECO.)";
    // std::string containerName = "Events(EventAux)";
 
    pool::Collection<Obj> 
      collection(&(*svc),
		 "ImplicitCollection", url, containerName, 
		 pool::ICollection::READ);
    
  //  std::string oid =  
//"[DB=F038EB17-6636-DB11-B7E4-00E08151E907][CNT=Events(EcalRecHitsSorted_ecalRecHitMaker__RECO.)]
// [CLID=4B9F6001-FA5E-FFD1-F620-4DC0EEF423AE][TECH=00000202][OID=00000023-000003E7]";

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

    return 0;
  }
  


