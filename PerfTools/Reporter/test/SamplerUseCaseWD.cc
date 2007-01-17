#include "SamplerUseCaseWD.h"
#include <valarray>

#include <boost/fusion/sequence.hpp>
// #include <boost/fusion/sequence/generation/vector_tie.hpp>
#include <boost/fusion/algorithm.hpp>
#include<vector>

#include <boost/assign/std/vector.hpp>
using namespace boost::assign;




namespace usecaseWD {

  // a fake service
  Sample sampleFactory(std::string const & config) {
    SampleFactory sf;
    // shall parse config.... (impl.det.)
    return sf.create(config,config,config);
  }
  
  void globalClient() {
    /// global function, global Sample...
    /// how we get the config????
    static Sample ss(sampleFactory("bha/bha/time,memory"));
    Sampler s(ss);
    // compute....
  }
  
  
  Client::Client(const Config& config) :
    f1sample(sampleFactory(config.find("f1sample")->second)),
    f2sample(sampleFactory(config.find("f2sample")->second)),
    f3sample(sampleFactory(config.find("f3sample")->second))
  {
    // declare in advance type and names (instead of default m_i)
    boost::fusion::vector<double,int,std::vector<double> > v3;
    std::vector<std::string> f3names; f3names+="x","count","energies";
    f3sample.advancedRegistration(v3,f3names.begin(),f3names.end());
    
  }
  
  void Client::act() {
    f1();
    f2();
    f3();
  }
  
  void Client::f1() {
    // create sampler of system quantities (as defined in config)
    Sampler s(f1sample);
    // do same computating
    {
      globalClient();
    }
    // on destruction the sampling is committed
  }

  void Client::f2() const {
    //  create a sampler (start sampling system quantities)  
    Sampler s(f2sample);
    {
      //instantiate local valarray
      std::valarray<double> v(3);
      
      // fill valarray with result of computation available only locally
      v[0] = 5;
      v[1] = 3;
      v[2] = 3.*2+7;
      // copy iy into the sampler
      // s.fill(v); will support this
      s.fill(boost::fusion::make_vector(v));
    }
    // on destruction the "sampling " is committed
  }

 void Client::f3() {
   // create the sampler
   Sampler s(f3sample);
   {
     // instantiate local variables
     double x = 3.14;
     int    c = 05;
     std::vector<double> energies;
     energies += 7.23, 12.2, 45;
     // copy them in the sampler
     // they can go out of scope at this point
     s.fill(boost::fusion::make_vector(x,c,energies));
   }
   // on destruction the sampling is committed
 }

  /*
  // possible alternative interface
  void Client::g1() {
    // instantiate local variables
    // shall not go out of scope before the Sampler
    double x = 0;
    int    c = 0;
    std::vector<double> energies;
    // register them to a sampler
    Sampler s(f1sampler,boost::fusion::vector_tie(x,c,energies));
    // conpute and fill variables
    x = 3.14;
    c = 05;
    energies += 7.23, 12.2, 45;
    // on destruction the sampling is committed
  }

  // one more (simplified?) interface
  void Client::g2() {
    //instantiate local valarray
    // shall not go out of scope before the Sampler
    std::valarray<double> v(5);
    //  register it to a sampler  
    Sampler s(f2sampler,v);
    
    // fill valarray with result of computation available only locally
    v[0] = 5;
    v[1] = 3;
    v[2] = 3.*2+7;
    
    // on destruction valarray is "sampled"
  }
  */

}


int main() {
  usecaseWD::Config config;
  usecaseWD::Client c1(config);

  return 0;

}
