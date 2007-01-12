#ifndef Synthetic_Branch_H
#define Synthetic_Branch_H


#include "PoolTests/Synthetic/interface/BaseBranch.h"
#include "PoolTests/Synthetic/interface/Synthesis.h"

#include "Reflex/Type.h"
#include "DataSvc/Ref.h"
#include "PersistencySvc/Placement.h"
#include <string>

namespace synthetic {

  std::string branchName(std::string const & tname, 
			 std::string const & cname) {
    return tname+"("+cname+")";
  }
 
  template <typename Vec>
  class Branch : public BaseBranch {
  public:
    
    typedef Vec vec_type;
    
    template <typename IT>
    Branch(pool::IDataSvc *svc, 
	   std::string const & fname,
	   std::string const & tname, 
	   std::string const & cname,  
	   IT ib, IT ie) : 
      dict(cname.c_str(),ib,ie), 
      place(fname, pool::DatabaseSpecification::PFN,
	    branchName(tname,cname), 
	    ROOT::Reflex::Type(), 
	    pool::ROOTTREE_StorageType.type()), 
      obj(svc), count(0), added(false) {}


   bool add(std::auto_ptr<vec_type> v) {
      if (added) return false; // throw?
      obj = v.release();
      added = obj;
      return added;
    }

  private:    
    synthetic::Dict<vec_type> dict;
    pool::Placement place;  
    pool::Ref<vec_type> obj;
    int count;
    bool added;
    
 
  private:
    void write(int globalCount) {
      if (globalCount<count) ; // throw
      // fill previous rows with defaults...
      for (;count!=globalCount; count++) {
	pool::Ref<vec_type> dummy = new vec_type;
	dummy.markWrite(place);
      }
      obj.markWrite(place);
    }
  };
  
}

#endif Synthetic_Branch_H







