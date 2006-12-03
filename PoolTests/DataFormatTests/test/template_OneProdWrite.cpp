
/* before this line the _genH.h header is placed
 *
 *
 */
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



int main(int argc, char * argv[]) {


  seal::PluginManager::get()->initialise();

  pool::URIParser p;
  p.parse();
  
  pool::IFileCatalog cat;
  cat.setWriteCatalog(p.contactstring());
 
  cat.connect();
  cat.start();
  OneBranchTree tree(&cat, filename);

  WrapperMaker<TheWrapper> maker;
 
  for (int i=0;i<50;i++) {
    tree.add(maker.make());
  }
  

  cat.commit();

  //  std::cout << "the end" << std::endl;



  return 0;

}
