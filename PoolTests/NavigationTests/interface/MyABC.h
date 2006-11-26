#ifndef MyABC_H
#define MyABC_H


#include<vector>
#include<string> 

// namespace NavigationTests  {

template<typename T>
struct R {
  std::string a;
};


  struct MA{
  MA(){}
  virtual ~MA(){}    
    int i;
  };
   
  struct MB;


  struct MCC;
  
  struct MB : public MA {
    MB(){}
    virtual ~MB(){}    
    int j;
    R<MCC> c;    
  };
  
  struct MCC : public MA {
    MCC(){}
    virtual ~MCC(){}    
    std::vector<R<MB> > bs;
    R<MCC> parent;
  };
  
  
  template<typename T>
  struct MD : public MB {
    MD(){}
    virtual ~MD(){}    
    std::vector<T> data;    
  };
// }


#endif
