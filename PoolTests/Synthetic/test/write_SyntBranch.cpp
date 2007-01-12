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

#include "PoolTests/Synthetic/interface/Synthesis.h"
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>
#include<vector>

#include <boost/assign/std/vector.hpp>
using namespace boost::assign;



class OneBranch {
public:
  typedef boost::fusion::vector<int, char, double, std::string, std::vector<int> > seq_type;

private:
  
  pool::IDataSvc *svc;
  
  struct Names {
    std::vector<std::string> names;
    Names() {
      names += "id", "dir";
    }
  };
  static std::vector<std::string> const & names() {
    static Names local;
    return local.names;
  }
  pool::Placement place;  
  pool::Ref<seq_type> obj;
  synthetic::Dict<seq_type> dict;
  

public:
  explicit OneBranch(pool::IFileCatalog * cat) :    
    svc(pool::DataSvcFactory::instance(cat)),
    place("T1.root", pool::DatabaseSpecification::PFN, "Syn(MyClass)", ROOT::Reflex::Type(), 
	    pool::ROOTTREE_StorageType.type()), 
    obj(svc),
    dict("MyClass",names().begin(),names().end())
  {
    // Define the policy for the implicit file handling
    pool::DatabaseConnectionPolicy policy;
    policy.setWriteModeForNonExisting(pool::DatabaseConnectionPolicy::CREATE);
    // policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::OVERWRITE);
    policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::UPDATE);
    svc->session().setDefaultConnectionPolicy(policy);
    
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    
  }
  
  ~OneBranch() {
    svc->transaction().commit();
    svc->session().disconnectAll();
    std::cout << "cache size at end " << svc->cacheSvc().cacheSize() << std::endl;
    delete svc;
  }

  void loop(int i) {

     svc->transaction().start(pool::ITransaction::UPDATE);
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    
    obj = new seq_type(i, 'x', 3.3*(i%5), "hello", std::vector<int>(4,0));

    obj.markWrite(place);
     
    std::cout << "cache size " << svc->cacheSvc().cacheSize() << std::endl;
    svc->transaction().commitAndHold();
    
    if (i%4==0) {
      std::cout << "cache size before commit " << svc->cacheSvc().cacheSize() << std::endl;
      svc->transaction().commit();
       std::cout << "cache size after commit " << svc->cacheSvc().cacheSize() << std::endl;
    }
  }
  


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
  OneBranch t(cat);


 
  for (int i=0;i<50;i++) {
    t.loop(i);

  }
  

  cat->commit();

  std::cout << "the end" << std::endl;


  return 0;
}
