#ifndef StreamPERSISTENCYSVC_H
#define StreamPERSISTENCYSVC_H

#include "PersistencySvc/IPersistencySvc.h"
#include "Utilities/GenUtil/interface/ThreadUtils.h"
#include "Utilities/Notification/interface/TimerProxy.h"


namespace Persil {
  class GetterFactory;
}

/** a thread safe proxy to pool::IPersistencySvc
 */
class StreamPersistencySvc : public pool::IPersistencySvc {
private:
  pool::IPersistencySvc & me;
  
  Persil::GetterFactory & getterFact;
  
  LockMutex::Mutex & mutex;
  typedef pool::IPersistencySvc super;
  TimerProxy searchIt;
  TimerProxy readIt;

public:
  
  StreamPersistencySvc(super & it, Persil::GetterFactory & igetf, LockMutex::Mutex & m);
  ~StreamPersistencySvc();

  pool::IFileCatalog& fileCatalog()    {
    LockMutex gl(mutex);
    return me.fileCatalog();
  }
  
  void setFileCatalog(  pool::IFileCatalog& catalog )    {
    LockMutex gl(mutex);
    me.setFileCatalog(catalog );
  }
  
  pool::ITokenValidator& tokenValidator()    {
    LockMutex gl(mutex);
    return me.tokenValidator();
  }
  
  void setTokenValidator( pool::ITokenValidator& validator )    {
    LockMutex gl(mutex);
    me.setTokenValidator(validator );
  }
  
  pool::IDataTransform& dataTransform()    {
    LockMutex gl(mutex);
    return  me.dataTransform();
  }
  
  void setDataTransform( pool::IDataTransform& dataTransform )    {
    LockMutex gl(mutex);
    me.setDataTransform(dataTransform );
  }
  
  std::pair< void*, seal::reflex::Type > readObject( const pool::Guid& transientGuid,
						     const pool::Token& token,
						     pool::DataHandler* handler = 0 )    {
    LockMutex gl(mutex);
    
    return me.readObject(transientGuid, token, handler);
  }
  
  std::pair< void*, seal::reflex::Type > readObject( const seal::reflex::Type& type,
						     const pool::Token& token,
						     pool::DataHandler* handler = 0 )    {
    LockMutex gl(mutex);
    
    return me.readObject(type, token, handler);
  }
  
  std::pair< void*, seal::reflex::Type > readObject( const pool::Token& token,
						     pool::DataHandler* handler = 0 );


  pool::Token* registerForWrite( const pool::Placement& place,
				 const void* object,
				 const seal::reflex::Type& type,
				 pool::DataHandler* handler = 0 )    {
    LockMutex gl(mutex);
    return me.registerForWrite( place,object,type,handler);
  }
  
  bool updateObject( const void* object,
		     const seal::reflex::Type& type,
		     const pool::Token& token,
		     pool::DataHandler* handler = 0 )    {
    LockMutex gl(mutex);
    return me.updateObject( object, type, token, handler);
  }
  
  
  
  bool destroyObject( const pool::Token& token )    {
    LockMutex gl(mutex);
    return me.destroyObject(token );
  }
  
  pool::ISession& session()  {
    LockMutex gl(mutex);
    return me.session();
  }

  bool registerObserver( pool::ITransactionObserver& observer ) {
    LockMutex gl(mutex);
    return me.registerObserver(observer );
  }

  bool deregisterObserver( pool::ITransactionObserver& observer ) {
    LockMutex gl(mutex);
    return me.deregisterObserver(observer );
  }

};

#endif // StreamPERSISTENCYSVC_H
