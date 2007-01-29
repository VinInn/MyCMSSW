#ifndef PerfTools_SimpleImmediateReporter_H
#define PerfTools_SimpleImmediateReporter_H

#include <sstream>
#include <ostream>
#include <string>

namespace perftools {

  class SimpleImmediateReporter {
  public:
    SimpleImmediateReporter(std::ostream & co, int fields, 
			    std::string const & sep=" ") :
      m_co(co), m_count(0), m_fields(fields), m_sep(sep) {}

    template<typename T>
    void operator()(char const * name, T const & t) {
      if (m_count>0) m_os << m_sep;
      if (name) m_os << name;
      m_os << t;
      m_count++;
      if (m_count==m_fields) flush();
    }

    void flush() {
      m_co <<  m_os.str() << std::endl;
      m_count=0;
      m_os.str()="";
    }

  private:
    std::ostream & m_co;
    std::ostringstream m_os;
    int m_count;
    int m_fields;
    std::string m_sep;
  };

}

#endif // PerfTools_SimpleImmediateReporter_H
