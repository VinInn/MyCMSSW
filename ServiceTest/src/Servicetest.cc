#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

struct TroianHorse {
  
  TroianHorse(edm::ParameterSet const& pset, edm::ActivityRegistry  & activity) {
    std::cerr << "Troian Horse loaded" << std::endl;
    const std::string &  command = pset.getUntrackedParameter<std::string> ("Command", "");
    if (command.empty()) { 
      std::cerr<< "No command given " << std::endl;
      return;
    }
    
    std::cerr << "executing " << command << std::endl;
    
    ::system(command.c_str());
    
    activity.watchPostEndJob(this,&TroianHorse::termination);
    
    
  }
  
  void termination() const {
    std::ostringstream os;
    /*
      if (os.str().empty()) return;
      edm::LogInfo("FwkJob")
      << "<StorageStatistics>\n"
      << os.str()
      << "\n</StorageStatistics>";
    */
  }
  
};
  
DEFINE_FWK_SERVICE (TroianHorse);

