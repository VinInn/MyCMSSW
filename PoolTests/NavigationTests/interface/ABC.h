#ifndef ABC_H
#define ABC_H


#include "DataSvc/Ref.h"
#include<map>
#include<vector>
#include<bitset>
#include<iostream>
#include<string>


namespace NavigationTests  {


  struct Simple {
    int j;
    float a;
    double b;

  };
  
  class Base {
  public:
    virtual ~Base(){}
    const std::string& myName() const { return name;}
    const std::string& myName(const std::string& iname) { return name=iname;}
    std::string name;
    int q;
  };

  struct Compound : public Base {
    int i;
    Simple a;
    Simple b;
    std::vector<Simple> v;
    Simple * p;
    std::map<int,std::string> m;
  };


struct AT{
  AT(){}
  virtual ~AT(){}
};

struct BT{
  BT(){}
  virtual ~BT(){}    
  int k;
};

class T2;

 class T1 : public AT, public BT {
 public:
    T1(){}
    virtual ~T1(){}    
    int j;
    pool::Ref<const T2> c;   
 };


class T2 : public AT {
public:
    T2(){}
    virtual ~T2(){}
    virtual void bha() {}
    std::vector<pool::Ref<AT> > bs;
};

struct S {

  int l;

};


  template<class T>
  struct AP {
    
    T t;
  };
  
  
  struct AS {
    AP<BT*> p;

  };

  struct A{
    static S s;
    A();
    virtual ~A();    
    int i;
    pool::Ref<A> previous;
  };
  
  struct AV : public A{
    AV(){}
    virtual ~AV(){}
    const AV * me() const { return this;}
    virtual size_t size() const = 0;
  };
  




  struct B;

  struct Tr{
    int j;
    void * p;
  };

  struct CC;
  
  struct CBase {
    virtual ~CBase(){}
    
    virtual void bha(){}
    int kk;
    
  };

  

  struct B : public A {

    static pool::IDataSvc * c_dsvc;
    B() : c(c_dsvc) {}
    explicit B(int jj) : j(jj) {}
    virtual ~B(){}    
    int j;
    pool::Ref<CC> c;   
    //    CC * tc_;
  };
  
  struct CC : public AV, public CBase {
    static bool verbose;
    CC();
    virtual ~CC();
    size_t size() const { return bs.size();}
    virtual void bha() {}
    std::vector<pool::Ref<B> > bs;
    pool::Ref<CC> parent;
  };
  
  struct K  {
    K(){}
    virtual ~K(){}    
   double d;
    Tr t_;
    
  };
  
  template<typename T>
    struct D : public B {
      D(){}
      virtual ~D(){
      }    
      std::vector<T> data;    
      Tr * t_;
      // A* ta_;
    };
  
  template<typename T>
    struct DP : public B {
      DP(){}
      virtual ~DP(){
	for ( typename std::vector<T*>::iterator p=data.begin();
	      p!=data.end();
	      p++) 
	  delete (*p);
      }    
      std::vector<T*> data;    
      std::vector<T*> ref;    
      Tr * t_;
      // A* ta_;
    };

  struct H {
    static bool verbose;
    H();
    virtual ~H();
    explicit H(int ii);
    H(const H&rh);
    H& operator=(const H&rh);
    
    int i;
  };


  struct Bits : public std::bitset<48> {
    virtual ~Bits(){}
  };
  
}


#endif
