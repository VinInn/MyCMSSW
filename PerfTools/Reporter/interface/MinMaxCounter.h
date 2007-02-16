#ifndef PerfTools_MinMaxCounter_H
#define PerfTools_MinMaxCounter_H
#include <limits>
#include <cmath>

namespace perftools {

  /* a class to count occurences, total amount, as well as min and max 
   */
  class MinMaxCounter {
  public:
    MinMaxCounter() :  m_counts(0), m_tot(0),
		       m_min(std::numeric_limits<double>::max()),
		       m_max(-m_min)
    {}
    
    void fill(double amount = 0.) {
      m_counts++;
      m_tot+=amount;
      if (amount < m_min)  m_min=amount;
      if (m_max  < amount) m_max=amount;
    }

    double ave() const {
      return m_counts>0 ? m_tot/double(m_counts) : 0.0;
    }

    double trunc_ave() const {
      return m_counts>2 ? (m_tot-m_min-m_max)/double(m_counts-2) : ave();
    }

  public:
    int    m_counts;
    double m_tot;
    double m_min;
    double m_max;
    
  };
  
}


#endif // PerfTools_MinMaxCounter_H
