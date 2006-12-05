#include "PoolTests/DataFormatTests/interface/TypeName.h"
#include "cxxabi.h"


namespace VUtils {

  std::string demangle(const std::type_info& t) {
    char buff[16*1024];
    size_t len = sizeof(buff);
    ::memset(buff,0,len);
    int    status = 0;
    const char* rr = __cxxabiv1::__cxa_demangle(t.name(), buff, &len, &status);
    return std::string(rr? rr : "");
  }

}
