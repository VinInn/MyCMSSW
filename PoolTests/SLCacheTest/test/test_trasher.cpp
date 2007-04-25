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

#include "PersistencySvc/IPersistencySvc.h"
#include "PersistencySvc/IPersistencySvcFactory.h"
#include "PersistencySvc/PersistencySvcException.h"
#
#include "SealUtil/SealTimer.h"
#include "SealUtil/TimingReport.h"
#include "SealUtil/SealHRRTChrono.h"
#include "SealUtil/SealHRChrono.h"

#include "PoolTest/CalibDBTests/interface/Calibs.h"

#include "PoolTest/CalibDBTests/interface/Alignments.h"
#include "PoolTest/CalibDBTests/interface/IOV.h"
#include<iostream>
#include<string>

#include "POOLCore/POOLContext.h"
#include "SealKernel/Exception.h"

#include "PoolTest/SLCacheTest/interface/StreamPersistencySvc.h"
#include "PoolTest/SLCacheTest/interface/Streamer.h"
#include "PoolTest/SLCacheTest/interface/TCPGetter.h"





int main(int csize, char** cline ) {
  seal::PluginManager::get()->initialise();
  seal::TimingReport timRep;
  
  try {
    // Loads the seal message stream

    LockMutex::Mutex mutex; 
    pool::POOLContext::loadComponent( "SEAL/Services/MessageService" );
    pool::POOLContext::loadComponent( "POOL/Services/EnvironmentAuthenticationService" );


    pool::URIParser p;
    p.parse();
    
    pool::IFileCatalog lcat;
    pool::IFileCatalog * cat = &lcat;
    cat->setWriteCatalog(p.contactstring());
    cat->connect();
    
    cat->start();
    own_ptr<pool::IPersistencySvc> oriper;
    own_ptr<pool::IPersistencySvc> safeper;
    own_ptr<pool::IDataSvc>  datasvc;
    
    // Persil::TCPGetter tcpGetter;
    Persil::TCPGetterFactory tcpGetter;
    pool::IPersistencySvcFactory* psfactory = pool::IPersistencySvcFactory::get();
    
    oriper.reset(psfactory->create( "PersistencySvc", lcat ));
    safeper.reset(new StreamPersistencySvc(*oriper.get(), tcpGetter, mutex));
    datasvc.reset(pool::DataSvcFactory::instance(safeper.get()));
    
    
    pool::IDataSvc *svc = datasvc.get();


  
    Persil::currentCache() = &svc->cacheSvc();
    
     svc->transaction().start(pool::ITransaction::READ);
 
    if (csize==2) {
      // peds
      std::string iovToken(cline[1]);
      svc->transaction().start(pool::ITransaction::READ);

      pool::Ref<CalibTests::IOV> iov(svc,iovToken);
      for (int t=0; t<10000; t+=7) {
	if (iov->iov.lower_bound(t)==iov->iov.end()) break;
	std::cout << "at time " << t << " used " << (*iov->iov.lower_bound(t)).first << std::endl; 
	std::string pedCid = (*iov->iov.lower_bound(t)).second;
	std::cout << "ped Cid " <<  pedCid << std::endl;
	pool::Ref<CalibTests::SimplePedestals> peds(svc,pedCid);
	std::cout << "peds size " << peds->m_pedestals.size() << std::endl;
	int l=2000; int le=2050;
	std::cout << "peds from " << l;
	for (CalibTests::SimplePedestals::ItemIterator p= peds->m_pedestals.begin()+l; p!= peds->m_pedestals.begin()+le;p++)
	  std::cout << " " << (*p).m_mean << "," << (*p).m_variance;
	std::cout << std::endl;
      }

      /*
      std::string iovToken(cline[1]);
      
      pool::Ref<CalibTests::IOV> iov(svc,iovToken);
      std::string pedCid = (*iov->iov.lower_bound(253)).second;
      std::cout << "ped Cid " <<  pedCid << std::endl;
      pool::Ref<CalibTests::Pedestals> peds(svc,pedCid);
      int l=2000;
      CalibTests::Pedestals::ItemRange r=peds->get(l);
      std::cout << "peds from " << l;
      for (CalibTests::Pedestals::ItemIterator p=r.first; p!=r.second;p++)
	std::cout << " " << (*p).m_mean << "," << (*p).m_variance;
      std::cout << std::endl;    
      */
    }

    if (csize==3) {
      // align
    
      std::string iovAToken(cline[1]);
      std::string iovIToken(cline[2]);
      
      pool::Ref<CalibTests::IOV> iovA(svc,iovAToken);
      pool::Ref<CalibTests::IOV> iovI(svc,iovIToken);
      
      std::cout << "iov size " << iovA->iov.size() << std::endl;
      
      
      
      for (int t=0; t<10000; t+=7) {
	if (iovA->iov.lower_bound(t)==iovA->iov.end()) break;
	if (iovI->iov.lower_bound(t)==iovI->iov.end()) break;
	std::string alignCid = (*iovA->iov.lower_bound(t)).second;
	std::string indexCid = (*iovI->iov.lower_bound(t)).second;
	std::cout << "align Cid " <<  alignCid << std::endl;
	std::cout << "index Cid " <<  indexCid << std::endl;
	pool::Ref<Alignments> align(svc,alignCid);
	pool::Ref<Geom2Index> index(svc,indexCid);
	
	
	
	Geom2Index::GeomId id=500+20+7;
	std::cout << "align for " << id;
	int i= index->m_indexTable[id];
	std::cout << " " << (*align).m_align[i].translation() << "," 
		  << (*align).m_align[i].eulerAngles();
	std::cout << std::endl;
	
      }
      
    }
     
    svc->transaction().commit();
    svc->session().disconnectAll();
    std::cout << "committed" << std::endl;
    
    std::cout << "commit catalog" << std::endl;
    cat->commit();
    
    
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
  return 0;
}
  
