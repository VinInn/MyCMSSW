#ifndef Common_PolyVector_h
#define Common_PolyVector_h

#include <boost/iterator_adaptors.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/bind.hpp>


#include <algorithm>
#include <functional>
#include <vector>
#include <map>
#include <typeinfo>

namespace edm {

  namespace details {

    
    template <typename B>
    struct PolyVectorInnerBase {
      virtual ~PolyVectorInnerBase(){}
      virtual bool empty() const=0;
      virtual size_t size() const=0;
      virtual void put(const B& b)=0;
      B& get(int i) { return get_impl(i); }
      const B& get(int i) const { return get_impl(i); }
    private:
      virtual B& get_impl(int i)=0;
      
    };
    
    template<typename T, typename B>
    struct PolyVectorBase : public PolyVectorInnerBase<B> {
      typedef  std::vector<T> base;
      typedef T value_type;
      std::vector<T> v;
      virtual bool empty() const { return v.empty();}
      virtual size_t size() const { return v.size();}
    private:
      void put(const B& b) { v.push_back((T&)(b));} 
      virtual T & get_impl(int i) { return v[i];}
    };
    
    
  }




  /* a polymorfic vector with by value storage (no cloning...)
     
     
   */
  template<class T>
  class PolyVector {
  private:
    typedef std::vector<details::PolyVectorInnerBase<T>*> base;
    typedef PolyVector<T> self;
    typedef std::pair<int, int> Index;
    typedef std::vector<Index> Indices;
    typedef Indices::reference index_ref; 
    typedef Indices::const_reference index_cref; 
    typedef Indices::iterator index_iter; 
    typedef Indices::const_iterator index_citer; 

    typedef std::map<std::type_info const *, int> Types;
    typedef Types::const_iterator Types_citer;
    typedef Types::iterator Types_iter;

    typedef typename
    boost::_bi::bind_t<const T&, boost::_mfi::cmf1<const T&, self, index_cref>, boost::_bi::list2<boost::_bi::value<self const *>, boost::arg<1> > > BindConstFunction;


  public:
    typedef typename base::size_type size_type;
    typedef T value_type;
    typedef T * pointer;
    typedef T & reference;
    typedef const T & const_reference;
    
    typedef boost::transform_iterator<BindConstFunction,index_citer> const_iterator;


  public:
    template<typename V>  Types_citer addType() {
      Types_citer p = m_types.find(&typeid(V));
      if (p!=m_types.end()) return p ;
      p = m_types.insert(std::make_pair(&typeid(V),m_data.size())).first;
      m_data.push_back(new details::PolyVectorBase<V,T>());
      return p;
    }

    template<typename V> void push_back(V const & v) {
      Types_citer p = m_types.find(&typeid(V));
      // shall we addType or throw?
      if (m_types.find(&typeid(V))==m_types.end()) 
	p = this->addType<V>();
      m_indices.push_back(Index((*p).second, m_data[(*p).second]->size()));
      m_data[(*p).second]->put(v);
    }

    const_iterator begin() const {
      return boost::make_transform_iterator(m_indices.begin(),boost::bind(&self::_cdata,this,_1));
    }

    const_iterator end() const  {
      return boost::make_transform_iterator(m_indices.end(),boost::bind(&self::_cdata,this,_1));
    }


    reference operator[](size_type i) {
      return data(i);
    }

    const_reference operator[](size_type i) const {
      return data(i);
    }

    reference _ncdata(index_cref j ) {
      return (*m_data[j.first]).get(j.second);
    }


    const_reference _cdata(index_cref j ) const {
      return (*m_data[j.first]).get(j.second);
    }


  private:

    reference data(size_type i) {
      index_cref j = m_indices[i];
      return (*m_data[j.first]).get(j.second);
    }


    const_reference data(size_type i) const {
      index_cref j = m_indices[i];
      return (*m_data[j.first]).get(j.second);
    }

  private:
    Types m_types;
    Indices m_indices;
    base m_data;
    
  };
  
}

#endif
