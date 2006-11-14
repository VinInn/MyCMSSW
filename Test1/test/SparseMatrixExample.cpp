#include "SparseMatrix.h"
#include <iostream>
#include <valarray>


template<class T> 
void print(const std::valarray<T>& v) { 
  for (int i=0;i<v.size();i++)
    std::cout << v[i] << ' ';
  std::cout << std::endl;
}

int main() {

  typedef std::valarray<double> VD;  
  
  double vd[] = {1.2,3.4,5.,6.,8.};
  VD v(vd,5);
  
  VD vr(0.,5);
  
  
  SparseMatrix<double> m(5,1.0);
  
  std::cout << m << std::endl;
  
  print(v);
  print(vr);
  std::cout << std::endl;
  
  {
    SparseMatrix<double>::const_iterator p = m.values().begin();
    SparseMatrix<double>::const_iterator e = m.values().end();
    for (p;p!=e;p++)
      vr[(*p).i] += (*p).v*v[(*p).j];
  }
  
  print(vr);
  std::cout << std::endl;
  
  double vrot[][2] = { {0.,0.5,}, {-0.5, 0.} };
  
  SparseMatrix<double> rot(2,vrot);
  
  std::cout << rot << std::endl;
  
  double vx[] = {1.0,0.};
  
  VD x(vx,2);
  VD x1(0.,2);
  {
    SparseMatrix<double>::const_iterator p = rot.values().begin();
    SparseMatrix<double>::const_iterator e = rot.values().end();
    for (p;p!=e;p++)
      x1[(*p).i] += (*p).v*x[(*p).j];
  }
  
  print(x);
  print(x1);
  
  
  std::cout << rot(0,0) << " " << rot(1,0) << std:: endl;
  rot(0,0) = 1.0;
  rot(1,1) = 1.0;
  rot(1,0) = 0.5;
  
  std::cout << rot << std::endl;
  
  
};


