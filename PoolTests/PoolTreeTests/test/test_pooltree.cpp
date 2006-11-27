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

#include "PoolTests/PoolTreeTests/interface/MyData.h"


class Tree1 {
public:

  explicit Tree1(pool::IFileCatalog * cat) :    
    svc(pool::DataSvcFactory::instance(cat)),
    placeTag("T1.root", pool::DatabaseSpecification::PFN, "Ev(Tag)", ROOT::Reflex::Type(), 
	    pool::ROOTTREE_StorageType.type()), 
    placeTr("T1.root", pool::DatabaseSpecification::PFN, "Ev(Tr)", ROOT::Reflex::Type(),
            pool::ROOTTREE_StorageType.type()),     
    placeVx("T1.root", 
pool::DatabaseSpecification::PFN, "Ev(Vx)", ROOT::Reflex::Type(), 
	    pool::ROOTTREE_StorageType.type()),
    tag(svc), tr(svc), vx(svc)
  {
    // Define the policy for the implicit file handling
    pool::DatabaseConnectionPolicy policy;
    policy.setWriteModeForNonExisting(pool::DatabaseConnectionPolicy::CREATE);
    // policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::OVERWRITE);
    policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::UPDATE);
    svc->session().setDefaultConnectionPolicy(policy);
    
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    
  }
  
  ~Tree1() {
    svc->transaction().commit();
    svc->session().disconnectAll();
    std::cout << "cache size at end " << svc->cacheSvc().cacheSize() << std::endl;
    delete svc;
  }

  void loop(int i) {

     svc->transaction().start(pool::ITransaction::UPDATE);
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    
    // pool::Ref<MyData::Tr> tr(svc, new MyData::Tr);
    // pool::Ref<MyData::Vx> vx(svc, new MyData::Vx);
    
    tag = new MyData::Tag;
    tr = new MyData::Tr;
    vx = new MyData::Vx;

    if (i%3==0) (*tag).name = "hello";
    
    tag.markWrite(placeTag);
    tr.markWrite(placeTr);
    vx.markWrite(placeVx);
    
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    svc->transaction().commitAndHold();
    
    if (i%4==0) {
      std::cout << "cache size before commit " << svc->cacheSvc().cacheSize() << std::endl;
      svc->transaction().commit();
       std::cout << "cache size after commit " << svc->cacheSvc().cacheSize() << std::endl;
    }
  }
  

private:
  
  pool::IDataSvc *svc;
  
  pool::Placement placeTag;  
  pool::Placement placeTr;
  pool::Placement placeVx;
  pool::Ref<MyData::Tag> tag;
  pool::Ref<MyData::Tr> tr;
  pool::Ref<MyData::Vx> vx;


};

class Tree2 {
public:

  explicit Tree2(pool::IFileCatalog * cat) :    
    svc(pool::DataSvcFactory::instance(cat)),
    placeTr("T2.root", pool::DatabaseSpecification::PFN, "Ev(Tr)", ROOT::Reflex::Type(), 
	    pool::ROOTTREE_StorageType.type()), 
    placeVx("T2.root", pool::DatabaseSpecification::PFN, "Ev(Vx)", ROOT::Reflex::Type(), 
	    pool::ROOTTREE_StorageType.type()),
    tr(svc), vx(svc)
  {
    // Define the policy for the implicit file handling
    pool::DatabaseConnectionPolicy policy;
    policy.setWriteModeForNonExisting(pool::DatabaseConnectionPolicy::CREATE);
    // policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::OVERWRITE);
    policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::UPDATE);
    svc->session().setDefaultConnectionPolicy(policy);
    
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    
  }
  
  ~Tree2() {
    svc->transaction().commit();
    svc->session().disconnectAll();
    std::cout << "cache size at end " << svc->cacheSvc().cacheSize() << std::endl;
    delete svc;
  }
  
  void loop(int i) {
    
    svc->transaction().start(pool::ITransaction::UPDATE);
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    
    // pool::Ref<MyData::Tr> tr(svc, new MyData::Tr);
    // pool::Ref<MyData::Vx> vx(svc, new MyData::Vx);
    
    tr = new MyData::Tr;
    vx = new MyData::Vx;
    
    tr.markWrite(placeTr);
    vx.markWrite(placeVx);
    
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    //    svc->transaction().commitAndHold();
    
    if (i%7==0) {
      std::cout << "cache size before commit " << svc->cacheSvc().cacheSize() << std::endl;
      svc->transaction().commit();
      std::cout << "cache size after commit " << svc->cacheSvc().cacheSize() << std::endl;
    }
  }
  
  
private:
  
  pool::IDataSvc *svc;
  
  pool::Placement placeTr;
  pool::Placement placeVx;
  pool::Ref<MyData::Tr> tr;
  pool::Ref<MyData::Vx> vx;
  
  
};


int main(int argc, char * argv[]) {


  seal::PluginManager::get()->initialise();

  pool::URIParser p;
  p.parse();
  
  pool::IFileCatalog lcat;
  pool::IFileCatalog * cat = &lcat;
  cat->setWriteCatalog(p.contactstring());
 
  cat->connect();
  cat->start();
  Tree1 t1(cat);
  Tree2 t2(cat);

 
  for (int i=0;i<50;i++) {
    t1.loop(i);
    t2.loop(i);

  }
  

  cat->commit();

  std::cout << "the end" << std::endl;


  return 0;
}
