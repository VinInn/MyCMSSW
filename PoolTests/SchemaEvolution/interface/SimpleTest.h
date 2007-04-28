#ifndef SchemaEvolution_SimpleTest_H
#define SchemaEvolution_SimpleTest_H

namespace shevsp {

  struct B {
    virtual ~B(){}
    int b;
  };

  struct A1 : public B {float a;};
  struct A2 : public B {double a;};

  struct C {
    C() : i(0),b(0) {}
    typedef A1 M1;
    typedef A2 M2;
    int i;
    M1 m1;
    M2 m2;
    B * b;

  };
  

}
#endif
