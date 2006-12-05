
/* before this line the _genH.h header is placed
 *
 *
 */
#include "PluginManager/PluginManager.h"
#include "FileCatalog/URIParser.h"
#include "FileCatalog/IFileCatalog.h"

#include "PoolTests/DataFormatTests/interface/OneBranchTree.h"
#include "PoolTests/DataFormatTests/interface/ProductMaker.h"

#include<iostream>


int main(int argc, char * argv[]) {

  int ret=0;
  try {
    
    seal::PluginManager::get()->initialise();
    
    pool::URIParser p;
    p.parse();
    
    pool::IFileCatalog cat;
    cat.setWriteCatalog(p.contactstring());
    
    cat.connect();
    cat.start();
    
    try {
      OneBranchTree tree(&cat, filename);
      
      WrapperMaker<TheWrapper> maker;
      
      std::cout << "[DataFormatsTest] ";
      maker.message(std::cout);
      std::cout << std::endl;
      
      for (int i=0;i<50;i++) {
	tree.add(maker.make());
      }
      
    }
    catch (const std::exception & ce) {
      std::cout << "[DataFormatsTest] Error: "
		<< ce.what() << std::endl;
      ret=100;
    }
    catch (...) {
      std::cout << "[DataFormatsTest] Error: "
		<< "unkown error" << std::endl;
      ret=101;
    }
    
    cat.commit();
  }
  catch (const std::exception & ce) {
    std::cout << "[DataFormatsTest] Error: "
	      << ce.what() << std::endl;
    ret=102;
  }
  catch (...) {
    std::cout << "[DataFormatsTest] Error: "
	      << "unkown error" << std::endl;
    ret=103;
  }
  
  return 0;
  
}
