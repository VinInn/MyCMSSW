#ifndef Utilities_PerfTools_RealTime_H
#define Utilities_PerfTools_RealTime_H
//
//  defines "rdtsc"
//
#if defined(__powerpc__) || defined(__ppc__) 
#include "PPCRealTime.h"
#elif defined(__i386__) || defined(__ia64) || defined(__ia64__)
#include "PentiumRealTime.h"
#else
#error The file Utilities/PerfTools/interface/RealTime.h needs to be set up for your CPU type.
#endif

namespace perftools {

  typedef long long int TimeDiffType;
  typedef unsigned long long int TimeType;

  // High Precision real time in clock-units
  inline TimeType realTime() {
    return rdtsc();
  }

}


#endif //   Utilities_PerfTools_RealTime_H
