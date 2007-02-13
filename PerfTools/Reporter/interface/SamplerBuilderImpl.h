#ifndef PerfTools_SamplerBuilder_H
#define PerfTools_SamplerBuilder_H

// FIXME: only for payload...
#include "PerfTools/Sampler/interface/SamplerBase.h"

#include <boost/bind.hpp>

#include<string>
#include<vector>

namespace perftools {

  /* Build a Sample Payload from config strings...
   * implementation...
   *
   */
  class SamplerBuilderImpl : public SamplerBuilder {
  public:
    typedef perftools::SamplerBuilder::Payload Payload;
    SamplerBuilderImpl();
    
    SamplerBuilderImpl(std::string const & name, 
		       std::vector<std::string> const & sources,
		       std::vector<std::string> const & reporter);
    
    
    virtual void build(std::string const & name, 
		       std::vector<std::string> const & sources,
		       std::vector<std::string> const & reporter);
    
  };

}


#endif //  PerfTools_SamplerBuilder_H
