#include "PoolTests/Synthetic/interface/BaseBranch.h"


#include <cppunit/extensions/HelperMacros.h>


namespace {

  struct Stub : public BaseBranch {
    Stub() : m_globalCount(0){}
    virtual  void write(int globalCount) {
      m_globalCount=globalCount;
    }

    int m_globalCount;

  };

}

class testBaseBranch : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testBaseBranch);
  CPPUNIT_TEST(check_constr);
  CPPUNIT_TEST(check_write);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp(){}
  void tearDown() {}
  void check_constr();
  void check_write();
};

CPPUNIT_TEST_SUITE_REGISTRATION(testBaseBranch);

void testBaseBranch::check_constr() {
  BaseBranch * bs = new Stub();
  CPPUNIT_ASSERT (bs!=0);
  delete bs;
}

void testBaseBranch::check_write() {
  BaseBranch * bs = new Stub();
  bs.write(3);
  Stub * s = dynamic_cast<Stub*>(bs);
  CPPUNIT_ASSERT (s!=0);
  CPPUNIT_ASSERT (s->m_globalCount==3);

  delete bs;

}
