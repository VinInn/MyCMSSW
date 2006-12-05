#ifndef ProductMaker_H
#define ProductMaker_H

#include "PoolTests/DataFormatTests/interface/TypeName.h"


#include "boost/mpl/if.hpp"
#include <memory>
#include <vector>
#include <ostream>
#include <string>


namespace edm {
  class EDProduct;
}

class ProductMaker {
public:
  virtual ~ProductMaker(){}
  virtual edm::EDProduct * make()=0;  

  virtual void message(std::ostream & os) const =0;

};

class MakerMessage {
public:
  MakerMessage() : 
    m_trailer("[DataFormatTest] ")
{}
 
  virtual ~MakerMessage() {}
 
  void message(std::ostream & os) const {
    os << trailer();
    makeMessage(os);
  }
  
  std::string const & trailer() const { return m_trailer;}

  template<typename T> static void typeAndSize(std::ostream & os) {
    os << VUtils::TypeName<T>::name() << ", " << sizeof(T);
  }

  
private:
  
  virtual void makeMessage(std::ostream & os) const=0;
  

  const std::string m_trailer;
};

template<typename Wrapper> 
struct MakeSimple : public  MakerMessage {
  typedef typename Wrapper::value_type Object;
  Wrapper * operator()() {
    std::auto_ptr<Object> o(new Object);
    return new Wrapper(o); 
  }
private:
  void makeMessage(std::ostream & os) const {
    os << "simple Product ";
    typeAndSize<Wrapper>(os); os<<" "; 
    typeAndSize<Object>(os);
  }
};

template<typename Wrapper> 
struct MakeContainerPB  : public  MakerMessage {
  typedef typename Wrapper::value_type Container;
  typedef typename Container::value_type Object;
  Wrapper * operator()() {
   std::auto_ptr<Container> cont(new Container);
   (*cont).push_back(Object());
    return new Wrapper(cont); 
  }

  void makeMessage(std::ostream & os) const {
    os << "Container Product ";
    typeAndSize<Wrapper>(os); os<<" "; 
    typeAndSize<Container>(os); os<<" "; 
    typeAndSize<Object>(os);
  }

};

template<typename Wrapper> 
struct MakeContainerIn  : public  MakerMessage {
  typedef typename Wrapper::value_type Container;
  typedef typename Container::value_type Object;
  Wrapper * operator()() {
   std::auto_ptr<Container> cont(new Container);
    (*cont).insert(Object());
    return new Wrapper(cont); 
  }

  void makeMessage(std::ostream & os) const {
    os << "Container Product ";
    typeAndSize<Wrapper>(os); os<<" "; 
    typeAndSize<Container>(os); os<<" "; 
    typeAndSize<Object>(os);
  }

};

template<typename Wrapper> 
struct MakeRangeMap  : public  MakerMessage {
  typedef typename Wrapper::value_type RangeMap;
  typedef typename RangeMap::value_type Object;
  typedef typename RangeMap::mapType::key_type ID;
  Wrapper * operator()() {
   std::auto_ptr<RangeMap> cont(new RangeMap);
   std::vector<Object> v; v.push_back(Object()); v.push_back(Object());
   (*cont).put(ID(),v.begin(),v.end());
    return new Wrapper(cont); 
  }

  void makeMessage(std::ostream & os) const {
    os << "RangeMap Product ";
    typeAndSize<Wrapper>(os); os<<" "; 
    typeAndSize<RangeMap>(os); os<<" "; 
    typeAndSize<Object>(os); os<<" "; 
    typeAndSize<ID>(os);
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



  template <typename T, void (T::*)( typename T::value_type const &)>  struct pb_function;
  template <typename T> no_tag  has_pb_helper(...);
  template <typename T> yes_tag has_pb_helper(pb_function<T, &T::push_back> * dummy);
  
  template<typename T>
  struct has_push_back
  {
    static bool const value = 
      sizeof(has_pb_helper<T>(0)) == sizeof(yes_tag);
  };

  template <typename T, void (T::*)( typename T::value_type const &)>  struct ins_function;
  template <typename T> no_tag  has_ins_helper(...);
  template <typename T> yes_tag has_ins_helper(pb_function<T, &T::insert> * dummy);
  
  template<typename T>
  struct has_insert
  {
    static bool const value = 
      sizeof(has_ins_helper<T>(0)) == sizeof(yes_tag);
  };

  // very had-oc
  template <typename T, typename V>  struct mapType;
  template <typename T> no_tag  is_RangeMap_helper(...);
  template <typename T> yes_tag is_RangeMap_helper(value_type<T, typename T::mapType> * dummy);
  
  template<typename T>
  struct is_RangeMap
  {
    static bool const value = 
      sizeof(is_RangeMap_helper<T>(0)) == sizeof(yes_tag);
  };

}


template<typename Wrapper> 
class WrapperMaker : public ProductMaker{
public:
  typedef typename Wrapper::value_type Container;
  
  // container = has value_type (maybe use something else)
  // check if RangeMap, has push_back or insert
  // if not write it as is....
  typename boost::mpl::if_c<details::has_value_type<Container>::value,
			    typename boost::mpl::if_c< 
			      details::is_RangeMap<Container>::value,
			      MakeRangeMap<Wrapper>, 
			      typename boost::mpl::if_c<details::has_push_back<Container>::value, 
							MakeContainerPB<Wrapper>,  
							typename boost::mpl::if_c<details::has_insert<Container>::value,
										  MakeContainerIn<Wrapper>, MakeSimple<Wrapper> >::type
							>::type >::type,
			    MakeSimple<Wrapper>  >::type maker;

  virtual edm::EDProduct * make() {
    return maker();
  }
  
  virtual void message(std::ostream & os) const {
    maker.message(os);
  }
  
  
};

#endif
