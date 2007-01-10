#ifndef Synthetic_Synthesis_H
#define Synthetic_Synthesis_H

#include "Reflex/Builder/ReflexBuilder.h"
#include <typeinfo>


namespace synthetic {

  ROOT::Reflex::NamespaceBuilder nsb("synthetic");
  ROOT::Reflex::Type type_void = TypeBuilder("void");

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
      static NewDelFunctions s_funcs;
      s_funcs.fNew         = NewDelFunctionsT< T >::new_T;
      s_funcs.fNewArray    = NewDelFunctionsT< T >::newArray_T;
      s_funcs.fDelete      = NewDelFunctionsT< T >::delete_T;
      s_funcs.fDeleteArray = NewDelFunctionsT< T >::deleteArray_T;
      s_funcs.fDestructor  = NewDelFunctionsT< T >::destruct_T;
      return &s_funcs;
    }
  };


  // add a member to a fusion sequence 
  template <typename Seq>
  struct addMembers {
    addMembers(Seq const & iseq, ROOT::Reflex::ClassBuilder & c) : 
      seq(iseq), cb(c), count(0){}

    template<typename T>
    void operator()(T const & t) const {
      ROOT::Reflex::Type type_T = ROOT::Reflex::Type::ByTypeInfo(typeid(T));
      static std::string const m("m_");
      std::ostringstream s; s << m << count;
      int off = (char*)(&t) - (char*)(&seq);
      cb.AddDataMember(type_T,s.str().c_str(), off, ROOT::Reflex::PUBLIC);
      count++;
    }
    
    Seq const & seq;
    mutable ROOT::Reflex::ClassBuilder & cb;
    mutable int count;
  };



  // build a dictionary for a fusion sequence T (actually even any POD...)
  template <typename T>
  void buildDict(char const * cname) {
    ROOT::Reflex::Type type_T = ROOT::Reflex::Type::ByTypeInfo(typeid(T));
    static const std::string tilde("~");
    static const std::string ns("::synthetic::");
    std::string qname = ns+cname;
    std::string dname = tilde+cname;
    ROOT::Reflex::ClassBuilder c(qname.c_str(), typeid(T), sizeof(T), PUBLIC);
    // c.AddProperty("ClassID", "5F16F0A9-79D1-4881-CE2B-C271DD84A7F2")
    
    T t;
    boost::fusion::for_each(t, addMembers<T>(t,c));
    
    c.AddFunctionMember(FunctionTypeBuilder(type_void, type_T), cname, Stubs<T>::copy_constructor, 0, "_ctor_arg", PUBLIC | ARTIFICIAL | CONSTRUCTOR)
      .AddFunctionMember(FunctionTypeBuilder(type_void), cname, Stubs<T>::constructor, 0, 0, PUBLIC | ARTIFICIAL | CONSTRUCTOR)
      .AddFunctionMember(FunctionTypeBuilder(type_void), dname.c_str(), Stubs<T>::destructor, 0, 0, PUBLIC | ARTIFICIAL | DESTRUCTOR )
      . template AddFunctionMember<void*(void)>("__getNewDelFunctions", Stubs<T>::newdel, 0, 0, PUBLIC | ARTIFICIAL);
  }
  
}

#endif // Synthetic_Synthesis_H




