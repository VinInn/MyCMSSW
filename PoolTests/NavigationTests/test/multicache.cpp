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

#include<iostream>
#include<iomanip>


int go(int argc);
int main(int argc) {

  try { return go(argc);}
  catch(const seal::Error& er){
    std::cout << "caught seal exception " << std::endl;
    std::cerr << er.explainSelf();
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
  
  
  pool::IDataSvc *svc1 = pool::DataSvcFactory::instance(cat);
  pool::IDataSvc *svc2 = pool::DataSvcFactory::instance(cat);
  NavigationTests::B::c_dsvc=svc1;
  // Define the policy for the implicit file handling
  pool::DatabaseConnectionPolicy policy;
  policy.setWriteModeForNonExisting(pool::DatabaseConnectionPolicy::CREATE);
  // policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::OVERWRITE);
  policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::UPDATE);
  svc1->session().setDefaultConnectionPolicy(policy);
  svc2->session().setDefaultConnectionPolicy(policy);


  svc1->transaction().start(pool::ITransaction::UPDATE);
  pool::Ref<NavigationTests::CC> c1(svc1, new NavigationTests::CC);
  {
    pool::Placement place("CN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    c1.markWrite(place);
  }
  std::cout << "c1 " << c1.toString() << std::endl;
  svc1->transaction().commit();
  svc1->session().disconnectAll();
  svc1->cacheSvc().resetCache();

  std::cout << "c done" << std::endl;

  svc1->transaction().start(pool::ITransaction::READ);
  svc2->transaction().start(pool::ITransaction::UPDATE);
  pool::Ref<NavigationTests::B> b1(svc2, new NavigationTests::B(4));
  {
    pool::Placement place("BN", pool::DatabaseSpecification::PFN, "A", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    b1.markWrite(place);
  }
  b1->c = c1;
  // b1->c =  pool::Ref<NavigationTests::CC>(svc2,c1.toString());
  std::cout << "b1->c " << b1->c.toString() << std::endl;
  svc2->transaction().commit();
  //  svc2->session().disconnectAll();
  svc2->cacheSvc().resetCache();
  svc1->transaction().commit();
  // svc1->session().disconnectAll();
  svc1->cacheSvc().resetCache();

  std::cout << "b done" << std::endl;

  svc1->transaction().start(pool::ITransaction::READ);
  svc2->transaction().start(pool::ITransaction::READ);

  std::cout << "C cache is " <<  &NavigationTests::B::c_dsvc->cacheSvc() << std::endl;
  // first explicit
  std::cout << "c1 " << c1.toString() << std::endl;
  std::cout << "c1 o " << c1.isOpen() <<std:: endl;
  std::cout << "c1 n " << c1.isNull() << std::endl;
  std::cout << "c1 t " << c1.token() << std::endl;
  std::cout << "c1 p " << &(*c1) << std::endl;
  std::cout << "c1 p " << (*c1).me() << std::endl;
  std::cout << "c1 c " << c1.cacheSvc() << std::endl;
  {
    pool::Ref<NavigationTests::A> c2(*c1.cacheSvc(),&(*c1));
    std::cout << "c2 " << c2.toString() << std::endl;
    std::cout << "c2 p " << &(*c2) << std::endl;
  }
  std::cout << "after c1 p " << &(*c1) << std::endl;
  {
    pool::Ref<NavigationTests::A> c2(svc1,const_cast<NavigationTests::AV*>((*c1).me()));
    std::cout << "c2 " << c2.toString() << std::endl;
    std::cout << "c2 p " << &(*c2) << std::endl;
  }

  {
    pool::Ref<NavigationTests::A> c4(svc2,c1.toString());
    std::cout << "c4 " << c4.toString() << std::endl;
    std::cout << "c4 p " << &(*c4) << std::endl;
  }

  struct Bha {
    Bha(const pool::Ref<NavigationTests::A>& c, const std::string & n) : cm(c){
      std::cout << "bha " << n << " " << cm.toString() << std::endl;
      std::cout << "bha " << n << " p " << &(*cm) << std::endl;
    }
 
    pool::Ref<NavigationTests::A> cm;
  };
  std::cout << "after c1 p " << &(*c1) << std::endl;
  {
    pool::Ref<NavigationTests::A> c3;
    {
      pool::Ref<NavigationTests::AV> c2(svc1,const_cast<NavigationTests::AV*>((*c1).me()));
      Bha aa(c2,"c2");
      c3 = c2;
    }
    std::cout << "after c3 p " << &(*c3) << std::endl;
  }
  std::cout << "after c1 p " << &(*c1) << std::endl;

  // then implicit
  std::cout << "b1->c " << b1->c.toString() << std::endl;
  std::cout << "b1->c o " << b1->c.isOpen() <<std:: endl;
  std::cout << "b1->c n " << b1->c.isNull() << std::endl;
  std::cout << "b1->c t " << b1->c.token() << std::endl;
  std::cout << "b1->c p " << &(*b1->c) << std::endl;
  std::cout << "b1->c c " << b1->c.cacheSvc() << std::endl;

  std::cout << "refs " << std::setiosflags(std::ios::boolalpha)
	    << (c1 == b1->c) << std::endl;

  std::cout << "tokens " << std::setiosflags(std::ios::boolalpha)
	    << (*c1.token() == *b1->c.token()) << std::endl;

  std::cout << "strings " << std::setiosflags(std::ios::boolalpha)
	    << (c1.toString() == b1->c.toString()) << std::endl;

  svc2->transaction().commit();
  svc2->session().disconnectAll();
  svc2->cacheSvc().resetCache();
  svc1->transaction().commit();
  svc1->session().disconnectAll();
  svc1->cacheSvc().resetCache();

  svc1->transaction().start(pool::ITransaction::READ);
  svc2->transaction().start(pool::ITransaction::READ);

  // first implicit
  std::cout << "b1->c " << b1->c.toString() << std::endl;
  std::cout << "b1->c o " << b1->c.isOpen() <<std:: endl;
  std::cout << "b1->c n " << b1->c.isNull() << std::endl;
  std::cout << "b1->c t " << b1->c.token() << std::endl;
  std::cout << "b1->c p " << &(*b1->c) << std::endl;

  // then explicit
  std::cout << "c1 " << c1.toString() << std::endl;
  std::cout << "c1 o " << c1.isOpen() <<std:: endl;
  std::cout << "c1 n " << c1.isNull() << std::endl;
  std::cout << "c1 t " << c1.token() << std::endl;
  std::cout << "c1 p " << &(*c1) << std::endl;

  svc2->transaction().commit();
  //  svc2->session().disconnectAll();
  svc2->cacheSvc().resetCache();
  svc1->transaction().commit();
  // svc1->session().disconnectAll();
  svc1->cacheSvc().resetCache();


  std::cout << "old style loop" << std::endl;
  for (int i=0;i<10;i++) {
    svc1->transaction().start(pool::ITransaction::UPDATE);
    {
      pool::Ref<NavigationTests::A> c3;
      {
	pool::Ref<NavigationTests::AV> c2(svc1,const_cast<NavigationTests::AV*>((*c1).me()));
	Bha aa(c2,"c2");
	c3 = c2;
      }
      {
	pool::Ref<NavigationTests::AV> cc;
	cc.castDynamic(c3); 
	std::cout << "after cc p " << &(*cc) << std::endl;
      }
      
      std::cout << "after c3 p " << &(*c3) << std::endl;
    }

    pool::Ref<NavigationTests::B> bn(svc1, new NavigationTests::B(4));
    {
      pool::Placement place("BN", pool::DatabaseSpecification::PFN, "B", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
      // This will also register the file. For this to occur, the placement object must use a PFN.
      bn.markWrite(place);
    }
    bn->c = c1;
    
    std::cout << i << " after c1 p " << &(*c1) << " " << (*c1).me() << std::endl;
    svc1->transaction().commit();
    // svc1->session().disconnectAll();
    svc1->cacheSvc().resetCache();
    
  }



  svc1->transaction().start(pool::ITransaction::READ);

  std::cout << "new style loop" << std::endl;
  for (int i=0;i<10;i++) {
    svc2->transaction().start(pool::ITransaction::UPDATE);
    {
      pool::Ref<NavigationTests::A> c3;
      {
	pool::Ref<NavigationTests::AV> c2(svc1,const_cast<NavigationTests::AV*>((*c1).me()));
	Bha aa(c2,"c2");
	c3 = c2;
      }
      std::cout << "after c3 p " << &(*c3) << std::endl;
    }
    
    pool::Ref<NavigationTests::B> bn1(svc2, new NavigationTests::B(4));
    pool::Ref<NavigationTests::B> bn2(svc2, new NavigationTests::B(4));
    {
      pool::Placement place("BN", pool::DatabaseSpecification::PFN, "B", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
      // This will also register the file. For this to occur, the placement object must use a PFN.
      bn1.markWrite(place);
      bn2.markWrite(place);
    }
    bn1->c = c1;
    bn2->c = &(*c1);

    if (i==3 && argc >1) {
      int * k= (int*)(-15) ; std::cout << *k << std::endl;
    }
    
    std::cout << i << " after c1     p " << &(*c1) << " " << (*c1).me() << std::endl;
    std::cout << i << " after bn1->c p " << &(*bn1->c) << " " << bn1->c->me() << std::endl;
    std::cout << i << " after bn2->c p " << &(*bn2->c) << " " << bn2->c->me() << std::endl;
    
    svc2->transaction().commit();
    if ( argc >2) svc2->session().disconnectAll();
    svc2->cacheSvc().resetCache();
    
  }
  svc1->transaction().commit();
  //svc1->session().disconnectAll();
  svc1->cacheSvc().resetCache();
  
  

  cat->commit();

  delete svc2;
  delete svc1;
  

  return 0;

}

