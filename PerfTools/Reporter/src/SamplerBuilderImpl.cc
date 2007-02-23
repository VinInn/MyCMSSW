#include "PerfTools/Reporter/interface/SamplerBuilderImpl.h"
#include "PerfTools/Reporter/src/SamplerImpl.h"

#include "PerfTools/Service/interface/ServiceFactory.h"

#include "PerfTools/Reporter/interface/SimpleImmediateReporter.h"
#include "PerfTools/Reporter/interface/ImmediateReporterWithNames.h"

#include "PerfTools/Reporter/interface/Reporter.h"
#include "PerfTools/Reporter/interface/MinMaxCounter.h"

#include<boost/function.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/bind.hpp>
#include <boost/iterator/zip_iterator.hpp>

#include<iostream>

#include "ReporterUtil.h"

namespace perftools {
  
  template<typename R>
  SamplerBuilder::Payload::value_type loadImmediate(std::string const & name, boost::shared_ptr<R> reporter) {
    //FIXME a bit ad hoc...
    typedef boost::function<long long(void)> Clock;
    typedef boost::function<int(void)> Mem;
    
    if (name.find("Clock")!=std::string::npos) {
      boost::shared_ptr<Clock> c = perftools::ServiceFactory::get()->getService<Clock>("PerfTools:"+name);
      return perftools::SamplerImpl<long long>(*c,
					       boost::bind(&R::template operator()<long long>,
							   reporter,(name+": "),_1),false,true);
    }
    else if (name.find("Memory")!=std::string::npos) {
      boost::shared_ptr<Mem> c = perftools::ServiceFactory::get()->getService<Mem>("PerfTools:"+name);
      return perftools::SamplerImpl<int>(*c,
					 boost::bind(&R::template operator()<int>,
						     reporter,(name+": "),_1),false,true);
    }
    else{}
    // FIXME what to do if not found?
    return SamplerBuilder::Payload::value_type();
  }
  
  SamplerBuilder::Payload::value_type loadMinMax(boost::tuple<std::string const&, MinMaxCounter const &> const & t) {
    //FIXME a bit ad hoc...
    typedef boost::function<long long(void)> Clock;
    typedef boost::function<int(void)> Mem;
    
    std::string const & name = t.get<0>();    
    MinMaxCounter & counter = const_cast<MinMaxCounter&>(t.get<1>());
    
    if (name.find("Clock")!=std::string::npos) {
      boost::shared_ptr<Clock> c = perftools::ServiceFactory::get()->getService<Clock>("PerfTools:"+name);
      return perftools::SamplerImpl<long long>(*c,
					       boost::bind(&MinMaxCounter::fill,
							   boost::ref(counter),_1),false,true);
    }
    else if (name.find("Memory")!=std::string::npos) {
      boost::shared_ptr<Mem> c = perftools::ServiceFactory::get()->getService<Mem>("PerfTools:"+name);
      return perftools::SamplerImpl<int>(*c,
					 boost::bind(&MinMaxCounter::fill,
						     boost::ref(counter),_1),false,true);
    }
    else{}
    // FIXME what to do if not found?
    return SamplerBuilder::Payload::value_type();
  }
  
  void UDQImmediate(UDQBase & udq, boost::shared_ptr<perftools::ImmediateReporterWithNames> & sir) {
    std::vector<std::string> v;
    udqr.toString(v);
    std::for_each(v.begin,v.end(), boost::bind(&perftools::ImmediateReporterWithNames::operator()<string>,
					       sir,_1));
  }

  void buildUDQ(std::string const & name, 
		std::vector<std::string> const & reporters, RegisterUDQI & udqr,
		SamplerBuilder::Payload::value_type & payload) {
    
    if (reporters[0]=="Immediate") {
      boost::shared_ptr<perftools::ImmediateReporterWithNames> 
	sir(new perftools::ImmediateReporterWithNames(std::cout,udqr.names, name+"_udq: "));
      boost::bind(UDQImmediate,_1,sir);
    }

    /*
	if (reporters[0]=="Summary") {
	boost::shared_ptr<perftools::Reporter> summary = 
	perftools::ServiceFactory::get()->getService<perftools::Reporter>("PerfTools:Reporter");
      // FIXME moveit in FW
      summary->autoReport();
      summary->setDefaultTitleReport(&printTitle);
      boost::shared_ptr<perftools::SimpleImmediateReporter> sir(new perftools::SimpleImmediateReporter(std::cout,sources.size()));
      
      std::vector<MinMaxCounter> & counters = 
	*summary->subscribe<std::vector<MinMaxCounter> >("Counts/Ave/TruncAve/Min/Max",name,boost::bind(printV<MinMaxCounter>,_1,_2,sources,sir));
      counters.resize(sources.size());
      std::transform(boost::make_zip_iterator(boost::make_tuple(sources.begin(), counters.begin())),
		     boost::make_zip_iterator(boost::make_tuple(sources.end(), counters.end())),
		     m_payload.begin(),
		     boost::bind(loadMinMax,_1)
		     );      
    }
    */
  }
  
  
  SamplerBuilderImpl::SamplerBuilderImpl(){}

  SamplerBuilderImpl::SamplerBuilderImpl(std::string const & name, std::vector<std::string> const & sources,
					 std::vector<std::string> const & reporters, RegisterUDQI const & udqr) {
    build(name,sources,reporters,udqr);
  }
 
 

  void  SamplerBuilderImpl::build(std::string const & name, std::vector<std::string> const & sources,
			      std::vector<std::string> const & reporters, RegisterUDQI const & udqr) {
    
    if ( (sources.empty()&&udqr.dont()) || reporters.empty()) return;
    // full combinatorics...
    //FIXME now just one reporter
    m_payload.resize(sources.size()+ (udqr.dont() ? 0 : 1) );
    
    // process UDQ
    if (!udqr.dont()) 
      buildUDQ(name,reporters,udqr,m_payload.back());

    // process sources
    if (source.empty()) return;
    // FIXME here goes a factory..
    //    if (reporters.find("Immediate")!=reporters.end()) {
    if (reporters[0]=="Immediate") {
      boost::shared_ptr<perftools::SimpleImmediateReporter> sir(new perftools::SimpleImmediateReporter(std::cout,sources.size(), name+": "));
      
      std::transform(sources.begin(),sources.end(),
		     m_payload.begin(),
		     boost::bind(loadImmediate<perftools::SimpleImmediateReporter>,_1,sir));
    }
    if (reporters[0]=="Summary") {
      boost::shared_ptr<perftools::Reporter> summary = 
	perftools::ServiceFactory::get()->getService<perftools::Reporter>("PerfTools:Reporter");
      // FIXME moveit in FW
      summary->autoReport();
      summary->setDefaultTitleReport(&printTitle);
      boost::shared_ptr<perftools::SimpleImmediateReporter> sir(new perftools::SimpleImmediateReporter(std::cout,sources.size()));
      
      std::vector<MinMaxCounter> & counters = 
	*summary->subscribe<std::vector<MinMaxCounter> >("Counts/Ave/TruncAve/Min/Max",name,boost::bind(printV<MinMaxCounter>,_1,_2,sources,sir));
      counters.resize(sources.size());
      std::transform(boost::make_zip_iterator(boost::make_tuple(sources.begin(), counters.begin())),
		     boost::make_zip_iterator(boost::make_tuple(sources.end(), counters.end())),
		     m_payload.begin(),
		     boost::bind(loadMinMax,_1)
		     );      
    }
  }
 
}
