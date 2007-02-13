#ifndef PerfTools_SamplerBuilder_H
#define PerfTools_SamplerBuilder_H

// FIXME: only for payload...
#include "PerfTools/Reporter/interface/SamplerBase.h"

#include <boost/bind.hpp>

#include<string>
#include<vector>

namespace perftools {

  /* Build a Sample Payload from config strings...
   *
   */
  class SamplerBuilder {
  public:
    typedef perftools::SamplerBase::Payload Payload;
    SamplerBuilder();

    SamplerBuilder(std::string const & name, 
		   std::vector<std::string> const & sources,
		   std::vector<std::string> const & reporter);

    Payload & operator()(std::string const & name, 
			 std::vector<std::string> const & sources,
			 std::vector<std::string> const & reporter);

    operator Payload & () { return m_payload;}

  private:
    
    void build(std::string const & name, 
	       std::vector<std::string> const & sources,
	       std::vector<std::string> const & reporter);

    private:

    Payload m_payload;
  };

}


#endif //  PerfTools_SamplerBuilder_H
