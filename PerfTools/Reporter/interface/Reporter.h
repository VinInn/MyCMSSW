#ifndef PerfTools_Reporter_H
#define PerfTools_Reporter_H

#include <string>
#include <map>
#include <typeinfo>

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace perftools {


  class Reporter {
  public:

   struct BaseReport {
      virtual ~BaseReport(){}
     virtual void operator()(std::string const & name, boost::any const & op) =0;
    };

    typedef std::map<std::string, std::pair<boost::any,boost::shared_ptr<BaseReport> > > OperationStats;
    typedef std::map<std::type_info const *, boost::shared_ptr<BaseReport> > Reports;
    typedef boost::function<void(std::string const &)> TitleReport;

    struct Category{
      Category() : reportTitle(0){}
      OperationStats stats;
      Reports reports;
      TitleReport reportTitle;
      void reportOneOp(OperationStats::value_type const & op) const;
    };

 
    typedef std::map<std::string, Category> Categories;


 
    template<typename OP>
    struct Report : public BaseReport {
      Report(){} 
      template<typename R> Report(R r) : m_report(r){}
      virtual void operator()(std::string const & name, boost::any const & iop) {
	OP const * op = boost::any_cast<OP>(&iop);
	if (op) m_report(name, *op);
      }
      boost::function<void(std::string const &, OP const &)> m_report;
    };


  public:

    Reporter();
    
    template<typename OP>
    OP * subscribe(std::string const & cat,
		   std::string const & name) {
      OP a(OP());
      boost::any x(a);
      return boost::any_cast<OP>(&m_operations[cat].stats[name].first.swap(x));
    }

    template<typename OP, typename R>
    OP * subscribe(std::string const & cat,
		   std::string const & name, R report, OP op=OP()) {
      m_operations[cat].stats[name].second.reset(new Report<OP>(report));
      boost::any x(op);
      return boost::any_cast<OP>(&m_operations[cat].stats[name].first.swap(x));
    }
		 
    template<typename OP,typename R> 
    void add(std::string const & cat,
	     R report) {
      // typedef typename boost::function_traits<R>::arg2_type OP;
      m_operations[cat].reports[&typeid(OP)].reset(new Report<OP>(report));
    }

    void report() const;

    void setDefaultTitleReport(TitleReport dtr) { defaultTitleReport = dtr;}

  private:
    void reportOneCat(Reporter::Categories::value_type const & icat) const;

  private:

    Categories m_operations;

    TitleReport defaultTitleReport;

  };


}


#endif //  Utilities_PerfTools_Reporter_H
