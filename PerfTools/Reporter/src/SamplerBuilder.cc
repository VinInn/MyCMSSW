#include "PerfTools/Reporter/interface/SampleBuilder.h"
#include "PerfTools/Reporter/src/SamplerImpl.h"

namespace perftools {

  SamplerBuilder::SamplerBuilder();

  SamplerBuilder::Payload & 
  SamplerBuilder::operator()(std::vector<std::string> const & sources,
			     std::vector<std::string> const & reporter) {

    if (sources.empty()||reporter.empty()) return;
    // full combinatorics...
    //FIXME now just one reporter
    m_payload.resize(sources.size());
    

  }


#include "PerfTools/Service/interface/ServiceFactory.h"

  void  SamplerBuilder::load(std::string& const name, Payload::value_type & target,
			     std::vector<std::string> const & reporter) {
    //FIXME a bit ad hoc...
    typedef boost::function<long long(void)> Clock;

    if (name.find("Clock")!=std::string::npos) {
      boost::shared_ptr<Clock> c = perftools::ServiceFactory::get()->getService<Clock>("PerfTools:"+name);
      target = perftools::SamplerImpl<long long>(*c,boost::bind(tell,0,_1),false,true);
    }
    else{}
  }

}