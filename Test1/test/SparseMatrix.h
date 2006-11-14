#ifndef SparseMatrix_H
#define SparseMatrix_H
#include <vector>
#include <utility>
#include <algorithm>

// a sparse square matrix with minimal accessors


template<typename T>
class SparseMatrix {

public:
  typedef int size_type;
  struct Element {
    Element(size_type ii, size_type jj, T vv) :
      i(ii),j(jj),v(vv){}
    size_type i;
    size_type j;
    T v;
    bool operator<(const Element& rh) const {
      return less(rh.i,rh.j);
    }
    bool less(size_type ii, size_type jj) const {
      return (i<ii) || (!(ii<i))&&(j<jj);
    }
    bool equal(size_type ii, size_type jj) const {
      return i==ii && j==jj;
    }
  };

  typedef Element value_type;
  typedef  std::vector<value_type> Container;
  typedef typename Container::iterator iterator;
  typedef typename Container::const_iterator const_iterator;


  // initialize it empty...
  explicit SparseMatrix(size_type r) : m_rank(r) {}

  // initialize it diagonal ...
  explicit SparseMatrix(size_type r, T t) : m_rank(r) 
  {
    m_values.reserve(m_rank);
    for (size_type row=0; row!=m_rank; row++)
      m_values.push_back(value_type(row,row,t));
  }
  // initialize it with a Matrix
  template<typename M>
  explicit SparseMatrix(size_type r, const M& m) : m_rank(r) 
  {
    m_values.reserve(m_rank);
    for (size_type row=0; row!=m_rank; row++)
      for (size_type col=0; col!=m_rank; col++)
	if (m[row][col]!=0)
	  m_values.push_back(value_type(row,col,m[row][col]));
  }
  
  // slow
  T operator()(size_type ii, size_type jj) const {
    value_type e(ii,jj,0);
    const_iterator p = std::lower_bound(values().begin(),values().end(),e);
    return p!=values().end()&&(*p).equal(ii,jj) ? (*p).v : T(0);
  }

  // slow
  T &  operator()(size_type ii, size_type jj) {
    value_type e(ii,jj,0);
    iterator p = std::lower_bound(values().begin(),values().end(),e);
    return p!=values().end()&&(*p).equal(ii,jj)  ? (*p).v :
      (*values().insert(p,e)).v;
  }
  
  // slow
  iterator insert(size_type ii, size_type j, T v) {
    if (v==0) return;
    value_type e(ii,jj,v);
    iterator p = std::lower_bound(values().begin(),values().end(),e);
    if (p!=values().end()&&(*p).equal(ii,jj) )  (*p).v=e;
    else p = values().insert(p,e);
    return p;
  }

  size_type rank() const { return m_rank;}
  
  const Container & values () const { return m_values;}
  Container & values ()  { return m_values;}
  
private:
  
  Container m_values;
  size_type m_rank;
  
};

#include<ostream>
template<typename T>
std::ostream & operator<<(std::ostream & os, const SparseMatrix<T> & m) {
  typename SparseMatrix<T>::const_iterator p = m.values().begin();
  typename SparseMatrix<T>::const_iterator e = m.values().end();
  for (typename SparseMatrix<T>::size_type row=0; row!=m.rank(); row++) {
    for (typename SparseMatrix<T>::size_type col=0; col!=m.rank(); col++) {
      if ( (*p).equal(row,col)) { 
	os <<  (*p).v; p++;
      } else os << 0.;
      os << " ";
    }
    os << "\n";
  }
  return os;
}

#endif
