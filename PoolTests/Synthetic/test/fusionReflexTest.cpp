#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>


#include "Reflex/Builder/ReflexBuilder.h"
#include <typeinfo>
#include<vector>
#include<boost/shared_ptr.hpp>
#include<sstream>

using namespace ROOT::Reflex;
namespace fusionReflex {

  Type type_void = TypeBuilder("void");
  Type type_float = TypeBuilder("float");
  Type type_int = TypeBuilder("int");
//  Type type_basic_string = TypeBuilder("std::basic_string<char>");
//  Type type_string = TypedefTypeBuilder("std::string", type_basic_string);


  //------Stub functions for class  -------------------------------
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



 template <typename T>
 void dict(char const * cname) {
   ROOT::Reflex::Type type_T = ROOT::Reflex::Type::ByTypeInfo(typeid(T));
   static const std::string tilde("~");
   std::string dname = tilde+cname;
   ROOT::Reflex::ClassBuilder c(cname, typeid(T), sizeof(T), PUBLIC);
   // c.AddProperty("ClassID", "5F16F0A9-79D1-4881-CE2B-C271DD84A7F2")

   T t;
   boost::fusion::for_each(t, addMembers<T>(t,c));

   //      .AddDataMember(type_int, "id", 0, PUBLIC)
   //   .AddDataMember(type_string, "name",  4, PUBLIC)

   c.AddFunctionMember(FunctionTypeBuilder(type_void, type_T), cname, Stubs<T>::copy_constructor, 0, "_ctor_arg", PUBLIC | ARTIFICIAL | CONSTRUCTOR)
     .AddFunctionMember(FunctionTypeBuilder(type_void), cname, Stubs<T>::constructor, 0, 0, PUBLIC | ARTIFICIAL | CONSTRUCTOR)
     .AddFunctionMember(FunctionTypeBuilder(type_void), dname.c_str(), Stubs<T>::destructor, 0, 0, PUBLIC | ARTIFICIAL | DESTRUCTOR )
     . template AddFunctionMember<void*(void)>("__getNewDelFunctions", Stubs<T>::newdel, 0, 0, PUBLIC | ARTIFICIAL);
 }


}


int main() {

  typedef boost::fusion::vector<int, char, double, std::string, std::vector<int> > seq_type;
  
  fusionReflex::dict<seq_type>("MyClass");
  
  
  seq_type t(1, 'x', 3.3, "hello", std::vector<int>(4,0));
  
  ROOT::Reflex::Type t1 = ROOT::Reflex::Type::ByName("MyClass");

  if (t1) {
    if (t1.IsClass()) std::cout << "Class ";
    std::cout << t1.Name();
  }
  else std::cout << "not there ";
  std::cout << std::endl;

  for (ROOT::Reflex::Member_Iterator mi = t1.DataMember_Begin(); mi != t1.DataMember_End(); ++mi) {
    std::cout << (*mi).Name(ROOT::Reflex::SCOPED) << " " 
	      << (*mi).TypeOf().Name(ROOT::Reflex::QUALIFIED) << std::endl;
  }

  ROOT::Reflex::Object o1(t1,&t);

  {
    ROOT::Reflex::Object mem_obj = o1.Get("m_0");
    int real_value = 0;
    if (mem_obj.TypeOf().Name() == "int")
      real_value = ROOT::Reflex::Object_Cast<int>(mem_obj);
    std::cout << mem_obj.TypeOf().Name() <<  " " << real_value << std::endl;
  }
  {
    ROOT::Reflex::Object mem_obj = o1.Get("m_2");
    double real_value = 0.;
    if (mem_obj.TypeOf().Name() == "double")
      real_value = ROOT::Reflex::Object_Cast<double>(mem_obj);
    std::cout  << mem_obj.TypeOf().Name() <<  " " << real_value << std::endl;
  }


  return 0;

}
