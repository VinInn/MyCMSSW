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


int main(int argc) {


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


  svc->transaction().start(pool::ITransaction::UPDATE);
  pool::Ref<NavigationTests::AS> as(svc, new NavigationTests::AS);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    as.markWrite(place);
  }
  as->p.t=new NavigationTests::BT;
  as->p.t->k = 3;

  // svc->transaction().start(pool::ITransaction::UPDATE);
  pool::Ref<NavigationTests::T2> o1(svc, new NavigationTests::T2);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o1.markWrite(place);
  }
  pool::Ref<NavigationTests::T2> o2(svc, new NavigationTests::T2);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o2.markWrite(place);
  }
  pool::Ref<NavigationTests::T2> o3(svc, new NavigationTests::T2);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o3.markWrite(place);
  }
  pool::Ref<NavigationTests::T2> o5(svc, new NavigationTests::T2);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o5.markWrite(place);
  }
  pool::Ref<NavigationTests::K> o4(svc, new NavigationTests::K);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "K", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o4.markWrite(place);
  }
  o3->bs.push_back(o1);
  o3->bs.push_back(o2);
  o3->bs.push_back(o5);
  o5->bs.push_back(o2);

  std::cout << "vec size " << o3->bs.size() << std::endl;

  // svc->cacheSvc().resetCache();
  o1.reset();
  std::cout << "o1 " << o1.toString() << std::endl;
  std::cout << "o1 o " << o1.isOpen() <<std:: endl;
  std::cout << "o1 p " << o1.isNull() << std::endl;
  std::cout << "o2 p " << o2.isNull() << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();
  svc->cacheSvc().resetCache();

  svc->transaction().start(pool::ITransaction::UPDATE);
  // std::cout << "o1 p " << o1.isNull() << std::endl;
  std::cout << "o1 " << o1.toString() << std::endl;
  std::cout << "o1 o " << o1.isOpen() <<std:: endl;
  std::cout << "o1 p " << o1.isNull() << std::endl;
  o1.markDelete();  
  o1.reset();
  std::cout << "after delete and reset" << std::endl;
  std::cout << "o1 " << o1.toString() << std::endl;
  std::cout << "o1 o " << o1.isOpen() <<std:: endl;
  std::cout << "o1 p " << o1.isNull() << std::endl;
  std::cout << "o2 d " << o2.toString() << std::endl;
  std::cout << "o2 t " << o2.token() << std::endl;
  std::cout << "o2 p " << o2.isNull() << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();

  svc->transaction().start(pool::ITransaction::UPDATE);
  std::cout << "vec size " << o3->bs.size() << std::endl;
  std::cout << "here we do not die anymore..." << std::endl;
  std::cout << "o2 d " << o2.toString() << std::endl;
  std::cout << "o2 t " << o2.token() << std::endl;
  std::cout << "o2 p " << o2.isNull() << std::endl;
  pool::Ref<NavigationTests::T2> o22 = o2;
  svc->transaction().commit();
  svc->session().disconnectAll();

  svc->cacheSvc().resetCache();

  svc->transaction().start(pool::ITransaction::UPDATE);
  std::cout << "vec size " << o3->bs.size() << std::endl;
  o3->bs.push_back(o5);
  o3.markUpdate();
  std::cout << "vec size " << o3->bs.size() << std::endl;
  svc->transaction().commit();
  if (argc>1) svc->session().disconnectAll();
  svc->cacheSvc().resetCache();
  svc->transaction().start(pool::ITransaction::UPDATE);
  std::cout << "vec size " << o3->bs.size() << std::endl;
  o3->bs.push_back(o5);
  std::cout << "vec size " << o3->bs.size() << std::endl;
  o3.markUpdate();
  svc->transaction().commit();
  svc->session().disconnectAll();

  svc->cacheSvc().resetCache();


  svc->transaction().start(pool::ITransaction::UPDATE);
  std::cout << "o1 d " << o1.toString() << std::endl;
  std::cout << "o1 t " << o1.token() << std::endl;
  std::cout << "o1 p " << o1.isNull() << std::endl;
  std::cout << "o2 d " << o2.toString() << std::endl;
  std::cout << "o2 t " << o2.token() << std::endl;
  std::cout << "o2 p " << o2.isNull() << std::endl;  /// this is true???
  svc->transaction().commit();
  svc->session().disconnectAll();

  try {
    svc->transaction().start(pool::ITransaction::UPDATE);
    std::cout << "o2 d " << o2.toString() << std::endl;
    std::cout << "o2 t " << o2.token() << std::endl;
    std::cout << "o2 p " << o2.isNull() << std::endl;
    o2.markUpdate();
    std::cout << "after update 1" << std::endl;
    o2.markDelete();  
    std::cout << "after delete 1" << std::endl;
    o2.markUpdate(); 
    std::cout << "after update 2" << std::endl;
    o2.markDelete();  
    std::cout << "after delete 2" << std::endl;
    o22.markDelete();  
    std::cout << "after delete 22" << std::endl;
    o3.markDelete();  
    std::cout << "after delete 3" << std::endl;
    pool::Ref<NavigationTests::T2> ol(svc, new NavigationTests::T2);
    {
      pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
      // This will also register the file. For this to occur, the placement object must use a PFN.
      ol.markWrite(place);
  }
    svc->transaction().commit();
    svc->session().disconnectAll();
  }
  catch(const seal::Error& er){
    std::cout << "caught seal exception " << std::endl;
    std::cerr << er.explainSelf();
    std::cerr << std::endl;
    svc->transaction().commit();
    svc->session().disconnectAll();
   }


  svc->transaction().start(pool::ITransaction::UPDATE);
  pool::Ref<NavigationTests::T2> oA(svc,new NavigationTests::T2);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    oA.markWrite(place);
  }
  svc->transaction().commit();
  svc->session().disconnectAll();

  svc->transaction().start(pool::ITransaction::UPDATE);
  oA.markDelete();
  pool::Ref<NavigationTests::T2> oB(svc,new NavigationTests::T2);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    oB.markWrite(place);
  }
  oA = oB;
  svc->transaction().commit();
  svc->session().disconnectAll();
  try {
    svc->transaction().start(pool::ITransaction::UPDATE);
    oA.markDelete();  
    std::cout << "after delete A" << std::endl;
     svc->transaction().commit();
    svc->session().disconnectAll();
  }
  catch(const seal::Error& er){
    std::cout << "caught seal exception " << std::endl;
    std::cerr << er.explainSelf();
    std::cerr << std::endl;
    svc->transaction().commit();
    svc->session().disconnectAll();
   }

  try {
    svc->transaction().start(pool::ITransaction::UPDATE);
    o5.markDelete();  
    std::cout << "after delete 5" << std::endl;
    o3.markDelete();  
    std::cout << "after delete 3 2" << std::endl;
    svc->transaction().commit();
    svc->session().disconnectAll();
  }
  catch(const seal::Error& er){
    std::cout << "OK! caught seal exception " << std::endl;
    std::cerr << er.explainSelf();
    std::cerr << std::endl;
    svc->transaction().commit();
    svc->session().disconnectAll();
   }

  try {
    svc->transaction().start(pool::ITransaction::READ);
    pool::Ref<NavigationTests::T2> h2;
    std::cout << "before is open" <<std:: endl;
    std::cout <<  h2.isOpen() <<std:: endl;
    std::cout << "before reset" <<std:: endl;
    h2.reset();
    svc->transaction().commit();
    svc->session().disconnectAll();
   }
  catch(const seal::Error& er){
    std::cout << "caught seal exception " << std::endl;
    std::cerr << er.explainSelf();
    std::cerr << std::endl;
    svc->transaction().commit();
    svc->session().disconnectAll();
  }


  cat->commit();

  delete svc;
  

}

