#ifndef MATH_TESTPOINT_H
#define MATH_TESTPOINT_H

#include <math/Point.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Point class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestPoint : public CxxTest::TestSuite
{
 public:
  void testBasic()
  {
    Point point;
    TS_ASSERT_EQUALS(point.getX(), 0);
    TS_ASSERT_EQUALS(point.getY(), 0);
    TS_ASSERT_EQUALS(point.x(),    0);
    TS_ASSERT_EQUALS(point.y(),    0);

    Point point2(5, 123);
    TS_ASSERT_EQUALS(point2.getX(),  5);
    TS_ASSERT_EQUALS(point2.getY(), 123);
    TS_ASSERT_EQUALS(point2.x(),  5);
    TS_ASSERT_EQUALS(point2.y(), 123);

    Point point3;
    point3.setY(12);
    TS_ASSERT_EQUALS(point3.getY(), 12);
    TS_ASSERT_EQUALS(point3.getX(), 0);
    TS_ASSERT_EQUALS(point3.y(), 12);
    TS_ASSERT_EQUALS(point3.x(), 0);
    point3.setX(42);
    TS_ASSERT_EQUALS(point3.getX(), 42);
    TS_ASSERT_EQUALS(point3.x(), 42);
    point3.x(51);
    TS_ASSERT_EQUALS(point3.x(), 51);
    point3.y(3);
    TS_ASSERT_EQUALS(point3.y(), 3);

    // Copy constructor
    Point A(41, 23);
    Point B = A;
    TS_ASSERT_EQUALS(A, B);
    Point C(A);
    TS_ASSERT_EQUALS(A, C);
  }

  void testClassOperators() {
    Point opA(2, 3);
    Point opB(5, 7);
    opB += opA;
    TS_ASSERT_EQUALS(opB, Point(7, 10));
    opA -= Point(1, 1);
    TS_ASSERT_EQUALS(opA, Point(1, 2));
    opB -= opB;
    TS_ASSERT_EQUALS(opB, Point(0, 0));
    Point opC(5, -3);
    opC *= 3.0;
    TS_ASSERT_EQUALS(opC, Point(15, -9));

    opC /= 3.0;
    TS_ASSERT_EQUALS(opC, Point(5, -3));

    Point opD(14, 33);
    opD /= 3.4f;
    TS_ASSERT_EQUALS(opD, Point(4, 9));
  }

  void testFriendOperators() {
    Point opA(2, 3);
    Point opAcp(2, 3);
    Point opB(5, 7);
    Point opC(2, 47);
    Point opD(47, 3);

    // Comparison
    TS_ASSERT_DIFFERS(opA, opB);
    TS_ASSERT_EQUALS(opA, opA);
    TS_ASSERT_EQUALS(opA, opAcp);
    TS_ASSERT_EQUALS(opB, opB);
    TS_ASSERT(!(opA != opAcp));
    TS_ASSERT(opA != opC);
    TS_ASSERT(opA != opD);

    // Addition
    Point resAdd = opA + opB;
    Point resAddExp(7, 10);
    TS_ASSERT_EQUALS(resAdd, resAddExp);

    // Substraction
    Point resSub = opA - opB;
    Point resSubExp(-3, -4);
    TS_ASSERT_EQUALS(resSub, resSubExp);

    // Multiplication
    Point resMult = opA * 2.0;
    Point resMultExp(4, 6);
    TS_ASSERT_EQUALS(resMult, resMultExp);
    Point resMult2 = 3.0 * opA;
    Point resMult2Exp(6, 9);
    TS_ASSERT_EQUALS(resMult2, resMult2Exp);

    // Division
    Point resDiv = opB / 2.0;
    Point resDivExp(3, 4);
    TS_ASSERT_EQUALS(resDiv, resDivExp);
  }

  void testTranspose() {
    Point opA(2, 3);
    opA.transpose();
    TS_ASSERT_EQUALS(opA, Point(3, 2));
  }


  void testGetData() {
    const Point cpoint(2, 3);
    TS_ASSERT_EQUALS(cpoint.getData()[0], cpoint.x());
    TS_ASSERT_EQUALS(cpoint.getData()[1], cpoint.y());
    Point point(5, 6);
    TS_ASSERT_EQUALS(point.getData()[0], point.x());
    TS_ASSERT_EQUALS(point.getData()[1], point.y());
    point.getData()[0] = 12;
    point.getData()[1] = 42;
    TS_ASSERT_EQUALS(point.x(), 12);
    TS_ASSERT_EQUALS(point.y(), 42);
  }

 private:
};

#endif  // MATH_TESTPOINT_H
