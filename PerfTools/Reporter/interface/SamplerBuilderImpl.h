#ifndef PerfTools_SamplerBuilderImpl_H
#define PerfTools_SamplerBuilderImpl_H

// FIXME: only for payload...
#include "PerfTools/Sampler/interface/SamplerBuilder.h"

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
    

    virtual std::auto_ptr<SamplerBuilder> 
    create(std::string const & name, 
	   std::vector<std::string> const & sources,
	   std::vector<std::string> const & reporters) const {
      return 
	std::auto_ptr<SamplerBuilder>(new SamplerBuilderImpl(name,sources,reporters));
    }

    
    virtual void build(std::string const & name, 
		       std::vector<std::string> const & sources,
		       std::vector<std::string> const & reporter);
    
  };

}


#endif //  PerfTools_SamplerBuilder_H
