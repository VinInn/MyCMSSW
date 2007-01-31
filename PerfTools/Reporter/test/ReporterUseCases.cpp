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

template<typename T>
void printV(std::string const & name , std::vector<T> const & v, perftools::SimpleImmediateReporter & sir) {
  sir.stream() << name << ": ";
  std::for_each(v.begin(),v.end(),
		boost::bind(&perftools::SimpleImmediateReporter::operator()<T>,
			    boost::ref(sir),"y= ",_1)); 
}


int main() {

  perftools::Reporter r;
  perftools::SimpleImmediateReporter sir(std::cout,3);

  std::vector<int> v; v +=1,2,3;

  std::for_each(v.begin(),v.end(),
		boost::bind(&perftools::SimpleImmediateReporter::operator()<int>,
			    boost::ref(sir),"x= ",_1));
  std::for_each(v.begin(),v.end(),
		boost::bind(&perftools::SimpleImmediateReporter::operator()<int>,
			    boost::ref(sir),"y= ",_1));


  r.subscribe<void(*)(void)>("here","c",c);

  int & a = *r.subscribe<int>("here","int 1");
  int & b = *r.subscribe<int>("here","int 2");

  std::vector<int> & v =  *r.subscribe<std::vector<int> >("here","vec 1");
  v.resize[2];
  
  v[1]=3;

  r.add<int>("here",boost::bind(print<int>,boost::bind(print<std::string const&>,boost::ref(std::cout),_1,": "),_2,"\n"));

  perftools::SimpleImmediateReporter sirV(std::cout,v.size());
  r.add<std::vector<int> >("here",boost::bind(printV<int>,_1,_2,boost::cref(sirV)));
  

  r.setDefaultTitleReport(boost::bind(print<char>,boost::bind(print<std::string const&>,boost::ref(std::cout),_1,""),'\n',""));
  //   r.setDefaultTitleReport(boost::bind(std::cout << _1 << std::endl));

  a++;

  r.report();

  return 0;

}
