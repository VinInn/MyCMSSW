#ifndef PerfTools_SamplerBuilder_H
#define PerfTools_SamplerBuilder_H

// FIXME: only for payload...
#include "PerfTools/Sampler/interface/SamplerBase.h"


#include<string>
#include<vector>
#include<boost/shared_ptr.hpp>

namespace perftools {

  /* interface to Build a Sample Payload from config strings...
   *
   */
  class SamplerBuilder {
  public:
    typedef perftools::SamplerBase::Payload Payload;
    SamplerBuilder(){}
    virtual  ~SamplerBuilder(){}
    
    Payload & operator()(std::string const & name, 
			 std::vector<std::string> const & sources,
			 std::vector<std::string> const & reporters) {
      build(name,sources,reporters);
      return m_payload;
    }

    virtual boost::shared_ptr<SamplerBuilder> 
    create(std::string const & name, 
	   std::vector<std::string> const & sources,
	   std::vector<std::string> const & reporters) const =0;

    
    operator Payload & () { return m_payload;}
    
  public:
    
    virtual void build(std::string const & name, 
		       std::vector<std::string> const & sources,
		       std::vector<std::string> const & reporters)=0;
    
  protected:
    
    Payload m_payload;
  };
  
}


#endif //  PerfTools_SamplerBuilder_H
