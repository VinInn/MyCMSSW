#include "PerfTools/Measure/interface/RealTime.h"

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/function_types/result_type.hpp>


#include <ctime>
#include <cmath>
#include <typeinfo> 
#include <iostream>

namespace {

  double gcrap=0;
  void waiste() {
    for (double i=1;i<100000;i++)
      gcrap+=std::log(i);
  }
}


template<typename S>
void checkTime(S source) {
  
  //  typedef typename boost::function_traits<S>::result_type T;
  typedef typename boost::function_types::result_type<S>::type T;
  
  T i = source();

  std::cout << "init value " << i << std::endl;
  
  std::cout << "min diff " << source()-source() << std::endl;
  
  waiste();

  T a = source();
  T b = source();
  std::cout << "min diff " << b-a << std::endl;
  
  T c = source();
  double d = double(source()-c);
  std::cout << "min diff " << d << std::endl;
 
  std::cout << "end value " << source() << std::endl;
  std::cout << "tot diff " << source()-i << std::endl;
  std::cout << std::endl;
 
}

#define CHECKTIME(S) \
  std::cout << "checking source " << #S << std::endl;	\
  checkTime(&S)




int main() {

  CHECKTIME(perftools::realTime);
  CHECKTIME(std::clock);


    return 0;
}
