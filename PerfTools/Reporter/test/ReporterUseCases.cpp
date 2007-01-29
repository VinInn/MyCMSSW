#include "PerfTools/Reporter/interface/Reporter.h"
#include "PerfTools/Reporter/interface/SimpleImmediateReporter.h"

#include <iostream>
#include <boost/bind.hpp>
#include <vector>
#include <algorithm>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
using namespace boost::assign;


template<typename T>
std::ostream& print(std::ostream& co, T is, char const * sep) {
  return co << is << sep;
}

void c(){}



int main() {

  perftools::Reporter r;
  perftools::SimpleImmediateReporter sir(std::cout,3);

  std::vector<int> v; v +=1,2,3;

  std::for_each(v.begin(),v.end(),
		boost::bind(&SimpleImmediateReporter::operator()<int>,
			    bost::ref(sir),"x= ",_1));


  r.inscribe<void(*)(void)>("here","c",c);

  int & a = *r.inscribe<int>("here","int 1");
  int & b = *r.inscribe<int>("here","int 2");

  r.add<int>("here",boost::bind(print<int>,boost::bind(print<std::string const&>,boost::ref(std::cout),_1,": "),_2,"\n"));

  r.setDefaultTitleReport(boost::bind(print<char>,boost::bind(print<std::string const&>,boost::ref(std::cout),_1,""),'\n',""));
  //   r.setDefaultTitleReport(boost::bind(std::cout << _1 << std::endl));

  a++;

  r.report();

  return 0;

}
