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

    struct Catalog {
      Catalog(boost::shared_ptr<pool::IFileCatalog> icat);
      ~Catalog();

      boost::shared_ptr<pool::IFileCatalog> cat;
    };

    BaseTree( std::string const & fname,
	      std::string const & tname,
	      boost::shared_ptr<pool::IFileCatalog> cat=
	      boost::shared_ptr<pool::IFileCatalog>()
	      );

    virtual ~BaseTree();
     

    bool add (std::string const & name, boost::shared_ptr<BaseBranch> branch);

    boost::shared_ptr<BaseBranch> find(std::string const & name) const;

    void write();

    Data const & data() const { return m_data;}

  private:
    Catalog m_cat;
    Data m_data;

    typedef std::map<std::string, boost::shared_ptr<BaseBranch> > Branches;
    Branches branches;
    int m_globalCount;

  };


}


#endif // Synthetic_BaseTree_H
