#include "PluginManager/PluginManager.h"
#include "POOLCore/Token.h"
#include "FileCatalog/URIParser.h"
#include "FileCatalog/FCSystemTools.h"
#include "FileCatalog/IFileCatalog.h"

#include "PoolTests/DataFormatTests/interface/OneBranchTree.h"
#include "PoolTests/DataFormatTests/interface/ProductMaker.h"

#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "DataFormats/Common/interface/Wrapper.h"
// #include "DataFormats/Common/interface/EventAux.h"

typedef edm::Wrapper<std::vector<SimTrack> > TheWrapper;
// cannot be written by pool...
// typedef edm::Wrapper<EventAux> TheWrapper;


int main(int argc, char * argv[]) {


  seal::PluginManager::get()->initialise();

  pool::URIParser p;
  p.parse();
  
  pool::IFileCatalog cat;
  cat.setWriteCatalog(p.contactstring());
 
  cat.connect();
  cat.start();
  OneBranchTree tree(&cat);

  WrapperMaker<TheWrapper> maker;
 
  for (int i=0;i<50;i++) {
    tree.add(maker.make());
  }
  

  cat.commit();

  //  std::cout << "the end" << std::endl;



  return 0;

}
