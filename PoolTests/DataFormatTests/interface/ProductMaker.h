#ifndef ProductMaker_H
#define ProductMaker_H

namespace edm {
  clasd EDProduct;
}

class ProductMaker {
public:

  virtual edm::EDProduct * make()=0;


};


template<typename Wrapper> 
class WrapperMaker : public ProductMaker{
public:
  typedef typename Wrapper::value_type Container;
  typedef typename Container::value_type Object;

  virtual edm::EDProduct * make() {
    //obvious assumption
    std::auto_ptr<Container> cont(new Container);
    (*cont).insert((*cont).end(),Object());
    return Wrapper(cont); 
  }
  

};

#endif
