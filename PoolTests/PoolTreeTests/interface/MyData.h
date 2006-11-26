#ifndef MyData_H
#define MyData_H


namespace MyData {

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
