#ifndef DetVectorPerformanceStubs_H
#define DetVectorPerformanceStubs_H

#include<vector>
#include<algorithm>
#include<iterator>
#include<utility>

namespace  detVectorPerformance {

  template<typename T, int N> 
  struct Array : public Base{
    typedef Array<T,N> self;
    enum { SIZE=N};
    typedef T value_type;
    void action() const {
    }
    T data[N];
  };


  template<typename T> DoubleVector {
    typedef std::vector<T> BaseCont;
    typedef std::pair<int, BaseCont> Elem;
    typeded std::pair<Elem> Cont;
    typedef std::pair<BaseCont::const_iterator, BaseCont::const_iterator> Range;
    

    void insert(int i, BaseCont& buffer) {
      cont.resize(cont.size()+1);
      cont.back().first=1;
      cont.back().second.swap(buffer);
    }

    Range operator[](int i) const {
      return invalid();
    }

    void postSort() {}

    Range invalid() const {
      static BaseCont l;
      return Range(l.end(),l.end());
    }

    Cont cont;


  };

 template<typename T> IndexedVector {
    typedef std::vector<T> BaseCont;
    struct Elem {
      int id;
      int loc;
      int size;
      bool operator<(Elem const & rh) const { return id<rh.id;}
    };
    typedef std::vector<Elem > Index;
    typedef std::pair<BaseCont::const_iterator, BaseCont::const_iterator> Range;
 

    void insert(int i, BaseCont& buffer) {
      Elem e = {i,cont.size(),buffer.size()};
      Index::iterator p = std::upper_range(index.begin(),index.end(),e);
      index.insert(p,e);
      cont.reserve(cont.size()+buffer.size());
      std::copy(buffer.begin(),buffer.end(),std::back_inserter(cont));
    }

    Range operator[](int i) const {
      return invalid();
    }

    void postSort() {}

    Range invalid() const {
      static BaseCont l;
      return Range(l.end(),l.end());
    }


    Index index;
    BaseCont cont;
  };



};




#endif //  DetVectorPerformanceStubs_H
