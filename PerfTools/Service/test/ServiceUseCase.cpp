/* simple service use case and tech. investigation...
 *
 */


#include<map>
#include<string>
#include<boost/shared_ptr.hpp>
#include<boost/any.hpp>

namespace perftools {

  typedef boost::shared_ptr<boost::any> ServiceHandle;
  ServiceHandle service(std::string & const name) {
    typedef std::map<std::string, ServiceHandle> Repository;
    static Repository local;
    ServiceHandle & h = local[name];
    if (h.empty()) h = loadService(name);
    if (h.empty()) reportErrorNoService(name);
    return h;
  }


}
