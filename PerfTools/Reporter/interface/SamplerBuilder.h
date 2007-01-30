#ifndef PerfTools_SamplerBuilder_H
#define PerfTools_SamplerBuilder_H

// FIXME: only for payload...
#include "PerfTools/Reporter/interface/Sample.h"

#include<string>
#include<vector>

namespace perftools {

  /* Build a Sample Payload from config strings...
   *
   */
  class SamplerBuilder {
  public:
    typedef perftools::Sample::Payload Payload;
    SamplerBuilder();

    Payload & operator()(std::vector<std::string> const & sources,
			 std::vector<std::string> const & reporter);

    private:
    Payload m_payload;
  };

}


#endif //  PerfTools_SamplerBuilder_H
