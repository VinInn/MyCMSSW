#include "Reflex/Type.h"
#include "FileCatalog/IFileCatalog.h"
#include "StorageSvc/DbType.h"
#include "PersistencySvc/DatabaseConnectionPolicy.h"
#include "PersistencySvc/ISession.h"
#include "PersistencySvc/ITransaction.h"
#include "PersistencySvc/IDatabase.h"
#include "PersistencySvc/Placement.h"
#include "DataSvc/DataSvcFactory.h"
#include "DataSvc/IDataSvc.h"
#include "DataSvc/ICacheSvc.h"
#include "DataSvc/Ref.h"

#include "PoolTests/DataFormatTests/interface/OneBranchTree.h"


OneBranchTree::OneBranchTree(pool::IFileCatalog * cat, 
			     std::string const & fname /* = "Events.root" */,
			     std::string const & bname /* = "Ev(Prod)" */ ) :
  m_nentry(0),    
  m_svc(pool::DataSvcFactory::instance(cat)),
  m_place(fname, pool::DatabaseSpecification::PFN, bname, ROOT::Reflex::Type(), 
	   pool::ROOTTREE_StorageType.type()),
  m_product(*m_svc)
{
   // Define the policy for the implicit file handling
    pool::DatabaseConnectionPolicy policy;
    policy.setWriteModeForNonExisting(pool::DatabaseConnectionPolicy::CREATE);
    // policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::OVERWRITE);
    policy.setWriteModeForExisting(pool::DatabaseConnectionPolicy::UPDATE);
    m_svc->session().setDefaultConnectionPolicy(policy);
}



OneBranchTree::~OneBranchTree()
{
  m_svc->transaction().commit();
  m_svc->session().disconnectAll();
  //  std::cout << "cache size at end " << svc->cacheSvc().cacheSize() << std::endl;
}


void OneBranchTree::add(edm::EDProduct * prod) {
  m_svc->transaction().start(pool::ITransaction::UPDATE);
  m_product = prod;
  m_product.markWrite(m_place);
  m_nentry++;
  m_svc->transaction().commitAndHold();
    
    if (m_nentry%10==0) {
      // std::cout << "cache size before commit " << svc->cacheSvc().cacheSize() << std::endl;
      m_svc->transaction().commit();
      // std::cout << "cache size after commit " << svc->cacheSvc().cacheSize() << std::endl;
    }
}
