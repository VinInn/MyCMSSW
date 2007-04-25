#include "PoolTest/SLCacheTest/interface/NamedPayload.h"
#include "PoolTest/SLCacheTest/interface/Protocol.h"
#include "Reflex/Reflex.h"
#include <sstream>
#include<iostream>
#include<vector>
#include<memory>
#include "PluginManager/PluginManager.h"
#include "Utilities/GenUtil/interface/Sockets.h"
#include "Utilities/GenUtil/interface/NetLogger.h"

#include<map>


namespace {

  typedef std::pair<ProtocalEx::Meta*,ProtocalEx::Product*> StoreElem;
  typedef std::map<std::string,std::pair<ProtocalEx::Meta*,ProtocalEx::Product*> > Store;

  class LocalConsumer : public NamedPayloadConsumer {
  public:
    explicit LocalConsumer(Store & istore) : store(istore){} 
    void operator()(const std::string &name, const seal::reflex::Object & meta, 
		    const seal::reflex::Object & ob) {
      // LocalConsumer & lc = *this;
      std::cout << "received " << meta.type().name(seal::reflex::SCOPED)  
		<< " " << ob.type().name(seal::reflex::SCOPED) <<std::endl;
      store[name] = std::pair<ProtocalEx::Meta*,ProtocalEx::Product*>((ProtocalEx::Meta*)(meta.address()), (ProtocalEx::Product*)(ob.address()));
    }
  private:
    Store & store;
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
      
      Store store;
      ProtocalEx::ProductT<std::vector<int> > v; v.t= std::vector<int>(10,3); v.t[4]=4;
      ProtocalEx::ProductT<std::string> h; h.t = "Hello";
      ProtocalEx::Meta fake;
      store["one"] = StoreElem(&fake,&v);
      store["two"] = StoreElem(&fake,&h);
      
      
      for (Store::const_iterator p=store.begin();p!=store.end();p++ ) {
	encoder.encode<Persil::Protocol>(NamedPayload((*p).first));
	encoder.encodeDirectPoly(*(*p).second.first);
	encoder.encodeDirectPoly(*(*p).second.second);
      }
      
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
      
      Store store;
      while(true) {

	std::cout << "server is ready" << std::endl;
	
	// accept connection from client
	TCPSocketWrapper sock(sockserver.accept());
	
	std::cout << "accepted connection from: "
	     << sock.address() << std::endl;
	store.clear();
	// make the stream around the socket wrapper
	TCPStream stream(sock);
	LocalConsumer lc(store);
	Persil::ProtocolDecoder decoder(stream,&lc); 
	while (decoder.decode());
	std::cout << "received " << store.size() << std::endl;

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
	Store store;
	ProtocalEx::ProductT<std::vector<int> > v; v.t= std::vector<int>(10,3); v.t[4]=4;
	ProtocalEx::ProductT<std::string> h; h.t = "Hello";
	ProtocalEx::Meta fake;
	store["one"] = StoreElem(&fake,&v);
	store["two"] = StoreElem(&fake,&h);
	
	
	Persil::ProtocolEncoder encoder(ss);
	
	for (Store::const_iterator p=store.begin();p!=store.end(); p++ ) {
	  encoder.encode<Persil::Protocol>(NamedPayload((*p).first));
	  encoder.encodeDirectPoly(*(*p).second.first);
	  encoder.encodeDirectPoly(*(*p).second.second);
	}
	
	encoder.encode<Persil::Protocol>(Persil::Eof());
      }
      
      std::cout << "stream size " << ss.str().size() << std::endl; 
      // std::cout << "stream cont " << ss.str() << std::endl; 
      std::cout << "md5 " << Persil::MD5(ss.str()) << std::endl;
 

      
      {
	Store store;
	LocalConsumer lc(store);
	Persil::ProtocolDecoder decoder(ss,&lc); 
	while (decoder.decode());
	std::cout << "received " << store.size() << std::endl;
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
