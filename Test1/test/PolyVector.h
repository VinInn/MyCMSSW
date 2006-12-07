#ifndef Common_PolyVector_h
#define Common_PolyVector_h

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

    typedef std::pair<int, int> Index;
    typedef std::vector<Index> Indices;
    typedef Indices::reference index_ref; 
    typedef Indices::const_reference index_cref; 

    typedef std::map<std::type_info const *, int> Types;
    typedef Types::const_iterator Types_citer;
    typedef Types::iterator Types_iter;

  public:
    typedef typename base::size_type size_type;
    typedef T value_type;
    typedef T * pointer;
    typedef T & reference;
    typedef const T & const_reference;
    
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

    reference operator[](size_type i) {
      return data(i);
    }

    const_reference operator[](size_type i) const {
      return data(i);
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
