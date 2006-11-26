#ifndef Traits_H
#define Traits_H



class T1;
class T2;


template<typename T>
struct TT{
  virtual ~TT(){}
  int i;
};

template<typename T>
struct Trans{
  T * get() { return (T*)(p);}
  void * p;
};


struct MYTrans {
int i;
};

struct TMe{
  TMe(){
    me.p = new MYTrans;
    (*me.get()).i=3;
  }
  virtual ~TMe(){ delete me.get();}
  int i;
  Trans<MYTrans> me;
};

struct TVoid{
  TVoid(){
    p = new MYTrans;
    get()->i=3;
  }
  virtual ~TVoid(){ delete get();}
  MYTrans * get() { return (p);}
  MYTrans * p;
};

#endif // Traits_H
