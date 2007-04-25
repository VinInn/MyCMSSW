#include "PoolTest/SLCacheTest/interface/Streamer.h"
#include "PoolTest/SLCacheTest/interface/TCPGetter.h"
#include "Utilities/GenUtil/interface/Sockets.h"
#include <iostream>
#include <string>
#include "POOLCore/Token.h"
#include <iostream>
#include <sstream>
#include<iterator>
#include<algorithm>
#include<utility>

#include "DataSvc/RefBase.h"

#include "PoolTest/SLCacheTest/interface/Streamer.h"
#include<boost/archive/text_oarchive.hpp>
#include<boost/archive/binary_oarchive.hpp>

#include "Reflection/Class.h"
#include "PluginManager/PluginCapabilities.h"
#include "PersistencySvc/ITransaction.h"
#include "PersistencySvc/ISession.h"
#include "DataSvc/IDataSvc.h"
#include "DataSvc/ICacheSvc.h"
#include "SealKernel/Exception.h"

#include <fstream>


#include "PoolTest/SLCacheTest/interface/Protocol.h"

namespace Persil {


  class Request : public Protocol {
  public:

    Request(){}
    Request(const std::string& it, const std::string& in) : 
      token(it), typeName(in){}

    // in reading
    virtual void action(ProtocolDecoder & decoder);

    std::string token;
    std::string typeName;
  };

  class Redirect : public Protocol {
  public:
    Redirect(){}
    explicit Redirect(int ip) : port(ip){}

    // in reading
    virtual void action(ProtocolDecoder & decoder);

    int port;

  };

  class NotFound : public Protocol {
  public:
    // in reading
    virtual void action(ProtocolDecoder & decoder);

    std::string errMess;

  };




  struct MyStream : public TCPClientStream {
    MyStream(const char * a, int i) :  TCPClientStream(a,i){}
    ~MyStream() { (*this) << "bye"  << std::endl;}
  };

  const std::string separator("$%$");

  const int IPportnumber = 12345;
  //  const char *IPserveraddress = "127.0.0.1";
  const char *IPserveraddress = "137.138.9.105";

  TCPGetter::TCPGetter() : stream(IPserveraddress, IPportnumber) {
    // stream.setf( std::ios::skipws );
  }

  TCPGetter::~TCPGetter() { bye();}

  void TCPGetter::bye() {
    stream << "bye"  << std::endl;
  }

  void TCPGetter::operator()(std::istream & str,  const pool::Token& token, 
			     const std::string & typeName) {
    stream << token.toString() << separator << typeName << std::endl;
    std::string message;
    int s;
    //    stream.clear();
    stream >> s; 
    std::cout << "got message size " << s << std::endl;
    stream.get();  // remove endl...
    std::cout << "good and eof " << stream.good() << " " << stream.eof() <<std:: endl;
    if (s==-1) {
      // not in the cache move to other port
      bye();
      stream.reconnect(IPserveraddress, IPportnumber+1);
      stream << token.toString() << separator << typeName << std::endl;
      stream >> s; stream.get();
      std::cout << "got new message size " << s << std::endl;
    }
    if (s==0) { 
      str.rdbuf(0);
      return;
    }

    // message.resize(s);
    // stream.std::istream::read(&(*message.begin()),s);

    // std::istream_iterator<char> sbegin(stream),send;
    // std::copy(sbegin,send,std::inserter(message,message.begin()));
    // for (size_t p=0; p<s; p++)
    // while (!stream.get(*(message.begin()+p)).good());

    /*
    {
      getline(stream, message);
    } while (message.empty());
    */
    //    std::cout << "good and eof " << stream.good() << " " << stream.eof() <<std:: endl;
    // std::cout << "got message of size " << s  // << " " << r  
      //  << " " << message.size()       << std::endl; 

    //str.str(message);
    // locstr.str(message);
    str.rdbuf(stream.rdbuf());
  }      


  TCPPutter::TCPPutter( pool::IDataSvc* ids) : dataSvc(ids){}
  TCPPutter::~TCPPutter(){}

  void TCPPutter::server(bool readit) {
    try {
      TCPSocketWrapper sockserver;
      
      // listen on some port
      int iport = IPportnumber;
      if (readit) iport++;
	
      sockserver.listen(iport);
      
      //	std::cout << "server is ready on port " << iport << std::endl;

      while (true) {
	std::ifstream t("stop.stop");
	if (t) break;
	std::cout << "server is ready on port " << iport << std::endl;
	// accept connection from client
	TCPSocketWrapper sock(sockserver.accept());
	
	std::cout << "accepted connection from: "
		  << sock.address() << " on port "<< iport << std::endl;
	
	// make the stream around the socket wrapper
	TCPStream stream(sock);
	//	stream.setf( std::ios::skipws );
	// stream.setf(std::ios::binary);
	bool oncemore = true;
	std::string  command;
	//	std::string res;
	while (oncemore) {
	  // read the command (token)
	  stream >> command;
	  if (command=="bye") oncemore=false;
	  else {
	    LockMutex gl(cleanMutex);  // avoid cleaner to mess up;
	    const std::string & res = 
	      readit ? fromDB(command) : fromCache(command);
	      // (*this)(command); 
	    std::cout << "sending message of size " << res.size() << std::endl;
	    // stream << res << std::endl;
	    if (res.size()>0) {
	      stream << res.size() << std::endl;
	      stream.write(res.c_str(),res.size()); stream.flush(); // stream << std::endl;
	    }
	    else  stream << (readit ? 0 : -1) << std::endl;
	    command="";
	  }
	}
      }
    }
    catch (const SocketRunTimeException &e) {
      std::cout << "socket exception: "
		<< e.what() << std::endl;
    }
  }
  
  namespace {
    struct Committer {
      pool::IDataSvc * svc;
      Committer(pool::IDataSvc * isvc) : svc(isvc) {
	svc->transaction().start(pool::ITransaction::READ);
	std::cout << "cache size at start " << svc->cacheSvc().cacheSize() << std::endl;
      }
      ~Committer() {
	std::cout << "cache size at commit " << svc->cacheSvc().cacheSize() << std::endl;
	svc->transaction().commit();
      }

    };

  }

  void TCPPutter::clean() {
    // LockMutex gl(mutex);
    LockMutex gl(cleanMutex);
    m_cache.cleaner();
  }



  //  void TCPPutter::operator()(std::string & stoken, std::string & res) {
  const std::string &  TCPPutter::operator()(std::string & stoken) {
     if (dataSvc) return fromDB(stoken);
     else return fromCache(stoken);
  }

  const std::string &  TCPPutter::fromCache(std::string & stoken) {
    static std::string bha;
    if (stoken.empty()) return bha;
    std::cout << "look for " << stoken  << std::endl;
    size_t  seploc = stoken.find(separator);
    std::string clName = stoken.substr(seploc+separator.size());
    stoken.resize(seploc);
    std::cout << "look for " << stoken  << " " << clName << std::endl;
    LockMutex gl(mutex);
    CI p = m_cache.find(stoken);
    if (p!=m_cache.end()) return (*p).second.payload();
    return bha;
  }
  
  const std::string &  TCPPutter::fromDB(std::string & stoken) {
    static std::string bha;
    if (stoken.empty()) return bha;
    std::cout << "look for " << stoken  << std::endl;
    size_t  seploc = stoken.find(separator);
    std::string clName = stoken.substr(seploc+separator.size());
    stoken.resize(seploc);
    std::cout << "look for " << stoken  << " " << clName << std::endl;
    // check cache first anyhow...
    CI p;  
    {
      LockMutex gl(mutex);
      p=m_cache.find(stoken);
    }
    if (p==m_cache.end()) {
      try {
	// load capability (sic)
	seal::PluginCapabilities::get()->load(std::string("LCGReflex/")+clName);
	
	Committer comm(dataSvc); 
	
	pool::Token* tok = new pool::Token();
	tok->fromString(stoken);
	ROOT::Reflex::Type t = ROOT::Reflex::Type::byName(clName);
	Baref ref(dataSvc,*tok, t.typeInfo());
	std::cout << "cache size " << dataSvc->cacheSvc().cacheSize() << std::endl;
	std::cout << "object of type " <<  t.typeInfo().name() 
		  << " loaded in cache at " << ref.ptr() << std::endl;
	tok->release();
	ROOT::Reflex::Object ob(t,ref.ptr());
	std::ostringstream file; // (std::ios::binary); // file.setf( std::ios::skipws );
	boost::archive::binary_oarchive oa(file);
	//      boost::archive::text_oarchive oa(file);
	oa << ob;
	LockMutex gl(mutex);
	std::pair<CI, bool> res = m_cache.insert(stoken, file.str());
	p =  res.first;
      }   
      catch ( seal::Exception& e ) {
	std::cout << "seal error " << e.what() << std::endl; 
	return bha;
      }
      catch ( std::exception& e ) {
	std::cout << "std error " << e.what() << std::endl;
	return bha;
      }
    }
    LockMutex gl(mutex);
    return (*p).second.payload();
  }
  
}
