#include "PerfTools/Reporter/interface/Reporter.h"
#include "PerfTools/Reporter/interface/SimpleImmediateReporter.h"

#include <iostream>
#include <boost/bind.hpp>
#include <vector>
#include <algorithm>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
using namespace boost::assign;
#include <boost/iterator/zip_iterator.hpp>

template<typename T>
std::ostream& print(std::ostream& co, T is, char const * sep) {
  return co << is << sep;
}

void c(){}


template<typename T>
void printOne(boost::tuple<T const&, std::string const&> const & t,
	    perftools::SimpleImmediateReporter & sir) {
  sir(t.get<1>(),t.get<0>());
}

template<typename T>
void printV(std::string const & name, std::vector<T> const & v,
	    std::vector<std::string> const & tags,
	    perftools::SimpleImmediateReporter & sir) {
  sir.stream() << name << ": ";
  typedef boost::tuple<T const&, std::string const&> Tuple;
  std::for_each(boost::make_zip_iterator(boost::make_tuple(v.begin(), tags.begin())),
		boost::make_zip_iterator(boost::make_tuple(v.end(), tags.end())),
		boost::bind(printOne<T>, _1, boost::ref(sir))
		); 
  //boost::bind(&perftools::SimpleImmediateReporter::operator()<T>,
  //boost::ref(sir), 
  // boost::bind(Tuple::get<1>, _1), 
  //boost::bind(Tuple::get<0>, _1))); 
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

  // later....
  // r.subscribe<void(*)(void)>("here","c",0,c);

  int & a = *r.subscribe<int>("here","int 1");
  int & b = *r.subscribe<int>("here","int 2");

  std::cout << "a,b " << a << " " << b << std::endl;

  std::vector<int> & v2 =  *r.subscribe<std::vector<int> >("here","vec 1");
  v2.resize(2);
  
  v2[1]=3;

  r.add<int>("here",boost::bind(print<int>,boost::bind(print<std::string const&>,boost::ref(std::cout),_1,": "),_2,"\n"));

  std::vector<std::string> tags; tags += "x= ","y= ";
  perftools::SimpleImmediateReporter sirV(std::cout,v2.size());
  r.add<std::vector<int> >("here",boost::bind(printV<int>,_1,_2,tags,boost::ref(sirV)));
  

  r.setDefaultTitleReport(boost::bind(print<char>,boost::bind(print<std::string const&>,boost::ref(std::cout),_1,""),'\n',""));
  //   r.setDefaultTitleReport(boost::bind(std::cout << _1 << std::endl));

  a++;

  r.report();

  return 0;

}
