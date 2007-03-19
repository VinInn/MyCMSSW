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
class  OneBranchTreeBase {

public:
  explicit OneBranchTreeBase(pool::IFileCatalog * cat, std::string const & fname = "Events.root",
			 std::string const & bname = "Events(Prod)" );
  ~OneBranchTreeBase();

  void start();
  void commit();

protected:

  int m_nentry;
  
  std::auto_ptr<pool::IDataSvc> m_svc;
  
  pool::Placement m_place;  


};

class  OneBranchTree : private  OneBranchTreeBase {

public:
  explicit OneBranchTree(pool::IFileCatalog * cat, std::string const & fname = "Events.root",
			 std::string const & bname = "Events(Prod)" );
  ~OneBranchTree(){}

  void add(edm::EDProduct * prod); 

private:
  pool::Ref<edm::EDProduct> m_product;

};

template<typename Product>
class  OneBranchTreeNaive : private  OneBranchTreeBase {

public:
  explicit OneBranchTree(pool::IFileCatalog * cat, std::string const & fname = "Events.root",
			 std::string const & bname = "Events(Prod)" ) : 
    OneBranchTreeBase(cat,fname,bname),
    m_product(*m_svc) {}

  ~OneBranchTree(){}

  void add(Product * prod) {
    start();
    m_product = prod;
    m_product.markWrite(m_place);
    commit();
  }

private:
  pool::Ref<Product> m_product;

};


#endif
