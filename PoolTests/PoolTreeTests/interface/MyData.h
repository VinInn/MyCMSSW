#ifndef MyData_H
#define MyData_H
#include<string>

namespace MyData {

  struct Tag {
    std::string name;
    unsigned int id;
  };

  struct Tr {
    Tr(): x(0),n(0){}
    virtual ~Tr(){}
    float x;
    int n;
    
  };
  
  struct Vx {
    Vx(): x(0),n(0){}
    virtual ~Vx(){}
    float x;
    int n;
    
  };

}



#endif
