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
      virtual PolyVectorInnerBase<B> * clone() const=0; 

      virtual bool empty() const=0;
      virtual size_t size() const=0;
      virtual void put(const B& b)=0;
      B& get(int i) { return get_impl(i); }
      const B& get(int i) const { return get_impl(i); }
      const B& front() const { return get_impl(0); }
 
    private:
      virtual B& get_impl(int i) const =0;
      
    };
    
    template<typename T, typename B>
    struct PolyVectorBase : public PolyVectorInnerBase<B> {
      typedef  std::vector<T> base;
      typedef T value_type;
      std::vector<T> v;
      
      virtual PolyVectorBase<T,B> * clone() const {
	return new  PolyVectorBase<T,B>(*this);
      }

      virtual bool empty() const { return v.empty();}
      virtual size_t size() const { return v.size();}
    private:
      void put(const B& b) { v.push_back((T&)(b));} 
      virtual T & get_impl(int i) const { return const_cast<T&>(v[i]);}
    };
    
    template<typename P>
    void destroy ( P & p ) { delete p; p=0;}

    template<typename P>
    P cloner ( P p ) { return p->clone();}


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

  public:
    typedef typename
    boost::_bi::bind_t<T&, boost::_mfi::mf1<T&, self, index_cref>, boost::_bi::list2<boost::_bi::value<self *>, boost::arg<1> > > BindFunction;
    typedef typename
    boost::_bi::bind_t<const T&, boost::_mfi::cmf1<const T&, self, index_cref>, boost::_bi::list2<boost::_bi::value<self const *>, boost::arg<1> > > BindConstFunction;


  public:
    typedef typename base::size_type size_type;
    typedef T value_type;
    typedef T * pointer;
    typedef T & reference;
    typedef const T & const_reference;
    
    typedef boost::transform_iterator<BindFunction,index_citer> iterator;
    typedef boost::transform_iterator<BindFunction,index_citer, const_reference> const_iterator;



  public:

    PolyVector(){}

    ~PolyVector()
    {
      destroy();
    }

    PolyVector(const self& other) :
      m_types(other.m_types),
      m_indices(other.m_indices) {
      m_data.reserve(other.m_data.size());
      for (typename base::const_iterator p=other.m_data.begin(); p!=other.m_data.end();p++)
	m_data.push_back((*p)->clone());
    }

    size_type size() const { return m_indices.size(); }
    bool empty() const { return m_indices.empty(); }

    void clear() {
      destroy();
      m_data.clear();
      m_indices.clear();
      m_types.clear();
    }

    void swap(self & other) {
      m_data.swap(other.m_data);
      m_indices.swap(other.m_indices);
      m_types.swap(other.m_types);
    }


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
      if (p==m_types.end()) 
	p = this->addType<V>();
      m_indices.push_back(Index((*p).second, m_data[(*p).second]->size()));
      m_data[(*p).second]->put(v);
    }

    void push_back(T const & t) {
      Types_citer p = m_types.find(&typeid(t));
      // no factories yet... so we throw
      if (p==m_types.end()) 
	throw std::string(std::string("type not added yet: ") +std::string(typeid(t).name()));
      m_indices.push_back(Index((*p).second, m_data[(*p).second]->size()));
      m_data[(*p).second]->put(t);
    }




    iterator begin() {
      return boost::make_transform_iterator(m_indices.begin(),boost::bind(&self::_ncdata,this,_1));
    }

    iterator end() {
      return boost::make_transform_iterator(m_indices.end(),boost::bind(&self::_ncdata,this,_1));
    }

    const_iterator begin() const {
      return boost::make_transform_iterator(m_indices.begin(),boost::bind(&self::_ncdata,const_cast<self*>(this),_1));
    }

    const_iterator end() const  {
      return boost::make_transform_iterator(m_indices.end(),boost::bind(&self::_ncdata,const_cast<self*>(this),_1));
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


    void buildTypes() {
      m_types.clear();
      // first remove empty containers...
      for (typename base::iterator p=m_data.begin(); p!=m_data.end();p++)
	if ( (*p)->empty() ) { delete (*p); (*p)=0;}
      typename base::iterator new_end = 
	std::remove_if(m_data.begin(), m_data.end(),std::bind2nd(std::equal_to<typename base::value_type>(),0));
      m_data.erase(new_end,m_data.end());
      // assume no empty, no duplicate
      for (typename base::iterator p=m_data.begin(); p!=m_data.end();p++)
	m_types.insert(std::make_pair(&typeid((*p)->front()), p-m_data.begin()));
    }

    bool verifyTypes() const {
      for (typename base::const_iterator p=m_data.begin(); p!=m_data.end();p++) 
	if ( (*p)->empty() || m_types.find(&typeid((*p)->front()))==m_types.end()) return false;
      for (Types_citer p = m_types.begin(); p!=m_types.end();p++)
	if ( m_data[(*p).second]->empty() || (&typeid( m_data[(*p).second]->front())!=(*p).first) )return false;
      return true;
    }


  private:

    void destroy() {
      std::for_each(m_data.begin(),m_data.end(), details::destroy<typename base::value_type>);
    }

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
