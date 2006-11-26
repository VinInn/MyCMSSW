#include "PoolTests/NavigationTests/interface/ABC.h"

using namespace NavigationTests;

pool::IDataSvc * B::c_dsvc=0;

A::A(){}
A::~A(){}
S A::s;

#include<iostream>
using std::cout;
using std::endl;

bool CC::verbose(false);
    
 CC::CC() {
  if (verbose) std::cout << "constr CC" << std::endl; 
}
CC::~CC(){
  if (verbose) std::cout << "destr CC" << std::endl; 
}

H::H(): i(-99) {
  if (verbose) cout << "def constr H" << endl;
}
    
bool H::verbose(false);

H::~H() {
  if (verbose) cout << "destr H " << i << endl;
}

H::H(int ii): i(ii) {
  if (verbose) cout << "standard constr H "  << i << endl;
  
}
H::H(const H&rh): i(rh.i) {
  if (verbose) cout << "copy constr H "  << i << endl;
  
}

H& H::operator=(const H&rh) {
  i = rh.i;
  if (verbose) cout << "ass H "  << i << endl;
  return *this;  
}
