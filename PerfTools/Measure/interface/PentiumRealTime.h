#ifndef Utilities_PerfTools_PENTIUMRealTime_H
#define Utilities_PerfTools_PENTIUMRealTime_H

namespace perftools {

  inline unsigned long long int rdtsc() {
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
  }

}

#endif //  Utilities_PerfTools_PENTIUMRealTime_H

