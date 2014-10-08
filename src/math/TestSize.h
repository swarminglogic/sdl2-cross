#ifndef MATH_TESTSIZE_H
#define MATH_TESTSIZE_H

#include <math/Size.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Size class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestSize : public CxxTest::TestSuite
{
 public:
  void testBasic()
  {
    Size size;
    TS_ASSERT_EQUALS(size.getWidth(),  0);
    TS_ASSERT_EQUALS(size.getHeight(), 0);
    TS_ASSERT_EQUALS(size.width(),    0);
    TS_ASSERT_EQUALS(size.height(),    0);

    Size size2(5, 123);
    TS_ASSERT_EQUALS(size2.getWidth(),  5);
    TS_ASSERT_EQUALS(size2.getHeight(), 123);
    TS_ASSERT_EQUALS(size2.width(),  5);
    TS_ASSERT_EQUALS(size2.height(), 123);

    Size size3;
    size3.setHeight(12);
    TS_ASSERT_EQUALS(size3.getHeight(), 12);
    TS_ASSERT_EQUALS(size3.getWidth(), 0);
    TS_ASSERT_EQUALS(size3.height(), 12);
    TS_ASSERT_EQUALS(size3.width(), 0);
    size3.setWidth(42);
    TS_ASSERT_EQUALS(size3.getWidth(), 42);
    TS_ASSERT_EQUALS(size3.width(), 42);
    size3.width(51);
    TS_ASSERT_EQUALS(size3.width(), 51);
    size3.height(3);
    TS_ASSERT_EQUALS(size3.height(), 3);

    // Copy constructor
    Size A(41, 23);
    Size B = A;
    TS_ASSERT_EQUALS(A, B);
    Size C(A);
    TS_ASSERT_EQUALS(A, C);
  }

  void testClassOperators() {
    Size opA(2, 3);
    Size opB(5, 7);
    opB += opA;
    TS_ASSERT_EQUALS(opB, Size(7, 10));
    opA -= Size(1, 1);
    TS_ASSERT_EQUALS(opA, Size(1, 2));
    opB -= opB;
    TS_ASSERT_EQUALS(opB, Size(0, 0));
    Size opC(5, -3);
    opC *= 3.0;
    TS_ASSERT_EQUALS(opC, Size(15, -9));

    opC /= 3.0;
    TS_ASSERT_EQUALS(opC, Size(5, -3));

    Size opD(14, 33);
    opD /= 3.4f;
    TS_ASSERT_EQUALS(opD, Size(4, 9));
  }

  void testFriendOperators() {
    Size opA(2, 3);
    Size opAcp(2, 3);
    Size opB(5, 7);
    Size opC(2, 47);
    Size opD(47, 3);

    // Comparison
    TS_ASSERT_DIFFERS(opA, opB);
    TS_ASSERT_EQUALS(opA, opA);
    TS_ASSERT_EQUALS(opA, opAcp);
    TS_ASSERT_EQUALS(opB, opB);
    TS_ASSERT(!(opA != opAcp));
    TS_ASSERT(opA != opC);
    TS_ASSERT(opA != opD);
    TS_ASSERT(!(opA == opC));
    TS_ASSERT(!(opA == opD));

    // Addition
    Size resAdd = opA + opB;
    Size resAddExp(7, 10);
    TS_ASSERT_EQUALS(resAdd, resAddExp);

    // Substraction
    Size resSub = opA - opB;
    Size resSubExp(-3, -4);
    TS_ASSERT_EQUALS(resSub, resSubExp);

    // Multiplication
    Size resMult = opA * 2.0;
    Size resMultExp(4, 6);
    TS_ASSERT_EQUALS(resMult, resMultExp);
    Size resMult2 = 3.0 * opA;
    Size resMult2Exp(6, 9);
    TS_ASSERT_EQUALS(resMult2, resMult2Exp);

    // Division
    Size resDiv = opB / 2.0;
    Size resDivExp(3, 4);
    TS_ASSERT_EQUALS(resDiv, resDivExp);
  }

  void testTranspose() {
    Size opA(2, 3);
    opA.transpose();
    TS_ASSERT_EQUALS(opA, Size(3, 2));
  }

  void testGetData() {
    const Size csize(2, 3);
    TS_ASSERT_EQUALS(csize.getData()[0], csize.w());
    TS_ASSERT_EQUALS(csize.getData()[1], csize.h());
    Size size(5, 6);
    TS_ASSERT_EQUALS(size.getData()[0], size.w());
    TS_ASSERT_EQUALS(size.getData()[1], size.h());
    size.getData()[0] = 12;
    size.getData()[1] = 42;
    TS_ASSERT_EQUALS(size.w(), 12);
    TS_ASSERT_EQUALS(size.h(), 42);
  }

 private:
};

#endif  // MATH_TESTSIZE_H
