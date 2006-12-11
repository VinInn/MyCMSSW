#include "PoolTests/PolyPerformance/interface/PolyPerformanceStubs.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "VinInn/Test1/test/PolyVector.h"
#include "DataFormats/Common/interface/Wrapper.h"

namespace {
  namespace {
    polyPerformance::Array<int,3> dummy1;
    polyPerformance::Array<int,6> dummy2;
    polyPerformance::Array<float,3> dummy11;
    polyPerformance::Array<float,6> dummy12;
    polyPerformance::Array<double,3> dummy21;
    polyPerformance::Array<double,6> dummy22;
    
    edm::details::PolyVectorBase< polyPerformance::Array<int,3>, polyPerformance::Base > vdummy1;
    edm::details::PolyVectorBase<polyPerformance::Array<int,6> , polyPerformance::Base > vdummy2;
    edm::details::PolyVectorBase<polyPerformance::Array<float,3> , polyPerformance::Base > vdummy11;
    edm::details::PolyVectorBase<polyPerformance::Array<float,6> , polyPerformance::Base > vdummy12;
    edm::details::PolyVectorBase< polyPerformance::Array<double,3> , polyPerformance::Base > vdummy21;
    edm::details::PolyVectorBase<polyPerformance::Array<double,6> , polyPerformance::Base > vdummy22;
    
    edm::PolyVector<polyPerformance::Base> pvd;
    edm::Wrapper<edm::PolyVector<polyPerformance::Base> >pvdw;
  }
}

// "polyPerformance::Array<int,3>" "polyPerformance::Array<int,6>" "polyPerformance::Array<float,3>" "polyPerformance::Array<float,6>" "polyPerformance::Array<double,3>" "polyPerformance::Array<double,6>" 
