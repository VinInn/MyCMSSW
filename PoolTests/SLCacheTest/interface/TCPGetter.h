#ifndef Persil_TCPStreamer_H
#define Persil_TCPStreamer_H
#include "PoolTest/SLCacheTest/interface/Getter.h"
#include "PoolTest/SLCacheTest/interface/Streamer.h"
#include "PoolTest/SLCacheTest/interface/Cache.h"

#include "Utilities/GenUtil/interface/ThreadUtils.h"
#include "Utilities/GenUtil/interface/Sockets.h"
#include <iosfwd>
#include <string>
#include<map>
#include<sstream>

namespace pool {
  class Token;
  class IDataSvc;
}



namespace Persil {

  class Baref : public pool::RefBase {
  public:
    Baref(pool::IDataSvc* aDataSvc, const pool::Token& aToken, 
      const std::type_info& refType) : pool::RefBase(aDataSvc, aToken,refType){}
    void * ptr() const {
      return  _pointee(true);
    }

  };

  class TCPGetter : public Getter {
    
    TCPClientStream stream;
    std::istringstream locstr;
  public:
    TCPGetter();
    ~TCPGetter();
    void bye();
    virtual void operator()(std::istream & str,  const pool::Token& token, 
			     const std::string & typeName);
  };

  struct TCPGetterFactory : public GetterFactory {
    virtual  ~TCPGetterFactory(){}
    virtual Getter * create() { 
      return new TCPGetter;
    }
  };

  class TCPPutter : public Putter {
  public:
    LockMutex::Mutex mutex;
    LockMutex::Mutex cleanMutex;
    pool::IDataSvc* dataSvc;
    typedef Persil::Cache Cache;
    typedef Cache::iterator CI;
    // pinning cache....
    Cache m_cache;

    explicit TCPPutter( pool::IDataSvc* ids);
    virtual ~TCPPutter();

    void server(bool readit=true);
    const std::string & operator()(std::string & stoken);
    const std::string & fromCache(std::string & stoken);
    const std::string & fromDB(std::string & stoken);
    // virtual void operator()(std::string & stoken, std::string & res);
    
    void clean();

  };

}
#endif // Persil_TCPStreamer_H
