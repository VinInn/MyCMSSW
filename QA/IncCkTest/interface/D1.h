// ---- file D1.h ----
#ifndef D1_H
#define D1_H

#include "QA/IncCkTest/interface/B1.h"

struct D1 : public B1 { 
  virtual ~D1(){}
  virtual S * s() const {
    return ptr; 
  } 
  S* ptr;
};
#endif

