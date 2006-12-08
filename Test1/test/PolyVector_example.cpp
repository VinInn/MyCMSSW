#include "PolyVector.h"
#include<iostream>

struct B{
  virtual ~B(){}
};

struct A : public B{};
struct C : public B{};



int main() {



  edm::PolyVector<B> v;

  A a; C c;

  v.push_back(a);
  v.push_back(c);

  std::cout << typeid(v[0]).name() << std::endl;
  std::cout << typeid(v[1]).name() << std::endl;

  for ( edm::PolyVector<B>::const_iterator p=v.begin(); p!=v.end();p++)
    std::cout << typeid(*p).name() << std::endl;


  return 0;
}
