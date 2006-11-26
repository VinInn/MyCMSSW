#ifndef VinTestP_Fields_H
#define VinTestP_Fields_H
#include<vector>

namespace VinTestP {

  struct Fields {
    unsigned int PFN : 22;
    int : 2;
    unsigned int CCA :3;
    bool nonreachable : 1;
    bool dirty : 1;
    bool valid : 1;
    bool global : 1;

  };



  struct Digi {

    Fields w;
  };


  struct Data : public std::vector<Digi> {
    virtual ~Data(){}
  };
  

}

#endif // VinTestP_Fields_H
