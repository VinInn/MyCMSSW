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
#include "PoolTests/NavigationTests/interface/Fields.h"
#include <stdexcept>
#include<iomanip>

#include "SealUtil/SealTimer.h"
#include "SealUtil/TimingReport.h"
#include "SealUtil/SealHRRTChrono.h"
#include "SealUtil/SealHRChrono.h"

inline void set ( VinTestP::Fields & w) {
  w.PFN = 7245;
  w.CCA = 5;
  w.nonreachable = false;
  w.dirty = false;
  w.valid = true;
  w.global = true;

}


inline std::ostream & operator<<(std::ostream & co, VinTestP::Fields w) {
  return co <<  w.PFN << "," << w.CCA << "," << std::setiosflags(std::ios::boolalpha)
	    << w.nonreachable << "," <<  w.dirty << "," << w.valid  << "," << w.global;
}


int main() {


  std::cout << "fields size " << sizeof(VinTestP::Fields) << std::endl;


  seal::PluginManager::get()->initialise();
  seal::TimingReport timRep;
  {

  seal::SealTimer a(timRep.item<seal::SealHRRTChrono>("fullRT"));
  seal::SealTimer b(timRep.item<seal::SealHRChrono>("full"));

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

  typedef NavigationTests::D<NavigationTests::H> Data;  
  typedef NavigationTests::DP<NavigationTests::H> PData;
  typedef NavigationTests::D<bool> Bools;
  svc->transaction().start(pool::ITransaction::UPDATE);
  pool::Ref<Data> o1(svc, new Data);
  pool::Ref<PData> p1(svc, new PData);
  p1->data.push_back(new NavigationTests::H(1));
  NavigationTests::H * h = new NavigationTests::H(2);
  p1->data.push_back(h);
  p1->ref.push_back(h);

  pool::Ref<Bools> b2(svc, new Bools);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o1.markWrite(place);
    p1.markWrite(place);
  }
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    b2.markWrite(place);
  }

  pool::Ref<NavigationTests::Bits> bits(svc, new NavigationTests::Bits);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    bits.markWrite(place);
  }

  /*
  pool::Ref<VinTestP::Data> field(svc, new VinTestP::Data);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    field.markWrite(place);
  }
  */
  std::auto_ptr<VinTestP::Data> field(new VinTestP::Data);
  field->resize(10);
  set(field->front().w);
  std::cout << "field " << field->front().w << std::endl;

  bits->set(15);
  bits->set(22);
  std::cout << "bits " << *bits << std::endl;
  o1->data.resize(600);
  b2->data.resize(600);
  b2->data[221]=true;
  b2->data[421]=false;
  std::cout << "size " << o1->data.size() << std::endl;
  std::cout << "capacity " << o1->data.capacity() << std::endl;
  std::cout << "b size " << b2->data.size() << std::endl;
  std::cout << "b capacity " << b2->data.capacity() << std::endl;
  std::cout << "p loc" << p1->data[0] << " " << p1->data[1] << " "  << p1->ref[0] << " " << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();
  svc->transaction().start(pool::ITransaction::READ);
  std::cout << "size " << o1->data.size() << std::endl;
  std::cout << "capacity " << o1->data.capacity() << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();
  svc->cacheSvc().resetCache();
  svc->transaction().start(pool::ITransaction::READ);
  std::cout << "field " << field->front().w << std::endl;
  std::cout << "bits " << *bits << std::endl;
  std::cout << "size " << o1->data.size() << std::endl;
  std::cout << "capacity " << o1->data.capacity() << std::endl;
  std::cout << "b size " << b2->data.size() << std::endl;
  std::cout << "b capacity " << b2->data.capacity() << std::endl;
  std::cout << "p loc" << p1->data[0] << " " << p1->data[1] << " "  << p1->ref[0] << " " << std::endl;
  if (!b2->data[221]) std::cout << "problem with true 221"<< std::endl;
  if (b2->data[421]) std::cout << "problem with false 421"<< std::endl;
  if ( p1->data[1]!=p1->ref[0]) std::cout << "problem with pointers" << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();

  svc->transaction().start(pool::ITransaction::READ);
  std::cout << "o1 d " << o1.toString() << std::endl;
  std::cout << "o1 t " << o1.token() << std::endl;
  std::cout << "o1 p " << o1.isNull() << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();
  svc->cacheSvc().resetCache();

  svc->transaction().start(pool::ITransaction::UPDATE);
  NavigationTests::H::verbose = true;
  pool::Ref<Data> o2(svc, new Data);
  {
    pool::Placement place("DN", pool::DatabaseSpecification::PFN, "L", ROOT::Reflex::Type(), pool::ROOTKEY_StorageType.type()); 
    // This will also register the file. For this to occur, the placement object must use a PFN.
    o2.markWrite(place);
  }
  static NavigationTests::H protoH(-999);
  o2->data.resize(2,protoH);
  o2->data[0].i=1;
  o2->data[1].i=2;
  // o2->data.push_back(NavigationTests::H(1));
  // o2->data.push_back(NavigationTests::H(2));
  svc->transaction().commit();
  svc->session().disconnectAll();
  svc->cacheSvc().resetCache();
  std::cout << "after reset cache size " << svc->cacheSvc().cacheSize() << std::endl;
  svc->transaction().start(pool::ITransaction::READ);
  std::cout << "size " << o2->data.size() << std::endl;
  std::cout << "capacity " << o2->data.capacity() << std::endl;
  std::cout << " a small test of reset..." << std::endl;
  o2.reset();      
  std::cout << "after reset cache size " << svc->cacheSvc().cacheSize() << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();
  std::cout << " a small test of reset..." << std::endl;
  svc->transaction().start(pool::ITransaction::READ);
  {
    pool::Ref<Data> o3(o2);
    std::cout << "size " << o3->data.size() << std::endl;
    /*
      std::string s2=o2.toString();
      o2 = pool::Ref<Data>(*o2.cacheSvc());
      o2.fromString(s2);
    */
    std::string s2=o2.toString();
    o2.clear();
    std::cout << "o2 open? " << o2.isOpen() << std::endl;
    std::cout << "after closing o2 cache size " << svc->cacheSvc().cacheSize() << std::endl;
  }    
  std::cout << "after delete o3 cache size " << svc->cacheSvc().cacheSize() << std::endl;
  svc->transaction().commit();
  svc->session().disconnectAll();


  NavigationTests::H::verbose = false;
  cat->commit();

  delete svc;

  }  
  timRep.dump();

  return 0;
}

