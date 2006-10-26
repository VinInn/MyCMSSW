// ---- file func1.cc ----

#include "QA/IncCkTest/interface/D1.h"

void f1(D1* d) {
  B1* p = d;
  S*  s2 = p->s();
}

