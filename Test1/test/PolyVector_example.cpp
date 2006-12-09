#include "PolyVector.h"
#include<iostream>

struct B{
  virtual ~B(){}
};

struct A : public B{};
struct C : public B{};

template<typename T, int N> 
struct Array : public B{
  T a[N];
};



int main() {



  edm::PolyVector<B> v;

  A a; C c;

  v.push_back(a);
  v.push_back(c);
  v.push_back(Array<float,4>());
  v.push_back(Array<double,6>());
  v.push_back(Array<long long,7>());
  v.push_back(c);
  v.push_back(Array<double,6>());

  edm::PolyVector<B> w(v);
  w.push_back(a);

  std::cout << typeid(v[0]).name() << std::endl;
  std::cout << typeid(v[1]).name() << std::endl;

  for ( edm::PolyVector<B>::const_iterator p=v.begin(); p!=v.end();p++)
    std::cout << typeid(*p).name() << ", "; 
  std::cout <<std::endl;

  for ( edm::PolyVector<B>::const_iterator p=w.begin(); p!=w.end();p++)
    std::cout << typeid(*p).name() << ", "; 
  std::cout <<std::endl;

  edm::PolyVector<B> z;
  z.push_back(Array<double,6>());
  z.push_back(Array<bool,3>());

  for ( edm::PolyVector<B>::const_iterator p=z.begin(); p!=z.end();p++)
    std::cout << typeid(*p).name() << ", "; 
  std::cout <<std::endl;

  z.swap(v);

  for ( edm::PolyVector<B>::const_iterator p=v.begin(); p!=v.end();p++)
    std::cout << typeid(*p).name() << ", "; 
  std::cout <<std::endl;

  for ( edm::PolyVector<B>::const_iterator p=z.begin(); p!=z.end();p++)
    std::cout << typeid(*p).name() << ", "; 
  std::cout <<std::endl;

  
  if (!v.verifyTypes()) std::cout << "problem with types in v" << std::endl;
  if (!w.verifyTypes()) std::cout << "problem with types in w" << std::endl;
  if (!z.verifyTypes()) std::cout << "problem with types in z" << std::endl;

  std::cout << "the end" << std::endl;

  return 0;
}
