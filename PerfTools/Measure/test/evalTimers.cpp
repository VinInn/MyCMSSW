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
  inline void waisteCPU() {
    for (double i=1;i<1000000;i++)
      gcrap+=std::log(std::sqrt(i));
  }

  inline void nap() {
    ::timespec req;
    req.tv_sec = 1;
    req.tv_nsec = 00000;
    ::timespec rem;
    ::nanosleep(&req,&rem);
  }

}


template<typename S>
void checkTime(S source) {
  
  //  typedef typename boost::function_traits<S>::result_type T;
  typedef typename boost::function_types::result_type<S>::type T;
  
  T i = source();

  std::cout << "init value " << i << std::endl;
  
  std::cout << "min diff " << source()-source() << std::endl;
  
  waisteCPU();

  T a = source();
  T b = source();
  std::cout << "min diff " << b-a << std::endl;
  std::cout << "cpu diff " << a-i << std::endl;

  nap();
  T n = source();
  std::cout << "nap diff " << n-b << std::endl;

  
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
