#ifndef PerfTools_SamplerBuilder_H
#define PerfTools_SamplerBuilder_H

// FIXME: only for payload...
#include "PerfTools/Reporter/Sampler/SamplerBase.h"


#include<string>
#include<vector>

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
			 std::vector<std::string> const & reporter) {
      build(name,sources,reporters);
      return m_payload;
    }
    
    operator Payload & () { return m_payload;}
    
  public:
    
    virtual void build(std::string const & name, 
		       std::vector<std::string> const & sources,
		       std::vector<std::string> const & reporter)=0;
    
  protected:
    
    Payload m_payload;
  };
  
}


#endif //  PerfTools_SamplerBuilder_H
