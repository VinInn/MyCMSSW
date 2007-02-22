#ifndef PerfTools_ImmediateReporterWithNames_H
#define PerfTools_ImmediateReporterWithNames_H

#include <sstream>
#include <ostream>
#include <string>
#include <vector>

namespace perftools {

  class ImmediateReporterWithNames {
  public:
    ImmediateReporterWithNames(std::ostream & co, 
			       std::vector<std::string> const & fields,
			       std::string const & iname=" ",
			       std::string const & sep=" ") :
      m_co(co), m_count(0), m_fields(fields), 
      m_name(iname), m_sep(sep) {}
    
    template<typename T>
    void operator()(T const & t) {
      if (m_count>0) m_os << m_sep;
      else m_os << m_name;
      m_os << m_fields[m_count];
      m_os << t;
      m_count++;
      if (m_count==m_fields.size()) flush();
    }

    void flush() {
      m_co <<  m_os.str() << std::endl;
      m_count=0;
      m_os.str("");
    }

    std::ostream & stream() { return m_co; }

  private:
    std::ostream & m_co;
    std::ostringstream m_os;
    int m_count;
    std::vector<std::string> m_fields;
    std::string m_name;
    std::string m_sep;
  };

}

#endif // PerfTools_ImmediateReporterWithNames_H
