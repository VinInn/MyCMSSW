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
#include "PoolTests/NavigationTests/interface/MyMap.h"
#include <stdexcept>

#define NavigationTests

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
  pool::Ref<MyMap> o1(svc, new MyMap);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o1.markWrite(place);
  }
  svc->transaction().commit();
  svc->session().disconnectAll();
  svc->transaction().start(pool::ITransaction::UPDATE);
  o1.markUpdate();  

  for (int i=1;i<5;i++)
    for (int j=1;j<5;j++) {
      o1->mes.insert(MM(i,j));
      o1->mev.push_back(MM(i,j));
    }
  svc->transaction().commit();
  svc->session().disconnectAll();
  svc->cacheSvc().resetCache();

  svc->transaction().start(pool::ITransaction::READ);
  std::cout << "sizes " <<  o1->mes.size() << " " << o1->mev.size() << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();
  
  svc->transaction().start(pool::ITransaction::UPDATE);
  o1.markUpdate();  
  o1->mes.clear();
  svc->transaction().commit();
  svc->session().disconnectAll();
  svc->cacheSvc().resetCache();

  svc->transaction().start(pool::ITransaction::READ);
  std::cout << "sizes " <<  o1->mes.size() << " " << o1->mev.size() << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();

  svc->transaction().start(pool::ITransaction::UPDATE);
  o1.markDelete();  
  svc->transaction().commit();
  svc->session().disconnectAll();
  svc->cacheSvc().resetCache();
  
  cat->commit();
  
  delete svc;
  
  
}
