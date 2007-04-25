#include "Reflection/Class.h"
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

#include "SealUtil/SealTimer.h"
#include "SealUtil/TimingReport.h"
#include "SealUtil/SealHRRTChrono.h"
#include "SealUtil/SealHRChrono.h"

#include "PoolTest/CalibDBTests/interface/Alignments.h"
#include "PoolTest/CalibDBTests/interface/IOV.h"
#include<iostream>
#include<fstream>
#include<string>

#include "POOLCore/POOLContext.h"
#include "SealKernel/Exception.h"
#include "PoolTest/SLCacheTest/interface/TCPGetter.h"
#include "PoolTest/SLCacheTest/interface/Streamer.h"

 
#include "Utilities/GenUtil/interface/Thread.h"
#include <time.h>

namespace {

  class DBServer : public Thread {
  public:
    
    explicit DBServer(Persil::TCPPutter & iput) : putter(iput) {
      initialize();
    }
    ~DBServer() {
      finalize();
    }
    
    virtual void run() {
      // wait for request to fill the cache...
      cout() << "start dbserver thread " << std::endl;
      putter.server(true);

    } 
    /// called if Terminate trapped
    virtual void terminate() {
      cout() << "terminating " << this << std::endl;
    }
    
    /// called if exception trapped;
    virtual void abort() {      
      cout() << "aborting " << this << std::endl;      
    }

    std::ostream & cout() { return std::cout;}


    Persil::TCPPutter & putter;
  };

  class Cleaner : public Thread {
  public:
    
    explicit Cleaner(Persil::TCPPutter & iput) : putter(iput) {
      initialize();
    }
    ~Cleaner() {
      finalize();
    }
    
    virtual void run() {
      // wait for request to fill the cache...
      cout() << "start cleaner thread " << std::endl;
      while (true) {
	std::ifstream t("stop.stop");
	if (t) break;

	putter.clean();
	
	timespec req;
	req.tv_sec = 100;
	req.tv_nsec = 0;
	timespec rem;
	nanosleep(&req,&rem);
      } 
    }
    /// called if Terminate trapped
    virtual void terminate() {
      cout() << "terminating " << this << std::endl;
    }
    
    /// called if exception trapped;
    virtual void abort() {      
      cout() << "aborting " << this << std::endl;      
    }
    
    std::ostream & cout() { return std::cout;}
    
    
    Persil::TCPPutter & putter;
  };
}

int main() {

  const std::string userNameEnv = "POOL_AUTH_USER=cms_vincenzo_dev";
  ::putenv( const_cast<char*>( userNameEnv.c_str() ) );
  const std::string passwdEnv = "POOL_AUTH_PASSWORD=vinPass3";
  ::putenv( const_cast<char*>( passwdEnv.c_str() ) );
  // std::string m_dbConnection( "oracle://devdb10/cms_vincenzo_dev");
  std::string m_dbConnection( "sqlite_file:///data/vin/testsql/align.db");

  seal::PluginManager::get()->initialise();
  seal::TimingReport timRep;
  
  try {
    // Loads the seal message stream

    pool::POOLContext::loadComponent( "SEAL/Services/MessageService" );
    pool::POOLContext::loadComponent( "POOL/Services/EnvironmentAuthenticationService" );


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

    
    Persil::currentCache() = &svc->cacheSvc();
    Persil::TCPPutter putter(svc);

    // start the thread serving from db
    DBServer dbserver(putter);

    // start the cleaner
    Cleaner cleaner(putter);

    // the main serves from cache
    putter.server(false);


    svc->session().disconnectAll();
 

    std::cout << "commit catalog" << std::endl;
    cat->commit();

    delete svc;
    
  }   
  catch ( seal::Exception& e ) {
    std::cout << "seal error " << e.what() << std::endl;
    return 1;
  }
  catch ( std::exception& e ) {
    std::cout << "std error " << e.what() << std::endl;
    return 1;
  }
  catch ( ... ) {
    std::cout << "Funny error" << std::endl;
    return 1;
  }
  
  

  timRep.dump();
  
  std::cout << "finished" << std::endl;

  return 0;
}

