#include "Reflex/Type.h"
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
#include <map>
#include<utility>
#include<vector>
#include<string>
#include "PoolTests/NavigationTests/interface/ABC.h"
#include <stdexcept>
#include "SealUtil/SealTimer.h"
#include "SealUtil/TimingReport.h"
#include "SealUtil/SealHRRTChrono.h"
#include<sstream>

using namespace NavigationTests;

int main(int argc, char * argv[]) {


  seal::PluginManager::get()->initialise();

  seal::TimingReport timRep;

  pool::URIParser p;
  p.parse();
  
  pool::IFileCatalog lcat;
  pool::IFileCatalog * cat = &lcat;
  cat->setWriteCatalog(p.contactstring());
 
  cat->connect();
  cat->start();
  pool::IDataSvc *svc = pool::DataSvcFactory::instance(cat);
  // Define the policy for the implicit file handling
  pool::DatabaseConnectionPolicy policy;
  policy.setWriteModeForNonExisting(pool::DatabaseConnectionPolicy::CREATE);
  // policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::OVERWRITE);
  policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::UPDATE);
  svc->session().setDefaultConnectionPolicy(policy);
  
  // svc->transaction().start(pool::ITransaction::UPDATE);
  

  std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
  typedef NavigationTests::D<NavigationTests::H> Data;

  if (argc<2) { // write
    for (int t=0;t<10;t++) {
      std::ostringstream oss;
      oss << "write_" << t << std::endl;
      std::ostringstream oss2;
      oss2 << "commit_" << t << std::endl;
      seal::SealTimer a(timRep.item<seal::SealHRRTChrono>(oss.str()));

      svc->transaction().start(pool::ITransaction::UPDATE);
      std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
      for (int i=0;i<1000;i++) {  
	pool::Ref<CC> o1(svc, new CC);
	{
	  pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
	  // This will also register the file. For this to occur, the placement object must use a PFN.
	  o1.markWrite(place);
	}
	std::cout << "oid : " << o1.toString() << std::endl;  
	for (int j=0;j<20;j++) {
	  pool::Ref<Data> d1(svc, new Data);
	  {
	    pool::Placement place("ZN", pool::DatabaseSpecification::PFN, "N", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
	    // This will also register the file. For this to occur, the placement object must use a PFN.
	    d1.markWrite(place);
	  }
	  d1->data.resize(600);
	  o1->bs.push_back(d1);
	}
	o1->bs.resize(40);
	for (int j=0;j<10;j++) {
	  pool::Ref<Data> d1(svc, new Data);
	  {
	    pool::Placement place("WN", pool::DatabaseSpecification::PFN, "N", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
	    // This will also register the file. For this to occur, the placement object must use a PFN.
	    d1.markWrite(place);
	  }
	  d1->data.resize(600);
	  o1->bs.push_back(d1);
	}
      }  
      std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
      {
	seal::SealTimer a(timRep.item<seal::SealHRRTChrono>(oss2.str()));
	svc->transaction().commit();
      }
    }
  }
  else { // read
    svc->transaction().start(pool::ITransaction::READ);
    for (int i=1;i<argc; i++) {
      std::string oid(argv[i]);
      seal::SealTimer a(timRep.item<seal::SealHRRTChrono>(oid));
      pool::Ref<CC> o1(svc,oid);
      std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
      std::cout << "o1 size " << o1->size() << std::endl;
      std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;    
      o1.reset();
      std::cout << "cache size after reset " << svc->cacheSvc().cacheSize() << std::endl;
    }
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    svc->transaction().commit();
  }
   
  svc->session().disconnectAll();
  std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
  svc->cacheSvc().resetCache();
  std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;



  
  svc->session().disconnectAll();
  cat->commit();
  delete svc;

  timRep.dump();

  return 0;
}
