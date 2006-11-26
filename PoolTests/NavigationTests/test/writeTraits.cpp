#include "PoolTests/NavigationTests/interface/Traits.h"

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
#include "Collection/Collection.h"

#include <stdexcept>



int go(int argc);
int main(int argc) {

  try { return go(argc);}
  catch(const seal::Error& er){
    std::cout << "caught seal exception " << std::endl;
    std::cerr << er.explainSelf(); std::cerr <<std::endl;
    return 4;
  }
}

int go(int argc){

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
  std::vector<pool::Ref<TT<T1> > > refs;
  
  
  std::string vten[] = { "0","1","2","3","4","5","6","7","8","9","10"};
  std::vector<std::string> ten(vten,vten+11);
  std::vector<std::string>::const_iterator s = ten.begin();
  for (;s!=ten.end();s++) {
    std::string f = std::string("E")+ (*s);
    std::string c = std::string("C")+ (*s);
    
    pool::Ref<TT<T1> > o1(svc, new TT<T1>);
    pool::Placement place(f, pool::DatabaseSpecification::PFN, c, ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o1.markWrite(place);
    refs.push_back(o1);
    
  }

  pool::Ref<TMe > o6(svc, new TMe);
  {
    pool::Placement place("E0", pool::DatabaseSpecification::PFN, "C0", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o6.markWrite(place);
  }  
  pool::Ref<TVoid> o7(svc, new TVoid);
  {
    pool::Placement place("E0", pool::DatabaseSpecification::PFN, "C0", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o7.markWrite(place);
  } 


  
  svc->transaction().commit();
  svc->session().disconnectAll();
  // cat->commit();

  // cat->start();
  svc->transaction().start(pool::ITransaction::UPDATE);

  pool::Ref<TT<T1> > o1(svc, new TT<T1>);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o1.markWrite(place);
  }
  pool::Ref<TT<T2> > o2(svc, new TT<T2>);
  {
    pool::Placement place("EN", pool::DatabaseSpecification::PFN, "N", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o2.markWrite(place);
  }
  //  o1->bs.push_back(o2);
  //  o2->c=o1;

  pool::Collection<TT<T1>  > collection(svc, "ImplicitCollection", "PFN:DN", "L", pool::ICollection::READ)
;
  pool::Collection<TT<T1> >::Iterator iter = collection.select();
  while (iter.next()) {
    std::cout << iter.ref().toString() << std::endl;
    std::cout << (*iter).i << std::endl;
  }


  std::vector<pool::Ref<TT<T1> > >::const_iterator r = refs.begin();
  int n=0;
  for (;r!=refs.end();r++) {
    n++;
    std::cout << r->toString() << std::endl;
    if (n==2||n==8) r->markUpdate();
  }  

  svc->transaction().commit();
  svc->session().disconnectAll();

  svc->cacheSvc().resetCache(); 

  svc->transaction().start(pool::ITransaction::READ);

  std::cout << "o6 " << o6.isNull()
	    << " " << (*o6->me.get()).i <<  std::endl;
  std::cout << "o7 " << o7.isNull()
	    << " " << (*o7->get()).i <<  std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();
  cat->commit();

  delete svc;
  
  return 0;
}
