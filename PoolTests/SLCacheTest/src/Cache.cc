#include "PoolTest/SLCacheTest/interface/Cache.h"
#include "SealBase/Time.h"

#include<map>
#include<vector>

#include<iostream>

namespace Persil {

  long Cache::now() {
    seal::TimeSpan now(seal::Time::current());
    return now.seconds();
  }
  
  Cache::Item::Item(const std::string& ipayload) :
    count(0),time(0),me(ipayload){}

  Cache::Cache(size_t lowt, size_t hight, long mage, size_t msh) : 
    m_size(0), m_lowTide(lowt), m_highTide(hight),
    m_maxSingleHits(msh), m_maxAge(mage){}


  const std::string& Cache::Item::payload() {
    count++;
    time=Cache::now();
    return me;
  } 

  std::pair<Cache::iterator,bool> Cache::insert(const Token& t, const std::string& ipay) {
    std::pair<Cache::iterator,bool> res = me.insert(std::make_pair(t,Item(ipay)));
    if (res.second) m_size+=ipay.size();
    return res;
  }

  Cache::iterator Cache::find(const Token& t) {
    return me.find(t);
  }

  void Cache::erase( Cache::iterator p) {
	m_size-=(*p).second.me.size();
	me.erase(p);
  }

  void Cache::cleaner(long age) {
    iterator p=me.begin();
    while(p!=me.end()) {
      iterator q=p; p++;
      if ( (*q).second.time<age ) erase(q);
    }
  }

  void Cache::cleanSingleHits(bool single) {
    typedef std::map<std::string, std::vector<iterator> > MAP;
    MAP vi;
    for ( iterator p=me.begin();p!=me.end(); p++) {
      if (single && ( (*p).second.count>1) ) continue;
      // find second ]
      size_t l = (*p).first.find(']'); l = (*p).first.find(']',l+1);
      vi[(*p).first.substr(0,l)].push_back(p);
    }
    for (MAP::iterator p=vi.begin();p!=vi.end();p++) {
      if ( (*p).second.size() <  m_maxSingleHits ) continue;
      std::cout << (*p).first << " has " << (*p).second.size() 
		<< (single ? " single" : " multiple") << " hits" 
		<< ". cleaning"
		<< std::endl;
      for (std::vector<iterator>::iterator z= (*p).second.begin();
	   z!=(*p).second.end();z++) erase(*z);
    }

  }

  void Cache::cleaner() {
    std::cout << "cache size " << m_size 
	      << " for " << me.size() << " objects" << std::endl; 
     if (lowTide()) return;
    // avoid cache trashing due to iterator...
    cleanSingleHits(true);
    if (lowTide()) return;
    // remove old stuff
    cleaner(m_maxAge);
    if (lowTide()) return;
    // remove all big containers
    cleanSingleHits(false);
    if (lowTide()) return;


    // sort item by time
    

  }



}
