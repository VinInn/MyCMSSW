#include "PoolTests/NavigationTests/interface/MyABC.h"
#include "PoolTests/NavigationTests/interface/ABC.h"
#include "PoolTests/NavigationTests/interface/Traits.h"
#include "PoolTests/NavigationTests/interface/MyMap.h"
#include "PoolTests/NavigationTests/interface/Fields.h"

/*
namespace {
  std::vector<double> dummy;
  NavigationTests::D<double> dummy2;
  pool::Ref<NavigationTests::C> dummy3;
  pool::Ref<NavigationTests::B> dummy4;
  std::vector<pool::Ref<NavigationTests::B> > dummy41;
}
*/

namespace {
  NavigationTests::D<double> dummy1;
  NavigationTests::D<bool> dummy12;
  NavigationTests::D<NavigationTests::H> dummy2;
  NavigationTests::DP<NavigationTests::H> dummy3;
  MD<double> dummy52;
  R<MCC> dummy53;
  R<MB> dummy54;
  std::vector<R<MB> > dummy541;
  std::string dummy542;
  TT<T1> t1;
  TT<T2> t2; 
  pool::Ref<const NavigationTests::AT> dumca;
  pool::Ref<const NavigationTests::T1> dumc1;
  pool::Ref<const NavigationTests::T2> dumc2;
  std::vector<pool::Ref<const NavigationTests::T1> > dumv1;
  std::vector<pool::Ref<const NavigationTests::T2> > dumv2;  
  std::pair<const int,std::string> dummym1;
  std::pair<int,std::string> dummym2;

  pool::Ref<NavigationTests::Base> rd1;
  pool::Ref<NavigationTests::Compound> rd2;

}
