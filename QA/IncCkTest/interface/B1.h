// ---- file B1.h ----
#ifndef B1_H
#define B1_H

struct S;
struct B1 { 
  virtual ~B1(){}
  virtual S * s() const =0; 
};
#endif

