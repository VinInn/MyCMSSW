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
      std::string m_fname;
      std::string m_tname;
      pool::IDataSvc * m_svc;

    };

    BaseTree( pool::IFileCatalog * cat,
	      std::string const & fname,
	      std::string const & tname);

    virtual ~BaseTree();
     
    void add (BaseBranch * branch);

    boost::shared_ptr<BaseBranch> find(std::string const & name);

    void write();

    Data const & data() const { return data;}

  private:

    Data m_data;

    typedef std::map<std::string, boost::shared_ptr<BaseBranch> > Branches;
    Branches branches;

  };


}


#endif // Synthetic_BaseTree_H
