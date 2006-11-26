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
#include<fstream>
#include <unistd.h>
#include <cstdlib>
#include "boost/lexical_cast.hpp"
using boost::lexical_cast;

int main(int argc) {

  int pid;
  std::ifstream statf;
  statf.open("/proc/self/stat");
  statf >> pid;
  statf.close();

  std::string dname;
  dname = "ls /proc/";
  dname += lexical_cast<std::string>(pid);
  dname += "/fd";
  ::system(dname.c_str());

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


  NavigationTests::B::c_dsvc = svc;
  
  svc->transaction().start(pool::ITransaction::UPDATE);
  std::vector<pool::Ref<NavigationTests::A> > refs;
  std::vector<pool::Ref<NavigationTests::A> > seqrefs;

  // RFIO_DEBUG=3' in your environment, or is it 'RFIO_TRACE'
  // foreach x ( 0 1 2 3 4 5 6 7 8 9 10 )
  // rfrm /castor/cern.ch/cms/reconstruction/datafiles/testPool/E$x
  std::string dir("./");
  // std::string dir("rfio:/castor/cern.ch/cms/reconstruction/datafiles/testPool/");

  std::string vten[] = { "0","1","2","3","4","5","6","7","8","9","10",
			 "11","12","13","14","15","16","17","18","19","20"};
  std::vector<std::string> ten(vten,vten+21);
  for (int gon=0;gon<10;gon++) {
    std::vector<std::string>::const_iterator s = ten.begin();
    for (;s!=ten.end();s++) {
      std::string f;
      if (argc > 1) 
	f = "E0";
      else
	f = std::string("E")+ (*s);

      std::string c = std::string("C")+ (*s);
      
      pool::Ref<NavigationTests::A> o1(svc, new NavigationTests::A);
      pool::Placement place(dir+f, pool::DatabaseSpecification::PFN, c, ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
      // This will also register the file. For this to occur, the placement object must use a PFN.
      o1.markWrite(place);
      refs.push_back(o1);
      if (gon==0) for (int l=0;l<10;l++) {
	pool::Ref<NavigationTests::A> o2(svc, new NavigationTests::A);
        o2.markWrite(place);
	seqrefs.push_back(o2);	
      }
      
    }
  }
  ::system(dname.c_str());
  svc->transaction().commit();
  svc->session().disconnectAll();
  cat->commit();

  // refs.clear();

  cat->start();
  typedef NavigationTests::D<double> Data;
  std::string s2;
  {
    svc->transaction().start(pool::ITransaction::UPDATE);
    
  
    pool::Ref<NavigationTests::CC> o1(svc, new NavigationTests::CC);
    {
      pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
      // This will also register the file. For this to occur, the placement object must use a PFN.
      o1.markWrite(place);
    }
    pool::Ref<Data> o2(svc, new Data);
    {
      pool::Placement place("EN", pool::DatabaseSpecification::PFN, "N", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
      // This will also register the file. For this to occur, the placement object must use a PFN.
      o2.markWrite(place);
    }
    o1->kk = 5;
    o1->bs.push_back(o2);
    o2->c=o1;
 
    svc->transaction().commit();
    svc->session().disconnectAll();
    cat->commit();
    svc->cacheSvc().resetCache();
    std::cout << "after clear cache size " << svc->cacheSvc().cacheSize() << std::endl;



    
    cat->start();
    svc->transaction().start(pool::ITransaction::UPDATE);

    std::cout << std::endl << "start loop on ref in many files" << std::endl<< std::endl;
    std::vector<pool::Ref<NavigationTests::A> >::const_iterator r = refs.begin();
    int n=0;
    for (;r!=refs.end();r++) {
      n++;
      std::cout << "Object " << n << " " << r->toString() << std::endl;
      if (n==2||n==8) r->markUpdate();
      else r->isNull();
    }  
    
    s2 = o2.toString();
    ::system(dname.c_str());
    svc->transaction().commit();
    svc->session().disconnectAll();

    {
      ::system(dname.c_str());
      svc->transaction().start(pool::ITransaction::UPDATE);
      std::cout << std::endl << "start loop on seq ref in many files" << std::endl<< std::endl;
      std::vector<pool::Ref<NavigationTests::A> >::const_iterator r = seqrefs.begin();
      n=0;
      for (;r!=seqrefs.end();r++) {
	n++;
	std::cout << "Object " << n << " " << r->toString() << std::endl; 
	r->isNull();
      }  
      ::system(dname.c_str());
      svc->transaction().commit();
      svc->session().disconnectAll();
      ::system(dname.c_str()); 
      std::cout << std::endl;
    }
  }

  std::cout << "after DELETE cache size " << svc->cacheSvc().cacheSize() << std::endl;
  svc->transaction().start(pool::ITransaction::READ);
  pool::Ref<Data> o2(svc, s2);
  std::cout << std::endl;
  std::cout << o2.toString() << std::endl;
  std::cout << o2->c.toString() << std::endl;
  std::cout << o2->c->kk << std::endl;
 

  svc->transaction().commit();
  svc->session().disconnectAll();

  cat->commit();

  delete svc;
  
  ::system(dname.c_str());

}
