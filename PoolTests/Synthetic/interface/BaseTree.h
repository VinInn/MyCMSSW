#ifndef Synthetic_BaseTree_H
#define Synthetic_BaseTree_H

#include "PoolTests/Synthetic/interface/BaseBranch.h"
#include <map>
#include <boost/shared_ptr.hpp>
#include <string>

namespace pool {
  class IFileCatalog;
  class IDataSvc;
}

namespace synthetic {

  class  BaseTree {
  public:
    struct Data {
      Data(std::string const & ifname,
	   std::string const & itname, 
	   pool::IDataSvc * isvc) : 
	fname(ifname),
	tname(itname),
	svc(isvc){}
      std::string fname;
      std::string tname;
      pool::IDataSvc * svc;

    };

    BaseTree( pool::IFileCatalog * cat,
	      std::string const & fname,
	      std::string const & tname);

    virtual ~BaseTree();
     
    bool add (std::string const & name, BaseBranch * branch);

    boost::shared_ptr<BaseBranch> find(std::string const & name);

    void write();

    Data const & data() const { return m_data;}

  private:

    Data m_data;

    typedef std::map<std::string, boost::shared_ptr<BaseBranch> > Branches;
    Branches branches;
    int m_globalCount;

  };


}


#endif // Synthetic_BaseTree_H
