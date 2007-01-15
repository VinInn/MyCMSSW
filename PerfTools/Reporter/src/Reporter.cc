#include "PerfTools/Reporter/interface/Reporter.h"


#include <algorithm>
#include <boost/bind.hpp>

namespace perftools {

  void Reporter::Category::reportOneOp(Reporter::OperationStats::value_type const & op) const {
    std::string const & name = op.first;
    Reports::const_iterator p = reports.find(&op.second.type());
    if (p!=reports.end()) 
      (*(*p).second)(name,op.second);
  }

  Reporter::Reporter()  {}

  void Reporter::reportOneCat(Reporter::Categories::value_type const & icat) const {
    Reporter::Category const & category = icat.second;
    if(category.reportTitle) category.reportTitle(icat.first);
    else if(defaultTitleReport) defaultTitleReport(icat.first);
    std::for_each(category.stats.begin(),category.stats.end(),
		  boost::bind(&Reporter::Category::reportOneOp,&category,_1));

  }

  void Reporter::report() const {
    std::for_each(m_operations.begin(),m_operations.end(), 
		  boost::bind(&Reporter::reportOneCat,this,_1));
  }

}
