#ifndef Persil_Cache_H
#define Persil_Cache_H


#include <string>
#include<map>

namespace Persil {

  /** a "smart cache...

  */
  class Cache {
  public:

    static long now();

    struct Item {

      explicit Item(const std::string& ipayload);

      // will update count & time
      const std::string& payload();

      size_t count;
      long time;
      std::string me;

    };

    typedef std::string Token;
    typedef std::map<Token, Item> ME;
    typedef ME::iterator iterator;
    typedef ME::value_type value_type;

    //
    explicit Cache(size_t lowt=300000, size_t hight=450000, 
	  long mage=7*24*3600, size_t msh=10);

    //
    std::pair<iterator,bool> insert(const Token& t, const std::string& ipay);

    //
    iterator find(const Token& t);
    
    iterator end() { return me.end();}

    //
    void erase(iterator p);

   // smart cleaner
    void cleaner();

    // old junk cleaner
    void cleaner(long age);

    // remove single container single hits
    // if single othewise not just sigle hits
    void cleanSingleHits(bool single=true);


    bool lowTide() const {
      return m_size<m_lowTide;
    }
    
    bool highTide() const {
      return (!lowTide())&& (m_size<m_highTide);
    }

    bool floaded() const {
      return m_size>m_highTide;

    }

  private:
    
    ME me;
    size_t m_size;
    size_t m_lowTide;
    size_t m_highTide;

    size_t m_maxSingleHits;
    long m_maxAge;

  };

}

#endif
