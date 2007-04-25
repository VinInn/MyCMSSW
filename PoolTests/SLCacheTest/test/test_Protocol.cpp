#include "PoolTest/SLCacheTest/interface/Protocol.h"
#include "Reflex/Reflex.h"
#include <sstream>
#include<iostream>
#include<vector>
#include<memory>
#include "PluginManager/PluginManager.h"
#include "Utilities/GenUtil/interface/Sockets.h"
#include "Utilities/GenUtil/interface/NetLogger.h"

namespace {
  class LocalConsumer : public Persil::Consumer {
  public:
    void operator()(const seal::reflex::Object & ob) {
      LocalConsumer & lc = *this;
      std::cout << "got " << ob.type().name(seal::reflex::SCOPED) <<std::endl;
      if ( ob.type().name(seal::reflex::SCOPED).find("Persil")!=std::string::npos) 
	std::cout << "error " <<  ob.type().name(seal::reflex::SCOPED) 
		  << "should not arrive here" <<std::endl;
      if ( ob.type().name(seal::reflex::SCOPED).find("vector")!=std::string::npos) {
	m_v.reset((std::vector<int>*)(ob.address()));
	std::cout << "decoded " << lc.v().size() << " " << lc.v()[1] << " " << lc.v()[4] << std::endl;
      } else {
	m_s.reset((std::string*)(ob.address()));
	std::cout << "decoded " << lc.s() << std::endl;
      }	
      
    }
    std::vector<int> & v() { return *m_v;}

    std::string & s() { return *m_s; }

    std::auto_ptr<std::vector<int> > m_v;
    std::auto_ptr<std::string > m_s;
  };

}

// address of the server
// for testing, both server and client are on the same machine
const int IPportnumber = 12345;


namespace {


  void send(const char *IPserveraddress = "127.0.0.1") {
    try {
      TCPClientStream stream(IPserveraddress, IPportnumber);
      Persil::ProtocolEncoder encoder(stream);
      
      std::vector<int> v(10,3); v[4]=4;
      std::cout << "encoding " << v.size() << " " << v[1] << " " << v[4] << std::endl;
      
      std::stringstream ss;
      NetLogger hello("Hello");
      ss << hello;
      
      encoder.encode(v);
      encoder.encode(ss.str());
      encoder.encode<Persil::Protocol>(Persil::Eof());
    }
    catch (const SocketRunTimeException &e) {
      std::cout << "socket exception: " << e.what() << std::endl;
    }
    
  }

  void collect() {

    try {
      TCPSocketWrapper sockserver;
      
      // listen on some port
      sockserver.listen(IPportnumber);
      
      while(true) {

	std::cout << "server is ready" << std::endl;
	
	// accept connection from client
	TCPSocketWrapper sock(sockserver.accept());
	
	std::cout << "accepted connection from: "
	     << sock.address() << std::endl;
	
	// make the stream around the socket wrapper
	TCPStream stream(sock);
	LocalConsumer lc;
	Persil::ProtocolDecoder decoder(stream,&lc); 
	while (decoder.decode());
      }
    } 
    catch (const SocketRunTimeException &e) {
      std::cout << "socket exception: "
		<< e.what() << std::endl;
    }
  }


  void local() {

    try { 
      seal::PluginManager::get()->initialise();
      std::stringstream ss;
      
      {
	Persil::ProtocolEncoder encoder(ss);
	
	std::vector<int> v(10,3); v[4]=4;
	std::cout << "encoding " << v.size() << " " << v[1] << " " << v[4] << std::endl;
	
	std::cout << "md5 " << Persil::objectMD5(v) << std::endl;
	std::cout << "md5 " << Persil::objectMD5(v) << std::endl;

	encoder.encode(v);
	encoder.encode(std::string("Hello"));
	encoder.encode<Persil::Protocol>(Persil::Eof());
      }
      
      std::cout << "stream size " << ss.str().size() << std::endl; 
      // std::cout << "stream cont " << ss.str() << std::endl; 
      std::cout << "md5 " << Persil::MD5(ss.str()) << std::endl;
 

      
      {
	LocalConsumer lc;
	Persil::ProtocolDecoder decoder(ss,&lc); 
	while (decoder.decode());
	
      }
      
    } catch (std::exception & ex) {
      std::cout << "error!!!" << ex.what() << std::endl;
    }
    
  }
  
}

int main(int argc, char * argv[]) {

  if (argc<2) local();
  else {
    if (argv[1][0]=='c') collect();
    else if (argv[1][0]=='s') send();
    else send(argv[1]);
  }


  return 0;
}
