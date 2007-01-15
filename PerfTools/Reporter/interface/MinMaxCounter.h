#ifndef PerfTools_MinMaxCounter_H
#define PerfTools_MinMaxCounter_H
#include <limits>
#include <cmath>

namespace perftools {

  /* a class to count occurences, total amount, as well as min and max 
   */
  class MinMaxCounter {
  public:
    MinMaxCouter() :  m_counts(0), m_tot(0),
		      m_min(std::numeric_limits<double>::max()),
		      m_max(-m_min))
    {}

    void fill(double amount = 0.) {
      m_counts++;
      m_tot+=amount;
      if (amount < m_min)  m_min=amount;
      if (m_max  < amount) m_max=amount;
    }

  public:
    int    m_counts;
    double m_tot;
    double m_min;
    double m_max;

  };

}


#endif // PerfTools_MinMaxCounter_H
