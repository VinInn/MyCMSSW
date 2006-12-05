#ifndef TypeName_H
#define TypeName_h

#include <typeinfo>
#include <string>

namespace VUtils {

  std::string demangle(const std::type_info& t);

  template<typename T>
  struct TypeName {
    static std::string & name() {
      static std::string local(demangle(typeid(T)));
      return local;
    }
  };

  template<typename T> 
  std::string typeName(T const & t) {
    return demangle(typeid(t));
  }
  
}






#endif
