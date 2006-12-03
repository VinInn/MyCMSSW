#ifndef ProductMaker_H
#define ProductMaker_H

#include "boost/mpl/if.hpp"
#include <memory>

namespace edm {
  class EDProduct;
}

class ProductMaker {
public:
  virtual ~ProductMaker(){}
  virtual edm::EDProduct * make()=0;
  
  
};


template<typename Wrapper> 
struct MakeSimple {
  typedef typename Wrapper::value_type Object;
  Wrapper * operator()() {
    std::auto_ptr<Object> o(new Object);
    return new Wrapper(o); 
  }
};

template<typename Wrapper> 
struct MakeContainer {
  typedef typename Wrapper::value_type Container;
  typedef typename Container::value_type Object;
  Wrapper * operator()() {
   std::auto_ptr<Container> cont(new Container);
    (*cont).insert((*cont).end(),Object());
    return new Wrapper(cont); 
  }
};

namespace details {

  typedef char (& no_tag )[1]; // type indicating FALSE
  typedef char (& yes_tag)[2]; // type indicating TRUE

  template <typename T, typename V>  struct value_type;
  template <typename T> no_tag  has_value_helper(...);
  template <typename T> yes_tag has_value_helper(value_type<T, typename T::value_type> * dummy);
  
  
  template<typename T>
  struct has_value_type
  {
    static bool const value = 
      sizeof(has_value_helper<T>(0)) == sizeof(yes_tag);
  };
}


template<typename Wrapper> 
class WrapperMaker : public ProductMaker{
public:
  typedef typename Wrapper::value_type Container;

  virtual edm::EDProduct * make() {
    typename boost::mpl::if_c<details::has_value_type<Container>::value, 
      MakeContainer<Wrapper>, 
      MakeSimple<Wrapper> >::type maker;

    return maker();
  }
  
};

#endif
