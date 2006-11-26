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
#include "PoolTests/NavigationTests/interface/MyABC.h"
#include <stdexcept>


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
  
  svc->transaction().start(pool::ITransaction::UPDATE);
  std::vector<pool::Ref<MA> > refs;
  
  
  std::string vten[] = { "0","1","2","3","4","5","6","7","8","9","10"};
  std::vector<std::string> ten(vten,vten+11);
  std::vector<std::string>::const_iterator s = ten.begin();
  for (;s!=ten.end();s++) {
    std::string f = std::string("E")+ (*s);
    std::string c = std::string("C")+ (*s);
    
    pool::Ref<MA> o1(svc, new MA);
    pool::Placement place(f, pool::DatabaseSpecification::PFN, c, ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o1.markWrite(place);
    refs.push_back(o1);
    
  }
  
  svc->transaction().commit();
  // svc->session().disconnectAll();
  // cat->commit();

  // cat->start();
  svc->transaction().start(pool::ITransaction::UPDATE);

  pool::Ref<MCC> o1(svc, new MCC);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o1.markWrite(place);
  }
  typedef MD<double> Data;
  pool::Ref<Data> o2(svc, new Data);
  {
    pool::Placement place("EN", pool::DatabaseSpecification::PFN, "N", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o2.markWrite(place);
  }
  o1->bs.push_back(R<MB>());
  o1->bs.back().a=o2.toString();
  o2->c.a=o1.toString();

  std::vector<pool::Ref<MA> >::const_iterator r = refs.begin();
  int n=0;
  for (;r!=refs.end();r++) {
    n++;
    std::cout << r->toString() << std::endl;
    if (n==2||n==8) r->markUpdate();
  }  

  svc->transaction().commit();
  svc->session().disconnectAll();
  
  cat->commit();

  delete svc;
  

}
