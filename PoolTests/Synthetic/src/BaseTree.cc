#include "PoolTests/Synthetic/interface/BaseTree.h"
#include "PoolTests/Synthetic/interface/BaseBranch.h"

#include "DataSvc/IDataSvc.h"
#include "DataSvc/ICacheSvc.h"
#include "PersistencySvc/DatabaseConnectionPolicy.h"
#include "PersistencySvc/ISession.h"
#include "PersistencySvc/ITransaction.h"
#include "PersistencySvc/IDatabase.h"
#include "DataSvc/DataSvcFactory.h"

#include "FileCatalog/URIParser.h"
#include "FileCatalog/FCSystemTools.h"
#include "FileCatalog/IFileCatalog.h"

#include "PluginManager/PluginManager.h"


#include <boost/bind.hpp>
#include <algorithm>


namespace synthetic {

  BaseTree::Catalog::Catalog(boost::shared_ptr<pool::IFileCatalog> icat) :
    cat (icat ? icat : boost::shared_ptr<pool::IFileCatalog>(new pool::IFileCatalog))
  {
    seal::PluginManager::get()->initialise();
    
    
    pool::URIParser p;
    p.parse();
    cat->setWriteCatalog(p.contactstring());
    
    cat->connect();
    cat->start();
  }
  
  Catalog::~Catalog() {
    cat->commit();
  }
  
  
  BaseTree::BaseTree( boost::shared_ptr<pool::IFileCatalog> cat,
		      std::string const & fname,
		      std::string const & tname) :
    m_cat(cat),
    m_data(fname,tname,pool::DataSvcFactory::instance(&*m_cat.cat)),
    m_globalCount(0) {
    pool::DatabaseConnectionPolicy policy;
    policy.setWriteModeForNonExisting(pool::DatabaseConnectionPolicy::CREATE);
    // policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::OVERWRITE);
    policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::UPDATE);
    m_data.svc->session().setDefaultConnectionPolicy(policy);   
  }

  BaseTree::~BaseTree() {
    m_data.svc->transaction().commit();
    m_data.svc->session().disconnectAll();
    delete m_data.svc;
  }
     
  bool BaseTree::add(std::string const & name, BaseBranch * branch) {
    return branches.insert(make_pair(name,branch)).second;
  }
  
  boost::shared_ptr<BaseBranch> BaseTree::find(std::string const & name) const  {
    Branches::const_iterator p = branches.find(name);
    return (p!=branches.end()) ? (*p).second : boost::shared_ptr<BaseBranch>();
  }
  
  void BaseTree::write() {
    m_globalCount++;
    m_data.svc->transaction().start(pool::ITransaction::UPDATE);
    std::for_each(branches.begin(),branches.end(),
		  boost::bind(&BaseBranch::write,
			      boost::bind(&Branches::value_type::second,_1), 
			      m_globalCount));
    m_data.svc->transaction().commitAndHold();
    
    if (m_globalCount%10==0)
      m_data.svc->transaction().commit();
    
  }


}
