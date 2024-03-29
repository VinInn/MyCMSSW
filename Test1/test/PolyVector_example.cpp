#include "PolyVector.h"
#include<iostream>

struct B{
  virtual ~B(){}
  virtual void b()=0;
};

struct A : public B{ void b(){} int i;};
struct C : public B{ void b(){} float f;};

template<typename T, int N> 
struct Array : public B{
  void b(){}
  T a[N];
};


template<typename T>
void print(edm::PolyVector<T> const & v) {
  for ( typename edm::PolyVector<T>::const_iterator p=v.begin(); p!=v.end();p++)
    std::cout << typeid(*p).name() << ", "; 
  if (!v.empty()) std::cout <<std::endl;
}




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

  print(v);

  for ( edm::PolyVector<B>::iterator p=w.begin(); p!=w.end();p++) {
    C * cc = dynamic_cast<C*>(&(*p)); if(cc) cc->f=3.14;
  }

  for ( edm::PolyVector<B>::const_iterator p=w.begin(); p!=w.end();p++)
    std::cout << typeid(*p).name() << ", "; 
  std::cout <<std::endl;

  edm::PolyVector<B> z;
  z.push_back(Array<double,6>());
  z.push_back(Array<bool,3>());

 
 for ( edm::PolyVector<B>::const_iterator p=z.begin(); p!=z.end();p++)
    std::cout << typeid(*p).name() << ", "; 
  std::cout <<std::endl;

  if (!v.verifyTypes()) std::cout << "problem with types in v" << std::endl;
  if (!w.verifyTypes()) std::cout << "problem with types in w" << std::endl;
  if (!z.verifyTypes()) std::cout << "problem with types in z" << std::endl;


  z.swap(v);

  for ( edm::PolyVector<B>::const_iterator p=v.begin(); p!=v.end();p++)
    std::cout << typeid(*p).name() << ", "; 
  std::cout <<std::endl;

  for ( edm::PolyVector<B>::const_iterator p=z.begin(); p!=z.end();p++)
    std::cout << typeid(*p).name() << ", "; 
  std::cout <<std::endl;

  // polymorfic copy: needs types defined
  w.clear();
  w.addType<A>();
  w.addType<C>(); 
  w.addType<Array<float,4> >();
  w.addType<Array<double,6> >();
  w.addType<Array<long long,7> >();
  try {
    std::copy(z.begin(),z.end(),std::back_insert_iterator< edm::PolyVector<B> >(w));
  }
  catch (const std::string & s) {
    std::cout << s << std::endl;
  }

  if (z.size()!=w.size())  std::cout << "problem with copy" << std::endl;
  
  if (!v.verifyTypes()) std::cout << "problem with types in v" << std::endl;
  if (!w.verifyTypes()) std::cout << "problem with types in w" << std::endl;
  if (!z.verifyTypes()) std::cout << "problem with types in z" << std::endl;

  w.clear();
  std::vector<A> va(4);
  std::vector<C> vc(8);
 
  // does not work without type init...
  w.addType<A>();
  w.addType<C>(); 
  try {
    std::copy(va.begin(),va.end(),std::back_insert_iterator< edm::PolyVector<B> >(w));
    std::copy(vc.begin(),vc.end(),std::back_insert_iterator< edm::PolyVector<B> >(w));
  }
  catch (const std::string & s) {
    std::cout << s << std::endl;
  }

  if (w.size()!=(va.size()+vc.size()))  std::cout << "problem with copy" << std::endl;

  w.clear();
  try {
    for (std::vector<A>::const_iterator p=va.begin();p!=va.end();p++)
      w.push_back(*p);
    for (std::vector<C>::const_iterator p=vc.begin();p!=vc.end();p++)
      w.push_back(*p);
  }
  catch (const std::string & s) {
    std::cout << s << std::endl;
  }

  if (w.size()!=(va.size()+vc.size()))  std::cout << "problem with copy" << std::endl;
 

  z.buildTypes();
  if (!z.verifyTypes()) std::cout << "problem with types after build" << std::endl;

  std::cout << "the end" << std::endl;

  return 0;
}
