#include "PolyVector.h"


struct B{};

struct A : public B{};
struct C : public B{};



int main() {



  edm::PolyVector<B> v;

  A a; C c;

  v.push_back(a);
  v.push_back(c);

  return 0;
}
