#include "PerfTools/Sampler/interface/SamplerUDQ.h"
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
using namespace boost::assign;


class TestUDQ : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestUDQ);
  CPPUNIT_TEST(check_Convert);
  CPPUNIT_TEST(check_constr);
  CPPUNIT_TEST_SUITE_END();
public:
  TestUDQ();
  void setUp() {}
  void tearDown() {}
  void check_Convert();
  void check_constr();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestSample);

TestUDQ::TestUDQ(){}


TestUDQ::check_Convert() {

  typedef boost::fusion::vector<int, char, double, char const*> Seq1;
  Seq1 s1(1, 'x', 3.3, "hello");
  typedef boost::fusion::vector<int, char, double, long long> Seq2;
  Seq2 s2(1, 'x', 3.3, 1234567890);
 
  {
    std::vector<std::string> vt; vt += "1", 'x', "3.3", "hello";
    std::vector<std::string> vs(boost::fusion::size(s1));
    boost::fusion::fold(s1,vs.begin(), perftools::udqUtils::Convert());
    CPPUNIT_ASSERT(vs==vt);
  }
  
  {
    std::vector<int> vt; vt+= 1, 'x', 3.3, 1234567890;
    std::vector<int> vs(boost::fusion::size(s2));
    boost::fusion::fold(s2,vs.begin(),perftools::udqUtils::Convert());
    CPPUNIT_ASSERT(vs==vt);
  }

}
