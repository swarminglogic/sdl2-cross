#ifndef MATH_TESTPOINTF_H
#define MATH_TESTPOINTF_H

#include <math/Pointf.h>
#include <math/Sizef.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Pointf class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestPointf : public CxxTest::TestSuite
{
 public:
  void testBasic()
  {
    const float delta = 0.00001f;

    // Defaults
    Pointf p;
    TS_ASSERT_EQUALS(p.getX(), 0.0f);
    TS_ASSERT_EQUALS(p.getY(), 0.0f);
    TS_ASSERT_EQUALS(p.x(),    0.0f);
    TS_ASSERT_EQUALS(p.y(),    0.0f);

    // operators:   ==   !=
    Pointf pSame(0.0f, 0.0f);
    TS_ASSERT_EQUALS(p, pSame);
    TS_ASSERT(p == pSame);
    Pointf pClose(0.0f, 0.00001f);
    TS_ASSERT(!(p == pClose));
    TS_ASSERT(p != pClose);
    TS_ASSERT_DIFFERS(p, Pointf(0.0f, 0.0001f));
    TS_ASSERT_DIFFERS(p, Pointf(0.0001f, 0.0f));

    // Getters
    Pointf p2(5.0f, 123.12f);
    TS_ASSERT_DELTA(p2.getX(), 5.0f,     delta);
    TS_ASSERT_DELTA(p2.x(),    5.0f,     delta);
    TS_ASSERT_DELTA(p2.getY(), 123.12f,  delta);
    TS_ASSERT_DELTA(p2.y(),    123.12f,  delta);

    // Getters, setters
    Pointf p3;
    p3.setY(12.0f);
    TS_ASSERT_DELTA(p3.getY(), 12.0f, delta);
    TS_ASSERT_DELTA(p3.getX(), 0.0f, delta);
    TS_ASSERT_DELTA(p3.y(), 12.0f, delta);
    TS_ASSERT_DELTA(p3.x(), 0.0f, delta);

    p3.setX(42.0f);
    TS_ASSERT_DELTA(p3.getX(), 42.0f, delta);
    TS_ASSERT_DELTA(p3.x(), 42.0f, delta);

    p3.x(51.0f);
    TS_ASSERT_DELTA(p3.x(), 51.0f, delta);

    p3.y(3.0f);
    TS_ASSERT_DELTA(p3.y(), 3.0f, delta);

    // Copy
    Pointf A(41.0f, 23.0f);
    Pointf B = A;
    TS_ASSERT_EQUALS(A, B);
    Pointf C(A);
    TS_ASSERT_EQUALS(A, C);
  }


  void testClassOperators() {
    Pointf opA(2, 3);
    Pointf opB(5, 7);
    opB += opA;
    TS_ASSERT_EQUALS(opB, Pointf(7, 10));
    opA -= Pointf(1, 1);
    TS_ASSERT_EQUALS(opA, Pointf(1, 2));
    opB -= opB;
    TS_ASSERT_EQUALS(opB, Pointf(0, 0));
    Pointf opC(5, -3);
    opC *= 3.0;
    TS_ASSERT_EQUALS(opC, Pointf(15, -9));

    const float delta = 0.00001f;
    opC /= 3.0;
    TS_ASSERT_DELTA(opC.x(), 5.0f, delta);
    TS_ASSERT_DELTA(opC.y(), -3.0f, delta);

    Pointf opD(14, 33);
    opD /= 3.4f;
    TS_ASSERT_DELTA(opD.x(), 4.117647059f, delta);
    TS_ASSERT_DELTA(opD.y(), 9.705882353f, delta);
  }

  void testFriendOperators() {
    const Pointf opA(2, 3);
    const Pointf opAcp(2, 3);
    const Pointf opB(5, 7);

    // Comparison
    TS_ASSERT_DIFFERS(opA, opB);
    TS_ASSERT_EQUALS(opA, opA);
    TS_ASSERT_EQUALS(opA, opAcp);
    TS_ASSERT_EQUALS(opB, opB);
    TS_ASSERT(opA != opB);

    // Addition
    Pointf resAdd = opA + opB;
    Pointf resAddExp(7.0f, 10.0f);
    TS_ASSERT_EQUALS(resAdd, resAddExp);

    // Substraction
    Pointf resSub = opA - opB;
    Pointf resSubExp(-3.0f, -4.0f);
    TS_ASSERT_EQUALS(resSub, resSubExp);

  //   // Multiplication
    Pointf resMult = opA * 0.5f;
    Pointf resMultExp(1.0f, 1.5f);
    TS_ASSERT_EQUALS(resMult, resMultExp);
    Pointf resMult2 = 3.2f * opA;
    Pointf resMult2Exp(6.4f, 9.6f);
    TS_ASSERT_EQUALS(resMult2, resMult2Exp);

  //   // Division
    Pointf resDiv = opB / 2.0f;
    Pointf resDivExp(2.5f, 3.5f);
    TS_ASSERT_EQUALS(resDiv, resDivExp);
  }

  void testExplicitTypeConversion() {
    const float delta = 0.00001f;
    Pointf pt(1.0f, 2.0f);
    Sizef op = static_cast<Sizef>(pt);
    TS_ASSERT_DELTA(op.w(), pt.x(), delta);
    TS_ASSERT_DELTA(op.h(), pt.y(), delta);
  }

  void testGetLength() {
    const float delta = 0.00001f;
    Pointf opA(2.0f, 0.0f);
    TS_ASSERT_DELTA(opA.getLength(), 2.0f, delta);
    opA = Pointf(3.0f, 4.0f);
    TS_ASSERT_DELTA(opA.getLength(), 5.0f, delta);
    opA = Pointf(4.0f, 3.0f);
    TS_ASSERT_DELTA(opA.getLength(), 5.0f, delta);
    opA = Pointf(-4.0f, 3.0f);
    TS_ASSERT_DELTA(opA.getLength(), 5.0f, delta);
  }

  void testNormalize() {
    const float delta = 0.00001f;
    Pointf opA(2.0f, 0.0f);
    opA.normalize();
    TS_ASSERT_DELTA(opA.getLength(), 1.0f, delta);
    TS_ASSERT_DELTA(opA.x(), 1.0f, delta);
    TS_ASSERT_DELTA(opA.y(), 0.0f, delta);

    opA = Pointf(1.0f, 1.0f);
    opA.normalize();
    TS_ASSERT_DELTA(opA.getLength(), 1.0f, delta);
    TS_ASSERT_DELTA(opA.x(), 0.7071f, delta);
    TS_ASSERT_DELTA(opA.y(), 0.7071f, delta);

    opA = Pointf(-1.0f, 1.0f);
    opA.normalize();
    TS_ASSERT_DELTA(opA.getLength(), 1.0f, delta);
    TS_ASSERT_DELTA(opA.x(), -0.7071f, delta);
    TS_ASSERT_DELTA(opA.y(), 0.7071f, delta);
  }

  void testTranspose() {
    Pointf opA(2.0f, 3.0f);
    opA.transpose();
    TS_ASSERT_EQUALS(opA, Pointf(3.0f, 2.0f));
  }

  void testGetData() {
    const float delta = 0.00001f;
    const Pointf cpoint(2.0f, 3.0f);
    TS_ASSERT_DELTA(cpoint.getData()[0], cpoint.x(), delta);
    TS_ASSERT_DELTA(cpoint.getData()[1], cpoint.y(), delta);
    Pointf point(5.0f, 6.0f);
    TS_ASSERT_DELTA(point.getData()[0], point.x(), delta);
    TS_ASSERT_DELTA(point.getData()[1], point.y(), delta);
    point.getData()[0] = 12.0f;
    point.getData()[1] = 42.0f;
    TS_ASSERT_DELTA(point.x(), 12.0f, delta);
    TS_ASSERT_DELTA(point.y(), 42.0f, delta);
  }

 private:
};

#endif  // MATH_TESTPOINTF_H
