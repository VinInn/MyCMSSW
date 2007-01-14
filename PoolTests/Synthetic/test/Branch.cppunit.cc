#define private public
#include "PoolTests/Synthetic/interface/Branch.h"
#undef private

#include <cppunit/extensions/HelperMacros.h>

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>
#include<vector>

#include <boost/assign/std/vector.hpp>
using namespace boost::assign;


class testBranch : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testBranch);
  CPPUNIT_TEST(check_name);
  CPPUNIT_TEST(check_constr);
  CPPUNIT_TEST(check_add);
  CPPUNIT_TEST(check_clean);
  CPPUNIT_TEST(check_write1);
  CPPUNIT_TEST(check_write2);
  CPPUNIT_TEST(check_writeDelayed);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
  void tearDown();
  void check_name();
  void check_constr();
  void check_add();
  void check_clean();
  void check_write1();
  void check_write2();
  void check_writeDelayed();

  std::string fname;
  std::string tname;
  std::string bname1;
  std::string bname2;
  std::vector<std::string> names2;

  typedef boost::fusion::vector<int, char, double, std::string, std::vector<int> > Vec1;
  typedef boost::fusion::vector<long long, double, std::string > Vec2;
  typedef synthetic::Branch<Vec1> Branch1;
  typedef synthetic::Branch<Vec2> Branch2;
  
  pool::IDataSvc *svc;


};

CPPUNIT_TEST_SUITE_REGISTRATION(testBranch);

void testBranch::setUp() {
  fname = "fname";
  tname = "tname";
  bname1 = "Branch1";
  bname2 = "Branch2";
  names2 += "time","delta","comment";
  svc = 0;
}

void testBranch::tearDown() {
  // delete svc;
}

void testBranch::check_name() {
  CPPUNIT_ASSERT(synthetic::branchName(tname,bname1)=="tname(Branch1)");
}

void testBranch::check_constr() {
  {
    synthetic::BaseBranch * bs = new Branch1(svc,fname,tname,bname1);
    CPPUNIT_ASSERT (bs!=0);
    delete bs;
  }
  {
    Branch2 b2(svc,fname,tname,bname2,names2.begin(),names2.end());
    CPPUNIT_ASSERT (!b2.added);
    CPPUNIT_ASSERT (b2.count==0);
    CPPUNIT_ASSERT (!b2.obj);
  }
}

void testBranch::check_add() {
  {
    Branch1 b1(svc,fname,tname,bname1);
    CPPUNIT_ASSERT (b1.add(std::auto_ptr<Branch1::vec_type>(new Branch1::vec_type(1,'x',3.3,"hello", std::vector<int>(4,0)))));
    CPPUNIT_ASSERT (!b1.add(std::auto_ptr<Branch1::vec_type>(new Branch1::vec_type(1,'x',3.3,"hello", std::vector<int>(4,0)))));
    CPPUNIT_ASSERT (b1.added);
    CPPUNIT_ASSERT (b1.count==1);
    CPPUNIT_ASSERT (b1.obj);
  }
  {
    Branch2 b2(svc,fname,tname,bname2);
    CPPUNIT_ASSERT (!b2.add(std::auto_ptr<Branch2::vec_type>(0)));
    CPPUNIT_ASSERT (!b2.added);
    CPPUNIT_ASSERT (b2.count==0);
    CPPUNIT_ASSERT (!b2.obj);
    CPPUNIT_ASSERT (b2.add(std::auto_ptr<Branch2::vec_type>(new Branch2::vec_type(1,3.3,"hello"))));
  }
}


void testBranch::check_clean() {
  Branch1 b1(svc,fname,tname,bname1);
  CPPUNIT_ASSERT (b1.add(std::auto_ptr<Branch1::vec_type>(new Branch1::vec_type(1,'x',3.3,"hello", std::vector<int>(4,0)))));
  b1.clean();
  CPPUNIT_ASSERT (!b1.added);
  CPPUNIT_ASSERT (b1.count==0);
}
 
#include "PoolTests/Synthetic/interface/BaseTree.h"

void testBranch::check_write1() {
  synthetic::BaseTree bt(fname,tname);
  synthetic::BaseTree::Data const & d = bt.data();
  boost::shared_ptr<Branch1> b1(new Branch1(d.svc,d.fname,d.tname,bname1));
  bt.add(bname1,b1);
  // "no mistake here"
  CPPUNIT_ASSERT (b1->add(std::auto_ptr<Branch1::vec_type>(new Branch1::vec_type(1,'x',3.3,"hello", std::vector<int>(4,0)))));
  bt.write();
  CPPUNIT_ASSERT (!b1->added);
  CPPUNIT_ASSERT (b1->count==1);

  CPPUNIT_ASSERT (b1->add(std::auto_ptr<Branch1::vec_type>(new Branch1::vec_type(1,'x',3.3,"hello", std::vector<int>(4,0)))));
  bt.write();
  CPPUNIT_ASSERT (b1->count==2);

}

void testBranch::check_write2() {
  synthetic::BaseTree bt(fname,tname);
  
}

void testBranch::check_writeDelayed() {
  synthetic::BaseTree bt(fname,tname);

}
