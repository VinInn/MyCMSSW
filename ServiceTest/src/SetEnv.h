#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

struct SetEnv {
  
  SetEnv(edm::ParameterSet const& pset, edm::ActivityRegistry  & activity) {
    std::cerr << "SetEnv loaded" << std::endl;
    const std::vector<std::string> &  envs = pset.getUntrackedParameter<std::vector<std::string> >("EnvVar",std::vector<std::string>() );
    if (envs.empty()) { 
      std::cerr<< "No envar defined given " << std::endl;
      return;
    }
    
    for (int i=0; i<int(envs.size());i++) {
      const std::string & env = envs[i];
      size_t p = env.find("=");
      if (p==std::string::npos) continue;
      ::setenv(env.substr(0,p).c_str(),env.substr(p+1).c_str(),1);
    }

    activity.watchPostEndJob(this,&SetEnv::termination);
    
    
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
  
DEFINE_ANOTHER_FWK_SERVICE (SetEnv);

