#include "PerfTools/Reporter/interface/SamplerBuilderImpl.h"
#include "PerfTools/Reporter/src/SamplerImpl.h"

#include "PerfTools/Service/interface/ServiceFactory.h"

#include "PerfTools/Reporter/interface/SimpleImmediateReporter.h"

#include "PerfTools/Reporter/interface/Reporter.h"
#include "PerfTools/Reporter/interface/MinMaxCounter.h"

#include<boost/function.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/bind.hpp>
#include <boost/iterator/zip_iterator.hpp>

#include<iostream>

std::ostream & operator<<(std::ostream & co,  perftools::MinMaxCounter const & counter) {
  return
    co << counter.m_counts
     << '/' << counter.m_tot/double(counter.m_counts)  
     << '/' << counter.m_min 
     << '/' << counter.m_max;
}

namespace perftools {

  template<typename T>
  void printOne(boost::tuple<T const&, std::string const&> const & t,
	        boost::shared_ptr<perftools::SimpleImmediateReporter> sir) {
    sir->operator()<T>(t. template get<1>() + ": ",t. template get<0>());
  }
  
  template<typename T>
  void printV(std::string const & name, std::vector<T> const & v,
	      std::vector<std::string> const & tags,
	      boost::shared_ptr<perftools::SimpleImmediateReporter> sir) {
    sir->stream() << name << ": ";
    typedef boost::tuple<T const&, std::string const&> Tuple;
    std::for_each(boost::make_zip_iterator(boost::make_tuple(v.begin(), tags.begin())),
		  boost::make_zip_iterator(boost::make_tuple(v.end(), tags.end())),
		  boost::bind(printOne<T>, _1, sir)
		  ); 
  }
  
  
  
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
      boost::shared_ptr<Mem> c = perftools::ServiceFactory::get()->getService<Clock>("PerfTools:"+name);
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
      boost::shared_ptr<Mem> c = perftools::ServiceFactory::get()->getService<Clock>("PerfTools:"+name);
      return perftools::SamplerImpl<int>(*c,
					 boost::bind(&MinMaxCounter::fill,
						     boost::ref(counter),_1),false,true);
    }
    else{}
    // FIXME what to do if not found?
    return SamplerBuilder::Payload::value_type();
  }
  
  
  SamplerBuilderImpl::SamplerBuilderImpl(){}

  SamplerBuilderImpl::SamplerBuilderImpl(std::string const & name, std::vector<std::string> const & sources,
				 std::vector<std::string> const & reporters) {
    build(name,sources,reporters);
  }
 
 

  void  SamplerBuilderImpl::build(std::string const & name, std::vector<std::string> const & sources,
			      std::vector<std::string> const & reporters) {
    
    if (sources.empty()||reporters.empty()) return;
    // full combinatorics...
    //FIXME now just one reporter
    m_payload.resize(sources.size());
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
      boost::shared_ptr<perftools::SimpleImmediateReporter> sir(new perftools::SimpleImmediateReporter(std::cout,sources.size()));
      
      std::vector<MinMaxCounter> & counters = 
	*summary->subscribe<std::vector<MinMaxCounter> >("Counts/Ave/Min/Max",name,boost::bind(printV<MinMaxCounter>,_1,_2,sources,sir));
      counters.resize(sources.size());
      std::transform(boost::make_zip_iterator(boost::make_tuple(sources.begin(), counters.begin())),
		     boost::make_zip_iterator(boost::make_tuple(sources.end(), counters.end())),
		     m_payload.begin(),
		     boost::bind(loadMinMax,_1)
		     );      
    }
  }
 
}
