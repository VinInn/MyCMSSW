#ifndef PerfTools_ReporterUtil_H
#define PerfTools_ReporterUtil_H

#include "PerfTools/Reporter/interface/SimpleImmediateReporter.h"
#include "PerfTools/Reporter/interface/MinMaxCounter.h"

#include<boost/function.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/bind.hpp>
#include <boost/iterator/zip_iterator.hpp>

#include<string>
#include<vector>
#include<ostream>

std::ostream & operator<<(std::ostream & co,  perftools::MinMaxCounter const & counter) {
  return
    co << counter.m_counts
       << '/' << counter.ave() 
       << '/' << counter.truncAve() 
       << '/' << counter.m_min 
       << '/' << counter.m_max;
}

namespace perftools {


  void printTitle(std::string const & name) {
    std::cout << name << std::endl;
  }


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
  
  
}

#endif // PerfTools_ReporterUtil_H
