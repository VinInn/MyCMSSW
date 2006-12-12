#ifndef PolyPerformanceStubs_H
#define PolyPerformanceStubs_H

namespace polyPerformance {

  struct Base {
    virtual ~Base(){}
    virtual void action() const=0;
    // because of ownvector
    virtual Base * clone() const=0;
  };

  template<typename T, int N> 
  struct Array : public Base{
    typedef Array<T,N> self;
    enum { SIZE=N};
    typedef T value_type;
    virtual void action() const {
    }
    virtual self * clone() const {return new self(*this);}
    T data[N];
  };

}


#endif  // PolyPerformanceStubs_H
