#ifndef Synthetic_BaseBranch_H
#define Synthetic_BaseBranch_H

namespace synthetic {

  class  BaseBranch {
  public:
    virtual ~BaseBranch() {}
    virtual void clean()=0;
    virtual  void write(int globalCount)=0;
  };

}


#endif // Synthetic_BaseBranch_H
