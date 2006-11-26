#ifndef MyMap_H
#define MyMap_H

#include<set>
#include<vector>
#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/object_pool.hpp>

struct MM {
  MM(){}
  MM(int ia,int ib) : a(ia),b(ib){}
  int a;
  int b;

};

struct LessMM {
  bool operator()(const MM&rh, const MM&lh) const {
    return (rh.a<lh.a)||(!(lh.a<rh.a)&&(rh.b<lh.b));
  }

};



struct MyMap {

  virtual ~MyMap(){}

  typedef boost::fast_pool_allocator<MM,
      boost::default_user_allocator_new_delete,
      boost::details::pool::null_mutex> fast_alloc;


  std::set<MM,LessMM> mesO;
  std::set<MM,LessMM,fast_alloc> mes;

  std::vector<MM,fast_alloc> mev;
};



#endif // MyMap_H
