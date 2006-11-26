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

using namespace NavigationTests;

int main() {


  seal::PluginManager::get()->initialise();
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
  pool::Token* t1;
  pool::Token* t2;
  std::string s2;

  {

    svc->transaction().start(pool::ITransaction::UPDATE);
    pool::Ref<T2> o1(svc, new T2);
    {
      pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
      // This will also register the file. For this to occur, the placement object must use a PFN.
      o1.markWrite(place);
    }
    //    typedef NavigationTests::T1 Data;
    typedef T1  Data;
    pool::Ref<Data> o2(svc, new Data);
    {
      pool::Placement place("EN", pool::DatabaseSpecification::PFN, "N", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
      // This will also register the file. For this to occur, the placement object must use a PFN.
      o2.markWrite(place);
    }
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    
    svc->transaction().commit();
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
  }
  

  std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
  {
     //    typedef NavigationTests::T1 Data;
    typedef T1  Data;
    
    svc->transaction().start(pool::ITransaction::UPDATE);
    pool::Ref<T2> o1(svc, new T2);
    {
      pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
      // This will also register the file. For this to occur, the placement object must use a PFN.
      o1.markWrite(place);
       (t1=const_cast<pool::Token*>(o1.token()))->addRef();
    }
    
    pool::Ref<Data> o2(svc, new Data);
    o2->j=4;
    {
      pool::Placement place("EN", pool::DatabaseSpecification::PFN, "N", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
      // This will also register the file. For this to occur, the placement object must use a PFN.
      o2.markWrite(place);
      (t2=const_cast<pool::Token*>(o2.token()))->addRef();
      s2 =  o2.toString();
    }
    o1->bs.push_back(o2);
    o2->c=o1;
    for (int j=0;j<20;j++) {
      pool::Ref<Data> o22(svc, new Data);
      o22->j=j;
      {
	pool::Placement place("FN", pool::DatabaseSpecification::PFN, "N", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
	// This will also register the file. For this to occur, the placement object must use a PFN.
	o22.markWrite(place);
      }
      o1->bs.push_back(o22);
      o22->c=o1;
      o1->bs.push_back(o22);   // (again...)
    }
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    std::cout << o2.toString() << std::endl;
    std::cout << o2->c.toString() << std::endl;
    // o2.reset(); (make the program crashing)
    svc->transaction().commit();
    svc->session().disconnectAll();
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    svc->cacheSvc().resetCache();
    // o2.reset();

    svc->transaction().start(pool::ITransaction::READ);
    {
      std::cout << "after reset cache size " << svc->cacheSvc().cacheSize() << std::endl;
      std::cout << "o1 open " << o1.isOpen() << std::endl;
      std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
      std::cout << "o1 size " << o1->bs.size() << std::endl;
      std::cout << "o1[5] open " << o1->bs[5].isOpen() << std::endl;
      std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
      pool::Ref<T2> o11(o1);
      std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
      o11.reset();   
      std::cout << "after o11 reset cache size " << svc->cacheSvc().cacheSize() << std::endl;
      std::cout << "o1 open " << o1.isOpen() << std::endl;
      o11->bha();
      std::cout << "o1 open " << o1.isOpen() << std::endl;
      std::cout << "cache size after bha " << svc->cacheSvc().cacheSize() << std::endl;
      
    }
    svc->transaction().commit();
    svc->session().disconnectAll();
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    svc->cacheSvc().resetCache();


    svc->transaction().start(pool::ITransaction::READ);
    std::cout << "after reset cache size " << svc->cacheSvc().cacheSize() << std::endl;
    std::cout << "o2 " << o2.toString() << std::endl;
    std::cout << "o2.c " << o2->c.toString() << std::endl;
    std::cout << "o2.c "<< (o2->c).token() << " " << o1.token() << std::endl;
    std::cout << "o1 " << o1.toString() << std::endl;
    std::cout << "o1 size " << o1->bs.size() << std::endl;
    std::cout << "o1.bs " << o1->bs.back().toString() << std::endl;
    std::cout << "o2.c " << o2->c.toString() << std::endl;
    svc->transaction().commit();
    svc->transaction().start(pool::ITransaction::UPDATE);
    pool::Ref<Data> o3( svc,  o2.toString());
    std::cout << "o2 " << o2.toString() << std::endl;
    std::cout << "o2.c " << o2->c.toString() << std::endl;
    std::cout << "o3 " << o3.toString() << std::endl;
    std::cout << "o3.c " << o3->c.toString() << std::endl;
    svc->transaction().commit();
    svc->cacheSvc().clearCache();
    std::cout << "after clear cache size " << svc->cacheSvc().cacheSize() << std::endl;
    svc->transaction().start(pool::ITransaction::UPDATE);
    pool::Ref<Data> o4( svc,  *t2);
    std::cout << "o4 " << o4.toString() << std::endl;
    std::cout << "o4.j " << o4->j << std::endl;
    std::cout << "o4.c " << (o4->c).toString() << std::endl;

    svc->transaction().commit();
  }

  std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;


  // svc->transaction().commit();


  svc->session().disconnectAll();

  {

    svc->transaction().start(pool::ITransaction::UPDATE);
    pool::Ref<T1 > o2(svc,s2);
    pool::Ref<const T2> o1 = o2->c;

    svc->transaction().commit();
    svc->session().disconnectAll();
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    std::cout << o1.toString() << std::endl;
    std::cout << o2.toString() << std::endl;
    std::cout << o2->c.toString() << std::endl;
    // o2.reset(); (make the program crashing)
    svc->transaction().commit();
    svc->session().disconnectAll();
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    svc->cacheSvc().resetCache();

    // o2.reset();

    svc->transaction().start(pool::ITransaction::UPDATE);
    std::cout << "after reset cache size " << svc->cacheSvc().cacheSize() << std::endl;
    for (int i=0; i<10;i++)
      if (o1.isNull()) std::cout << "o1 null???" << std::endl;

    std::cout << "o2 " << o2.toString() << std::endl;
    std::cout << "o2.c " << o2->c.toString() << std::endl;


    svc->transaction().commit();
    svc->session().disconnectAll();

  }


  {     //    typedef NavigationTests::T1 Data;
    typedef T1  Data;
    
    svc->transaction().start(pool::ITransaction::UPDATE);
    pool::Ref<T2> o1(svc, new T2);
    {
      pool::Placement place("QN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
      // This will also register the file. For this to occur, the placement object must use a PFN.
      o1.markWrite(place);
      (t1=const_cast<pool::Token*>(o1.token()))->addRef();
    }
    
    for (int j=0;j<20;j++) {
      pool::Ref<Data> o22(svc, new Data);
      o22->j=j;
      {
	pool::Placement place("ZN", pool::DatabaseSpecification::PFN, "N", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
	// This will also register the file. For this to occur, the placement object must use a PFN.
	o22.markWrite(place);
      }
      o1->bs.push_back(o22);
      o22->c=o1;
      o1->bs.push_back(o22);   // (again...)
    }
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
  }
  std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();
  std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
  svc->cacheSvc().resetCache();
  std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;



  
  svc->session().disconnectAll();
  cat->commit();

  delete svc;
  

}

