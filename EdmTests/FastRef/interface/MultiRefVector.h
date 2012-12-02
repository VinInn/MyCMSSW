#ifndef MultiRefVector_H
#define MultiRefVector_H

#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefCoreWithIndex.h"
#include <tuple>
#include<cstdint>
#include<vector>


template<typename R>
class Iter {
public:
  
  typedef typename R::value_type value_type;
  Iter(){}
  explicit Iter(edm::RefCore const & p, uint32_t const * ip) :
    product(&p), p(ip){}

  value_type const & operator*() const {
    Ref me(*product,*p);
    return *me;
  }
  
  Iter & operator++() { ++p; return *this;}
  bool operator!=(Iter const & rh) const { return p!=rh.p;}

  edm::RefCore const * product=nullptr;
  uint32_t const * p=nullptr; 
};


template<typename Ref>
class Range {
public:
  
  typedef typename Ref::value_type value_type;
  typedef Iter<Ref> const_iterator;
  explicit Range(RefCore const &  p, uint32_t const * ib, uint32_t const * ie) :
    prod(p),b(ib) e(ie){}
  
  const_iterator begin() const {
    return const_iterator(prod,b);
  }
  const_iterator end() const {
    return const_iterator(prod,e);
  }

  std::size_t size() const { return e-b;}
  
  value_type const & operator[](size_t i) const {
    Ref me(prod,index(i));
    return *me;
  }

  RefCore const &  product() const { return prod;}

  uint32_t index(size_t i) const { return *(b+i);}


private:
  RefCore const &  prod;
  uint32_t const * b; uint32_t const * e;
};


template<typename... Refs>
class MultiRefVector {
 public:
  typedef std::tuple<Refs...> Refs;  // for reference (sic...)
  typedef std::tuple<RefProd<Refs::product_type>...> Products;

  static constexpr std::size_t SIZE = std::tuple_size<Products>::value;
  static constexpr std::size_t size() { return SIZE;}

  template<int N>
    Range<typename std::tuple_element<N,Refs>::type> range(uint32_t b, uint32_t e) const {
    touch<N>();
    return Range<typename std::tuple_element<N,Refs>::type>(std::get<N>(products), &indeces[b],&indeces[e]);
  }

  template<int N>
  typename std::tuple_element<N,Products>::type::product_type const &
    product() const {
    return *std::get<N>(products);
  }

  template<int N>
    void set(std::tuple_element<N,TUPLE>::type const & prod) {
    std::get<N>(products) = prod;
  }

  // *T is a Ref or RefCoreWithIndex 
  template<typename T>
    uint32_t add(T b, T e) {
    for(b!=e; ++b) {
      // verify product?
      indeces.push_back((*b).index());
    }
    return indeces.size();
  }

  uint32_t indSize() const {
    return indeces.size();
  }

  template<int N> 
    void touch() const { *std::get<N>(products);}

  Products products;
  std::vector<uint32_t> indeces;

};


template<typename MRV>
struct Assoc {
  
  template<int N>
  Range<typename std::tuple_element<N,typename MRV::Refs>::type>
   range() const {
    return (*mrv).range<N>(offsets[N],offsets[N+1]);
  }

  // *T is a Ref, P is a RefProd corresponding to N in MRV
  template<int N, typename P, typename T>
  void add(P const & prod, T b, T e) {
    mrv.set<N>(prod);
    offsets[N] = (*mrv).indSize();
    offsets[N+1] = (*mrv).add(b,e);
  }
   
  edm::RefProd<MRV> mrv;
  uint32_t offsets[MRV::SIZE+1];
};

#endif
