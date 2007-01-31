#include "PerfTools/Reporter/interface/SamplerBuilder.h"
#include "PerfTools/Reporter/src/SamplerImpl.h"

#include "PerfTools/Service/interface/ServiceFactory.h"

#include<boost/function.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/bind.hpp>

namespace perftools {

  template<typename R>
  SamplerBuilder::Payload::value_type load(std::string const & name, R reporter) {
    //FIXME a bit ad hoc...
    typedef boost::function<long long(void)> Clock;
    
    if (name.find("Clock")!=std::string::npos) {
      boost::shared_ptr<Clock> c = perftools::ServiceFactory::get()->getService<Clock>("PerfTools:"+name);
      return perftools::SamplerImpl<long long>(*c,
					       boost::bind(&R::operator()<long long>,
							   reporter,name.c_str(),_1),false,true);
    }
    else{}
  }
  

  SamplerBuilder::SamplerBuilder(){}

  SamplerBuilder::Payload & 
  SamplerBuilder::operator()(std::vector<std::string> const & sources,
			     std::vector<std::string> const &) {

    if (sources.empty()||reporter.empty()) return m_payload;
    // full combinatorics...
    //FIXME now just one reporter
    m_payload.resize(sources.size());
    boost::shared_ptr<perftools::SimpleImmediateReporter> sir(new perftools::SimpleImmediateReporter(std::cout,3));

    std::transform(sources.begin(),source.end(),
		   m_payload.begin(),
		   boost::bind(load< boost::shared_ptr<perftools::SimpleImmediateReporter> >,_1,sir));
    return m_payload;
  }

 
}
