#ifndef OneBranchTree_H
#define OneBranchTree_H

#include "DataFormats/Common/interface/EDProduct.h"
#include "PersistencySvc/Placement.h"
#include "DataSvc/Ref.h"
#include <memory>

namespace pool {
  class IFileCatalog;
  class IDataSvc;
}

/** a simple tree to test each edm prodcut independently
 *
 */
class  OneBranchTree {

public:
  explicit OneBranchTree(pool::IFileCatalog * cat, std::string const & fname = "Events.root",
			 std::string const & bname = "Ev(Prod)" );
  ~OneBranchTree();

  void add(edm::EDProduct * prod); 

private:

  int m_nentry;
  
  std::auto_ptr<pool::IDataSvc> m_svc;
  
  pool::Placement m_place;  

  pool::Ref<edm::EDProduct> m_product;

};

#endif
