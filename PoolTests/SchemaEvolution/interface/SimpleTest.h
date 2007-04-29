#ifndef SchemaEvolution_SimpleTest_H
#define SchemaEvolution_SimpleTest_H

namespace shevsp {

  struct B {
    virtual ~B(){}
    long long b;
    virtual bool operator==(B const & rh) const { return b==rh.b;}
    bool operator!=(B const & rh) const { return !((*this)==rh);}
  };

  struct A1 : public B {
    float a;
    virtual bool operator==(A1 const & rh) const { 
      return B::operator==(rh) && a==rh.a;
    }
    bool operator!=(A1 const & rh) const { return !((*this)==rh);}
  };
  struct A2 : public B {
    double a;
    virtual bool operator==(A2 const & rh) const { 
      return B::operator==(rh) && a==rh.a;
    }
    bool operator!=(A2 const & rh) const { return !((*this)==rh);}
  };

  struct C {
    C() : i(0),b(0) {}
    typedef A1 M1;
    typedef A2 M2;
    int i;
    M1 m1;
    M2 m2;
    B * b;
    bool operator==(C const & rh) const { 
      return i==rh.i && m1==rh.m1 && m2==rh.m2 && 
	( (b==0&&rh.b==0) || (b&&rh.b&&((*b)==(*rh.b))));
    }
    bool operator!=(C const & rh) const { return !((*this)==rh);}

  };
  

}
#endif
