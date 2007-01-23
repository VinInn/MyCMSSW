#ifndef PerfTools_PerfTools_H
#define PerfTools_PerfTools_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include<boost/any.hpp>


/* top level Framework service
   to interface clients to any intrusive perftool... 
 */
class PerfTools {
public:

  typedef boost::any Handle;

  PerfTools(edm::ParameterSet const& pset, edm::ActivityRegistry  & activity);


  Handle get(edm::ParameterSet const& pset);




};

#endif
