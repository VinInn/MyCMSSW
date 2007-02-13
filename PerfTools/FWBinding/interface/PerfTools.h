#ifndef PerfTools_PerfTools_H
#define PerfTools_PerfTools_H

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"


#include<boost/any.hpp>


/* top level Framework service
   to interface clients to any intrusive perftool... 
 */
class PerfTools {
public:

  typedef std::vector<boost::any> Payload;

  PerfTools(edm::ParameterSet const& pset, edm::ActivityRegistry  & activity);


  Payload & get(edm::ParameterSet const& pset);

private :

  Payload * m_payload;


};

#endif
