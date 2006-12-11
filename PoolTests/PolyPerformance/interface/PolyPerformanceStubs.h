#ifndef PolyPerformanceStubs_H
#define PolyPerformanceStubs_H

namespace polyPerformance {

  struct Base {
    virtual ~Base(){}
    virtual void action() const=0;
  };

  template<typename T, int N> 
  struct Array : public Base{
    enum { SIZE=N};
    typedef T value_type;
    virtual void action() const {
    }
    T data[N];
  };

}


#endif  // PolyPerformanceStubs_H
