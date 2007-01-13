#include "PoolTests/Synthetic/interface/BaseTree.h"
#include "PoolTests/Synthetic/interface/BaseBranch.h"


#include <cppunit/extensions/HelperMacros.h>


namespace {

  struct Stub : public synthetic::BaseBranch {
    Stub() : m_globalCount(0){}
    virtual  void write(int globalCount) {
      m_globalCount=globalCount;
    }

    int m_globalCount;

  };

}

class testBaseTree : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testBaseTree);
  CPPUNIT_TEST(check_catalog);
  CPPUNIT_TEST(check_data);
  CPPUNIT_TEST(check_constr);
  CPPUNIT_TEST(check_add);
  CPPUNIT_TEST(check_find);
  CPPUNIT_TEST(check_write);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
  void tearDown() {}
  void check_catalog();
  void check_data();
  void check_constr();
  void check_add();
  void check_find();
  void check_write();

  static Stub * add3Branches(synthetic::BaseTree& bt, bool verify=false);

  std::string fname;
  std::string tname;
  
};

CPPUNIT_TEST_SUITE_REGISTRATION(testBaseTree);

void testBaseTree::setUp() {
  fname = "fname";
  tname = "tname";

}

#include "FileCatalog/IFileCatalog.h"

void testBaseTree::check_catalog() {
  {
    synthetic::BaseTree::Catalog cat(boost::shared_ptr<pool::IFileCatalog>());
    CPPUNIT_ASSERT (cat.cat!=0);
  }
  {
    boost::shared_ptr<pool::IFileCatalog> p(new pool::IFileCatalog);
    synthetic::BaseTree::Catalog cat(p);
    CPPUNIT_ASSERT (cat.cat==p);
  }
}


void testBaseTree::check_data() {
  synthetic::BaseTree::Data d(fname,tname,0);
  CPPUNIT_ASSERT (d.fname==fname);
  CPPUNIT_ASSERT (d.tname==tname);
  CPPUNIT_ASSERT (d.svc==0);
}

void testBaseTree::check_constr() {
  synthetic::BaseTree bt(fname,tname);
  synthetic::BaseTree::Data const & d = bt.data();
  CPPUNIT_ASSERT (d.fname==fname);
  CPPUNIT_ASSERT (d.tname==tname);
  CPPUNIT_ASSERT (d.svc==0);
}

void testBaseTree::check_add() {
  synthetic::BaseTree bt(fname,tname);
  add3Branches(bt,true);
}

void  testBaseTree::check_find() {
  synthetic::BaseTree bt(fname,tname);
  add3Branches(bt,true);
  CPPUNIT_ASSERT (bt.find("1"));
  CPPUNIT_ASSERT (bt.find("1"));
  CPPUNIT_ASSERT (bt.find("2"));
  CPPUNIT_ASSERT (bt.find("3"));
  CPPUNIT_ASSERT (!bt.find("4"));
  CPPUNIT_ASSERT (!bt.find("5"));
  CPPUNIT_ASSERT (bt.find("99"));

}

void testBaseTree::check_write() {
  synthetic::BaseTree bt(fname,tname);
  Stub * bs = add3Branches(bt,false);

  bt.write();
  bt.write();
  bt.write();

  CPPUNIT_ASSERT (bs->m_globalCount==3);

} 

Stub *  testBaseTree::add3Branches(synthetic::BaseTree& bt, bool verify) {
  Stub * bs = new Stub();
  CPPUNIT_ASSERT (bt.add("1", bs));
  CPPUNIT_ASSERT (bs);
  CPPUNIT_ASSERT (bt.add("2", new Stub()));
  CPPUNIT_ASSERT (bt.add("3", new Stub()));

  if (verify) {
    CPPUNIT_ASSERT (!bt.add("1", bs));
    
    // this is not protected....
    CPPUNIT_ASSERT (bt.add("99", bs));
  }
  CPPUNIT_ASSERT (bs);
  return bs;
}
