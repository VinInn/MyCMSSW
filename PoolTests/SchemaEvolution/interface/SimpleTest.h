#ifndef SchemaEvolution_SimpleTest_H
#define SchemaEvolution_SimpleTest_H

#include<vector>
#include<algorithm>

namespace shevsp {

  struct B {
    B() : b(0){}
    virtual ~B(){}
    long long b;
    virtual bool operator==(B const & rh) const { return b==rh.b;}
    bool operator!=(B const & rh) const { return !((*this)==rh);}
  };

  struct A1 : public B {
    A1() : a(0){}
    float a;
    virtual bool operator==(A1 const & rh) const { 
      return B::operator==(rh) && a==rh.a;
    }
    bool operator!=(A1 const & rh) const { return !((*this)==rh);}
  };
  struct A2 : public B {
    A2() :a(0){}
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
  
  struct CompareB{
    inline bool operator()(B const * b1,B const * b2) {
      return (b1==0&&b2==0) || (b1&&b2&&((*b1)==(*b2)));
    }
 };

  struct V {
    char ac[5];
    A1 aa[5];
    std::vector<float> vf;
    std::vector<A1> va;
    std::vector<B*> vb;
    bool operator==(V const & rh) const { 
      return std::equal(ac,ac+5,rh.ac) && std::equal(aa,aa+5,rh.aa) &&
	vf==rh.vf && va==rh.va && 
	std::equal(vb.begin(),vb.end(),rh.vb.begin(),CompareB());
    }
    bool operator!=(V const & rh) const { return !((*this)==rh);}
    
  };

}
#endif
