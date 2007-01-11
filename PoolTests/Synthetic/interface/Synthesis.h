#ifndef Synthetic_Synthesis_H
#define Synthetic_Synthesis_H

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>

#include "Reflex/Builder/ReflexBuilder.h"
#include <typeinfo>
#include <sstream>

namespace synthetic {

  //------Stub functions for fusion seq  -------------------------------
  template <typename T>
  struct Stubs {
    
    static void* copy_constructor( void* mem, const std::vector<void*>& arg, void*) {
      return ::new(mem) T(*(const T*)arg[0]);
    }
    
    static void* constructor( void* mem, const std::vector<void*>&, void*) {
      return ::new(mem) T();
    }
    
    static void* destructor(void * o, const std::vector<void*>&, void *) {
      ((T*)o)->~T(); return 0;
    }
    
    
    static void* newdel( void*, const std::vector<void*>&, void*) {
      static ROOT::Reflex::NewDelFunctions s_funcs;
      s_funcs.fNew         =  ROOT::Reflex::NewDelFunctionsT< T >::new_T;
      s_funcs.fNewArray    =  ROOT::Reflex::NewDelFunctionsT< T >::newArray_T;
      s_funcs.fDelete      =  ROOT::Reflex::NewDelFunctionsT< T >::delete_T;
      s_funcs.fDeleteArray =  ROOT::Reflex::NewDelFunctionsT< T >::deleteArray_T;
      s_funcs.fDestructor  =  ROOT::Reflex::NewDelFunctionsT< T >::destruct_T;
      return &s_funcs;
    }
  };


  // add a member to a fusion sequence 
  template <typename Seq, typename IT>
  struct addMembers {
    addMembers(Seq const & iseq, 
	       ROOT::Reflex::ClassBuilder & c, IT ib, IT ie) : 
      seq(iseq), cb(c), count(0), b(ib), e(ie) {}

    template<typename T>
    void operator()(T const & t) const {
      ROOT::Reflex::Type type_T = ROOT::Reflex::Type::ByTypeInfo(typeid(T));
      static std::string const m("m_");
      std::ostringstream s;
      if (b!=e) s << *(b++);
      else s << m << count;
      int off = (char*)(&t) - (char*)(&seq);
      cb.AddDataMember(type_T,s.str().c_str(), off, ROOT::Reflex::PUBLIC);
      count++;
    }
    
    Seq const & seq;
    mutable ROOT::Reflex::ClassBuilder & cb;
    mutable int count;
    mutable IT b, e;
  };



  // build a dictionary for a fusion sequence T (actually even any POD...)
  template <typename T, typename IT>
  ROOT::Reflex::Type buildType(char const * cname, IT b, IT e) {
    ROOT::Reflex::NamespaceBuilder nsb("synthetic");
    ROOT::Reflex::Type type_void = ROOT::Reflex::TypeBuilder("void");

    ROOT::Reflex::Type type_T = ROOT::Reflex::Type::ByTypeInfo(typeid(T));
    static const std::string tilde("~");
    static const std::string ns("::synthetic::");
    std::string qname = ns+cname;
    std::string dname = tilde+cname;
    ROOT::Reflex::ClassBuilder c(qname.c_str(), typeid(T), sizeof(T), ROOT::Reflex::PUBLIC);
    // c.AddProperty("ClassID", "5F16F0A9-79D1-4881-CE2B-C271DD84A7F2")
    
    T t;
    boost::fusion::for_each(t, addMembers<T,IT>(t,c,b,e));
    
    c.AddFunctionMember(ROOT::Reflex::FunctionTypeBuilder(type_void, type_T), cname, 
			Stubs<T>::copy_constructor, 0, "_ctor_arg", 
			ROOT::Reflex::PUBLIC | ROOT::Reflex::ARTIFICIAL | ROOT::Reflex::CONSTRUCTOR)
      .AddFunctionMember(ROOT::Reflex::FunctionTypeBuilder(type_void), cname, Stubs<T>::constructor, 0, 0, 
			 ROOT::Reflex::PUBLIC | ROOT::Reflex::ARTIFICIAL | ROOT::Reflex::CONSTRUCTOR)
      .AddFunctionMember(ROOT::Reflex::FunctionTypeBuilder(type_void), dname.c_str(), Stubs<T>::destructor, 0, 0, 
			 ROOT::Reflex::PUBLIC | ROOT::Reflex::ARTIFICIAL | ROOT::Reflex::DESTRUCTOR )
      . template AddFunctionMember<void*(void)>("__getNewDelFunctions", Stubs<T>::newdel, 0, 0, 
						ROOT::Reflex::PUBLIC | ROOT::Reflex::ARTIFICIAL);

    return type_T;
  }

  template <typename T>
  class Dict {
  public:
    Dict(char const * cname) :
      type(buildType<T, char*>(cname,(char*)(0),(char*)(0))) {}
    template<typename IT>
    Dict(char const * cname, IT b, IT e) : 
      type(buildType<T,IT>(cname,b,e)){}
    ~Dict() {
      type.Unload();
    }
    
  private:
    ROOT::Reflex::Type type;
  };

}

#endif // Synthetic_Synthesis_H




