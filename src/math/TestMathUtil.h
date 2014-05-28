#ifndef MATH_TESTMATHUTIL_H
#define MATH_TESTMATHUTIL_H

#include <math/MathUtil.h>
#include <math/Rectf.h>
#include <math/Size.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the MathUtil class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestMathUtil : public CxxTest::TestSuite
{
public:
  void testClamp() {
    const float delta = 0.0001f;
    TS_ASSERT_DELTA(MathUtil::clamp(1.0f, -1.0f, 1.0f), 1.0f,delta);
    TS_ASSERT_DELTA(MathUtil::clamp(0.0f, -1.0f, 1.0f), 0.0f,delta);
    TS_ASSERT_DELTA(MathUtil::clamp(1.2f, -1.0f, 1.0f), 1.0f,delta);
    TS_ASSERT_DELTA(MathUtil::clamp(-1.2f, -1.0f, 1.0f), -1.0f,delta);
    TS_ASSERT_DELTA(MathUtil::clamp(-0.2f, -1.0f, 1.0f), -0.2f,delta);
    TS_ASSERT_DELTA(MathUtil::clamp(-1.2f, -10.0f, 1.0f), -1.2f,delta);

    // Default ranges [0, 1]
    TS_ASSERT_DELTA(MathUtil::clamp(1.0f), 1.0f,delta);
    TS_ASSERT_DELTA(MathUtil::clamp(0.0f), 0.0f,delta);
    TS_ASSERT_DELTA(MathUtil::clamp(1.2f), 1.0f,delta);
    TS_ASSERT_DELTA(MathUtil::clamp(-1.2f), 0.0f,delta);
  }

  void testLerp() {
    const float delta = 0.0001f;
    TS_ASSERT_DELTA(MathUtil::lerp(2.0f, 4.0f, 0.0f), 2.0f, delta);
    TS_ASSERT_DELTA(MathUtil::lerp(2.0f, 4.0f,-5.0f), 2.0f, delta);
    TS_ASSERT_DELTA(MathUtil::lerp(2.0f, 4.0f, 1.0f), 4.0f, delta);
    TS_ASSERT_DELTA(MathUtil::lerp(2.0f, 4.0f, 5.0f), 4.0f, delta);
    TS_ASSERT_DELTA(MathUtil::lerp(2.0f, 4.0f, 0.5f), 3.0f, delta);
    TS_ASSERT_DELTA(MathUtil::lerp(-2.0f, 4.0f, 0.0f), -2.0f, delta);
    TS_ASSERT_DELTA(MathUtil::lerp(-2.0f, 4.0f, 0.5f), 1.0f, delta);
    TS_ASSERT_DELTA(MathUtil::lerp(-2.0f, 4.0f, 1.0f), 4.0f, delta);
  }

  void testNextPow2A()
  {
    TS_ASSERT_EQUALS(MathUtil::nextPow2(0), 0); // Invalid use in many cases.
    TS_ASSERT_EQUALS(MathUtil::nextPow2(1), 1);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(2), 2);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(3), 4);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(4), 4);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(5), 8);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(8), 8);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(9), 16);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(12), 16);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(16), 16);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(127), 128);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(128), 128);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(129), 256);
    TS_ASSERT_EQUALS(MathUtil::nextPow2(131000), 131072);

    unsigned int pof2 = 1;
    for (size_t i = 1 ; i < 100000u ; ++i) {
      TS_ASSERT_EQUALS(MathUtil::nextPow2((int)i), pof2);
      if (i == pof2) pof2 *= 2;
    }
  }

  void testNextSquare()
  {
    TS_ASSERT_EQUALS(MathUtil::nextSquare(0), 0);
    TS_ASSERT_EQUALS(MathUtil::nextSquare(1), 1);
    TS_ASSERT_EQUALS(MathUtil::nextSquare(3), 4);
    TS_ASSERT_EQUALS(MathUtil::nextSquare(4), 4);
    TS_ASSERT_EQUALS(MathUtil::nextSquare(5), 9);
    TS_ASSERT_EQUALS(MathUtil::nextSquare(14), 16);
    TS_ASSERT_EQUALS(MathUtil::nextSquare(20), 25);

    TS_ASSERT_EQUALS(MathUtil::nextSquare(114*114), 114*114);
    TS_ASSERT_EQUALS(MathUtil::nextSquare(114*114+1), 115*115);
    TS_ASSERT_EQUALS(MathUtil::nextSquare(1234*1234), 1234*1234);
    TS_ASSERT_EQUALS(MathUtil::nextSquare(1234*1234 - 1), 1234*1234);
    TS_ASSERT_EQUALS(MathUtil::nextSquare(1234*1234 + 1), 1235*1235);
  }


  void testNextSquareBase()
  {
    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(0), 0);
    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(1), 1);
    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(3), 2);
    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(4), 2);
    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(5), 3);
    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(14), 4);
    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(20), 5);

    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(114*114), 114);
    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(114*114+1), 115);
    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(1234*1234), 1234);
    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(1234*1234 - 1), 1234);
    TS_ASSERT_EQUALS(MathUtil::nextSquareBase(1234*1234 + 1), 1235);
  }


  void testPrevSquare()
  {
    TS_ASSERT_EQUALS(MathUtil::prevSquare(0), 0);
    TS_ASSERT_EQUALS(MathUtil::prevSquare(1), 1);
    TS_ASSERT_EQUALS(MathUtil::prevSquare(3), 1);
    TS_ASSERT_EQUALS(MathUtil::prevSquare(4), 4);
    TS_ASSERT_EQUALS(MathUtil::prevSquare(5), 4);
    TS_ASSERT_EQUALS(MathUtil::prevSquare(14), 9);
    TS_ASSERT_EQUALS(MathUtil::prevSquare(20), 16);

    TS_ASSERT_EQUALS(MathUtil::prevSquare(114*114), 114*114);
    TS_ASSERT_EQUALS(MathUtil::prevSquare(114*114 + 1), 114*114);
    TS_ASSERT_EQUALS(MathUtil::prevSquare(114*114 - 1), 113*113);
    TS_ASSERT_EQUALS(MathUtil::prevSquare(1234*1234), 1234*1234);
    TS_ASSERT_EQUALS(MathUtil::prevSquare(1234*1234 - 1), 1233*1233);
    TS_ASSERT_EQUALS(MathUtil::prevSquare(1234*1234 + 1), 1234*1234);
  }

  void testPrevSquareBase()
  {
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(0), 0);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(1), 1);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(3), 1);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(4), 2);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(5), 2);

    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(8), 2);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(9), 3);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(10), 3);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(11), 3);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(12), 3);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(13), 3);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(14), 3);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(15), 3);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(16), 4);

    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(20), 4);

    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(114*114), 114);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(114*114 + 1), 114);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(114*114 - 1), 113);

    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(1234*1234), 1234);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(1234*1234 - 1), 1233);
    TS_ASSERT_EQUALS(MathUtil::prevSquareBase(1234*1234 + 1), 1234);
  }


  void testIsPow2()
  {
    TS_ASSERT(MathUtil::isPow2(1));
    TS_ASSERT(MathUtil::isPow2(2));
    TS_ASSERT(MathUtil::isPow2(4));
    TS_ASSERT(MathUtil::isPow2(8));
    TS_ASSERT(MathUtil::isPow2(16));
    TS_ASSERT(MathUtil::isPow2(32));
    TS_ASSERT(MathUtil::isPow2(64));
    TS_ASSERT(MathUtil::isPow2(32768));
    TS_ASSERT(MathUtil::isPow2(2097152));
    TS_ASSERT(MathUtil::isPow2(536870912));

    TS_ASSERT(!MathUtil::isPow2(0));
    TS_ASSERT(!MathUtil::isPow2(3));
    TS_ASSERT(!MathUtil::isPow2(9));
    TS_ASSERT(!MathUtil::isPow2(24));
    TS_ASSERT(!MathUtil::isPow2(2560));
    TS_ASSERT(!MathUtil::isPow2(1023));
    TS_ASSERT(!MathUtil::isPow2(18000));
  }

  void testIsSquare()
  {
    TS_ASSERT(MathUtil::isSquare(1));
    TS_ASSERT(!MathUtil::isSquare(2));
    TS_ASSERT(MathUtil::isSquare(4));
    TS_ASSERT(!MathUtil::isSquare(8));
    TS_ASSERT(MathUtil::isSquare(16));
    TS_ASSERT(!MathUtil::isSquare(32));
    TS_ASSERT(MathUtil::isSquare(64));
    TS_ASSERT(!MathUtil::isSquare(32768));
    TS_ASSERT(!MathUtil::isSquare(2097152));
    TS_ASSERT(!MathUtil::isSquare(536870912));

    TS_ASSERT(MathUtil::isSquare(207936));
    TS_ASSERT(MathUtil::isSquare(789*789));
    TS_ASSERT(!MathUtil::isSquare(789*789+1));
    TS_ASSERT(!MathUtil::isSquare(789*789-1));
  }


  void testNextPow2TexCoord()
  {
    const float delta = 0.000001f;
    TS_ASSERT_DELTA(MathUtil::nextPow2TexCoord(0.6f, 128), 0.6f, delta);
    TS_ASSERT_DELTA(MathUtil::nextPow2TexCoord(0.2f, 64), 0.2f, delta);

    // 1.0f * 192 / 256
    TS_ASSERT_DELTA(MathUtil::nextPow2TexCoord(1.0f, 192), 0.75f, delta);

    // 0.2 * (66 / 128)
    TS_ASSERT_DELTA(MathUtil::nextPow2TexCoord(0.2f, 66), 0.103125f, delta);

    // w/Pointf,Size
    Pointf res1 = MathUtil::nextPow2TexCoord(Pointf(0.6f, 0.2f),
                                             Size(128, 64));
    TS_ASSERT_DELTA(res1.x(), 0.6f, delta);
    TS_ASSERT_DELTA(res1.y(), 0.2f, delta);

    Pointf res2 = MathUtil::nextPow2TexCoord(Pointf(1.0f, 0.2f),
                                             Size(192, 66));
    TS_ASSERT_DELTA(res2.x(), 0.75f, delta);
    TS_ASSERT_DELTA(res2.y(), 0.103125f, delta);
  }


  void testIsRectangleOverlap()
  {
    Rectf rectA(0, 0, 1, 1);
    Rectf rectB(2, 2, 1, 1);
    TS_ASSERT(!MathUtil::isRectangleOverlap(rectA, rectB));

    rectB.x(1.001f);
    rectB.y(1.001f);
    TS_ASSERT(!MathUtil::isRectangleOverlap(rectA, rectB));

    rectB.x(0.999f);
    rectB.y(0.999f);
    TS_ASSERT(MathUtil::isRectangleOverlap(rectA, rectB));
  }

  void testMapU8Specials() {
    // [0, 1] -> [0, 255], using mapToU8
    TS_ASSERT_EQUALS(MathUtil::mapToU8(0.0f)     , 0u);
    TS_ASSERT_EQUALS(MathUtil::mapToU8(0.0001f)  , 0u);
    TS_ASSERT_EQUALS(MathUtil::mapToU8(1.0f)     , 255u);
    TS_ASSERT_EQUALS(MathUtil::mapToU8(0.99999f) , 255u);
    TS_ASSERT_EQUALS(MathUtil::mapToU8(0.5f)     , 127u);


    // [-1, 1] -> [0, 254], using mapToU8
    TS_ASSERT_EQUALS(MathUtil::mapToU8special(-1.0f)       , 0u);
    TS_ASSERT_EQUALS(MathUtil::mapToU8special(0.0f)        , 127u);
    TS_ASSERT_EQUALS(MathUtil::mapToU8special(1.0f)        , 254u);
    TS_ASSERT_EQUALS(254u - MathUtil::mapToU8special(0.0f) , 127u);
  }

private:
};

#endif
