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
#include "SealBase/Error.h"

#include <typeinfo>

int go(int argc);

int main(int argc) {
  try {
    go(argc);
  }
  catch(const seal::Error& er){
    std::cout << "caught seal exception " 
	      << typeid(er).name() << std::endl;
    std::cerr << er.explainSelf();
    std::cerr << std::endl;
  }
}


const std::string son[] = {
  "[DB=0C8F7D57-27E4-D711-96D6-0040F45CCA94][CNT=L][CLID=3F16F0A9-79D1-4881-CE0B-C271DD84A7F1][TECH=00000201][OID=00000004-00000003]",
  "[DB=36B0DC0B-41E4-D711-8981-0040F45CCA94][CNT=L][CLID=3F16F0A9-79D1-4881-CE0B-C271DD84A7F1][TECH=00000201][OID=00000004-00000099]",
  "[DB=36B0DC0B-41E4-D711-8981-0040F45CCA94][CNT=LL][CLID=3F16F0A9-79D1-4881-CE0B-C271DD84A7F1][TECH=00000201][OID=00000004-00000099]",
  "[DB=36B0DC0B-41E4-D711-8981-0040F45CCA94][CNT=LL][CLID=3F16F0A9-79D1-4882-CE0B-C271DD84A7F1][TECH=00000201][OID=00000004-00000099]"};

int go(int argc) {

  const std::string & so = son[argc-1];
  std::cout << std::endl << "so " << so << std::endl<< std::endl;

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
  pool::Ref<NavigationTests::A> o1(svc, new NavigationTests::A);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o1.markWrite(place);
  }
  pool::Ref<NavigationTests::A> o2(svc, new NavigationTests::A);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o2.markWrite(place);
  }
  o2->previous=o1;
  o1->previous=pool::Ref<NavigationTests::A>(svc, so);

  std::cout << "o1 " << o1.toString() << std::endl;
  std::cout << "o1 p " << o1.isNull() << std::endl;
  std::cout << "o2 " << o2.toString() << std::endl;
  std::cout << "o2 p " << o2.isNull() << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();
  svc->transaction().start(pool::ITransaction::READ);

  std::cout << "o1p " << o1->previous.toString() << std::endl;
  std::cout << "o1p t " << o1->previous.token() << std::endl;
  try {
    std::cout << "o1p p " << o1->previous.isNull() << std::endl;
  }
  catch(const seal::Error& er){
    std::cout << "caught seal exception " 
	      << typeid(er).name() << std::endl;
    std::cout << er.explainSelf();
    std::cout << std::endl;
  }
  std::cout << "o2p " << o2->previous.toString() << std::endl;
  std::cout << "o2p t " << o2->previous.token() << std::endl;
  std::cout << "o2p p " << o2->previous.isNull() << std::endl;

  svc->transaction().commit();
  svc->session().disconnectAll();

  std::cout << "update and delete " << std::endl;
  svc->transaction().start(pool::ITransaction::UPDATE);
  o1.markDelete();
  o2.markUpdate();
  svc->transaction().commit();
  svc->session().disconnectAll();
  svc->transaction().start(pool::ITransaction::READ);
  std::cout << "o1 " << o1.toString() << std::endl;
  std::cout << "o1 p " << o1.isNull() << std::endl;
  std::cout << "o2 " << o2.toString() << std::endl;
  std::cout << "o2 p " << o2.isNull() << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();

  cat->commit();

  delete svc;
  
  return 0;
}
