#ifndef MATH_TESTSIZEF_H
#define MATH_TESTSIZEF_H

#include <math/Pointf.h>
#include <math/Sizef.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Sizef class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestSizef : public CxxTest::TestSuite
{
public:
  void testBasic()
  {
    const float delta = 0.00001f;

    // Defaults
    Sizef p;
    TS_ASSERT_EQUALS(p.getWidth(), 0.0f);
    TS_ASSERT_EQUALS(p.getHeight(), 0.0f);
    TS_ASSERT_EQUALS(p.width(),    0.0f);
    TS_ASSERT_EQUALS(p.height(),    0.0f);

    // operators:   ==   !=
    Sizef pSame(0.0f, 0.0f);
    TS_ASSERT_EQUALS(p, pSame);
    TS_ASSERT(p == pSame);
    Sizef pClose(0.0f, 0.00001f);
    TS_ASSERT(!(p == pClose));
    TS_ASSERT(p != pClose);
    TS_ASSERT_DIFFERS(p, Sizef(0.0f, 0.0001f));
    TS_ASSERT_DIFFERS(p, Sizef(0.0001f, 0.0f));

    // Getters
    Sizef p2(5.0f, 123.12f);
    TS_ASSERT_DELTA(p2.getWidth(), 5.0f,     delta);
    TS_ASSERT_DELTA(p2.width(),    5.0f,     delta);
    TS_ASSERT_DELTA(p2.getHeight(), 123.12f,  delta);
    TS_ASSERT_DELTA(p2.height(),    123.12f,  delta);

    // Getters, setters
    Sizef p3;
    p3.setHeight(12.0f);
    TS_ASSERT_DELTA(p3.getHeight(), 12.0f, delta);
    TS_ASSERT_DELTA(p3.getWidth(), 0.0f, delta);
    TS_ASSERT_DELTA(p3.height(), 12.0f, delta);
    TS_ASSERT_DELTA(p3.width(), 0.0f, delta);

    p3.setWidth(42.0f);
    TS_ASSERT_DELTA(p3.getWidth(), 42.0f, delta);
    TS_ASSERT_DELTA(p3.width(), 42.0f, delta);

    p3.width(51.0f);
    TS_ASSERT_DELTA(p3.width(), 51.0f, delta);

    p3.height(3.0f);
    TS_ASSERT_DELTA(p3.height(), 3.0f, delta);

    // Copy
    Sizef A(41.0f, 23.0f);
    Sizef B = A;
    TS_ASSERT_EQUALS(A, B);
    Sizef C(A);
    TS_ASSERT_EQUALS(A, C);
  }


  void testClassOperators() {
    Sizef opA(2, 3);
    Sizef opB(5, 7);
    opB += opA;
    TS_ASSERT_EQUALS(opB, Sizef(7, 10));
    opA -= Sizef(1,1);
    TS_ASSERT_EQUALS(opA, Sizef(1, 2));
    opB -= opB;
    TS_ASSERT_EQUALS(opB, Sizef(0, 0));
    Sizef opC(5,-3);
    opC *= 3.0;
    TS_ASSERT_EQUALS(opC, Sizef(15, -9));


    const float delta = 0.00001f;
    opC /= 3.0;
    TS_ASSERT_DELTA(opC.w(), 5.0f, delta);
    TS_ASSERT_DELTA(opC.h(), -3.0f, delta);
    Sizef opD(14, 33);
    opD /= 3.4f;
    TS_ASSERT_DELTA(opD.w(), 4.117647059f, delta);
    TS_ASSERT_DELTA(opD.h(), 9.705882353f, delta);
  }

  void testFriendOperators() {
    const Sizef opA(2, 3);
    const Sizef opAcp(2, 3);
    const Sizef opB(5, 7);

    // Comparison
    TS_ASSERT_DIFFERS(opA, opB);
    TS_ASSERT_EQUALS(opA, opA);
    TS_ASSERT_EQUALS(opA, opAcp);
    TS_ASSERT_EQUALS(opB, opB);
    TS_ASSERT(opA != opB);

    // Addition
    Sizef resAdd = opA + opB;
    Sizef resAddExp(7.0f, 10.0f);
    TS_ASSERT_EQUALS(resAdd, resAddExp);

    // Substraction
    Sizef resSub = opA - opB;
    Sizef resSubExp(-3.0f, -4.0f);
    TS_ASSERT_EQUALS(resSub, resSubExp);

  //   // Multiplication
    Sizef resMult = opA * 0.5f;
    Sizef resMultExp(1.0f, 1.5f);
    TS_ASSERT_EQUALS(resMult, resMultExp);
    Sizef resMult2 = 3.2f * opA;
    Sizef resMult2Exp(6.4f, 9.6f);
    TS_ASSERT_EQUALS(resMult2, resMult2Exp);

  //   // Division
    Sizef resDiv = opB / 2.0f;
    Sizef resDivExp(2.5f, 3.5f);
    TS_ASSERT_EQUALS(resDiv, resDivExp);
  }

  void testTranspose() {
    Sizef opA(2.0f, 3.0f);
    opA.transpose();
    TS_ASSERT_EQUALS(opA, Sizef(3.0f, 2.0f));
  }

  void testGetLength() {
    const float delta = 0.00001f;
    Sizef opA(2.0f, 0.0f);
    TS_ASSERT_DELTA(opA.getLength(), 2.0f, delta);
    opA = Sizef(3.0f, 4.0f);
    TS_ASSERT_DELTA(opA.getLength(), 5.0f, delta);
    opA = Sizef(4.0f, 3.0f);
    TS_ASSERT_DELTA(opA.getLength(), 5.0f, delta);
    opA = Sizef(-4.0f, 3.0f);
    TS_ASSERT_DELTA(opA.getLength(), 5.0f, delta);
  }


  void testNormalize() {
    const float delta = 0.00001f;
    Sizef opA(2.0f, 0.0f);
    opA.normalize();
    TS_ASSERT_DELTA(opA.getLength(), 1.0f, delta);
    TS_ASSERT_DELTA(opA.w(), 1.0f, delta);
    TS_ASSERT_DELTA(opA.h(), 0.0f, delta);

    opA = Sizef(1.0f, 1.0f);
    opA.normalize();
    TS_ASSERT_DELTA(opA.getLength(), 1.0f, delta);
    TS_ASSERT_DELTA(opA.w(), 0.7071f, delta);
    TS_ASSERT_DELTA(opA.h(), 0.7071f, delta);

    opA = Sizef(-1.0f, 1.0f);
    opA.normalize();
    TS_ASSERT_DELTA(opA.getLength(), 1.0f, delta);
    TS_ASSERT_DELTA(opA.w(), -0.7071f, delta);
    TS_ASSERT_DELTA(opA.h(), 0.7071f, delta);
  }


  void testGetData() {
    const float delta = 0.00001f;
    const Sizef csize(2.0f, 3.0f);
    TS_ASSERT_DELTA(csize.getData()[0], csize.w(), delta);
    TS_ASSERT_DELTA(csize.getData()[1], csize.h(), delta);
    Sizef size(5.0f, 6.0f);
    TS_ASSERT_DELTA(size.getData()[0], size.w(), delta);
    TS_ASSERT_DELTA(size.getData()[1], size.h(), delta);
    size.getData()[0] = 12.0f;
    size.getData()[1] = 42.0f;
    TS_ASSERT_DELTA(size.w(), 12.0f, delta);
    TS_ASSERT_DELTA(size.h(), 42.0f, delta);
  }

  void testExplicitTypeConversion() {
    const float delta = 0.00001f;
    Sizef op(1.0f, 2.0f);
    Pointf pt = static_cast<Pointf>(op);
    TS_ASSERT_DELTA(op.w(), pt.x(), delta);
    TS_ASSERT_DELTA(op.h(), pt.y(), delta);
  }

private:
};

#endif
