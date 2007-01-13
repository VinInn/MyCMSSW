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
  CPPUNIT_TEST_SUITE(testBaseBranch);
  CPPUNIT_TEST(check_name);
  CPPUNIT_TEST(check_constr);
  CPPUNIT_TEST(check_add);
  CPPUNIT_TEST(check_clean);
  CPPUNIT_TEST(check_write);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
  void tearDown();
  void check_name();
  void check_constr();
  void check_add();
  void check_clean();
  void check_write();

  std::string fname;
  std::string tname;
  std::string bname1;
  std::string bname2;

  typedef boost::fusion::vector<int, char, double, std::string, std::vector<int> > Vec1;
  typedef boost::fusion::vector<long long, double, std::string > Vec2;
  typedef synthetic::Branch<Vec1> Branch1;
  typedef synthetic::Branch<Vec1> Branch2;
  
  pool::IDataSvc *svc;


};

CPPUNIT_TEST_SUITE_REGISTRATION(testBranch);

void testBaseTree::setUp() {
  fname = "fname";
  tname = "tname";
  bname1 = "Branch1";
  bname2 = "Branch2";
  svc = 0;

}

void tearDown() {
  delete svc;
}

void testBranch::check_name() {
  CPPUNIT_ASSERT(synthetic::branchName(tname,bname1)=="tname(Bname1)");
}

void testBranch::check_constr() {
  {
    synthetic::BaseBranch * bs = new Branch1(svc,fname,tname,bname1);
    CPPUNIT_ASSERT (bs!=0);
    delete bs;
  }
  {
    Branch2 b2(svc,fname,tname,bname2);
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
    Branch1 b1(svc,fname,tname,bname1);
    CPPUNIT_ASSERT (!b1.add(std::auto_ptr<Branch1::vec_type>(0)));
    CPPUNIT_ASSERT (!b1.added);
    CPPUNIT_ASSERT (b1.count==0);
    CPPUNIT_ASSERT (!b1.obj);
    CPPUNIT_ASSERT (b1.add(std::auto_ptr<Branch1::vec_type>(new Branch1::vec_type(1,'x',3.3,"hello", std::vector<int>(4,0)))));
  }
}


void testBranch::check_clean() {
  Branch1 b1(svc,fname,tname,bname1);
  CPPUNIT_ASSERT (b1.add(std::auto_ptr<Branch1::vec_type>(new Branch1::vec_type(1,'x',3.3,"hello", std::vector<int>(4,0)))));
  b1.clean();
  CPPUNIT_ASSERT (!b1.added);
  CPPUNIT_ASSERT (b1.count==0);

}
 
void testBranch::check_write() {

}
