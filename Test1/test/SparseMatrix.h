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
  typedef std::pair<iterator, iterator> range;
  typedef std::pair<const_iterator,const_iterator> const_range;

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
  
  const_range row(size_type i) const {
    value_type b(i,0,0);
    value_type e(i+1,0,0);
    const_iterator f = std::lower_bound(values().begin(),values().end(),b);
    const_iterator s = std::lower_bound(values().begin(),values().end(),e);
    return const_range(f,s);
  }

  range row(size_type i)  {
    value_type b(i,0,0);
    value_type e(i+1,0,0);
    iterator f = std::lower_bound(values().begin(),values().end(),b);
    iterator s = std::lower_bound(values().begin(),values().end(),e);
    return range(f,s);
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
  iterator insert(size_type ii, size_type jj, T v) {
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


template<typename V, typename T>
V operator*(SparseMatrix<T> const & m, V const & vi) {
  V vo(T(0),m.rank());
  SparseMatrix<double>::const_iterator p = m.values().begin();
  SparseMatrix<double>::const_iterator e = m.values().end();
  for (;p!=e;p++)
    vo[(*p).i] += (*p).v*vi[(*p).j];
  return vo;

}

template<typename V, typename T>
V operator*(V const & vi, SparseMatrix<T> const & m) {
  V vo(T(0),m.rank());
  SparseMatrix<double>::const_iterator p = m.values().begin();
  SparseMatrix<double>::const_iterator e = m.values().end();
  for (;p!=e;p++)
    vo[(*p).j] += (*p).v*vi[(*p).i];
  return vo;
}

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
