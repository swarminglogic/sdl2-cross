#ifndef MATH_TESTRECT_H
#define MATH_TESTRECT_H

#include <math/Rect.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Rect class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestRect : public CxxTest::TestSuite
{
 public:
  void testRect()
  {
    // == !=, constructors
    TS_ASSERT_EQUALS(Rect(2, 4, 6, 8), Rect(2, 4, 6, 8));
    TS_ASSERT_DIFFERS(Rect(2, 4, 6, 8), Rect(2, 6, 4, 8));
    TS_ASSERT_EQUALS(Rect(Point(2, 6), Size(4, 8)), Rect(2, 6, 4, 8));
    TS_ASSERT_EQUALS(Rect(Size(4, 8)), Rect(0, 0, 4, 8));

    // Copy
    Point pos(10, 20);
    Size size(30, 40);
    Rect rect(pos, size);
    Rect rect1 = rect;
    Rect rect2(rect);
    TS_ASSERT_EQUALS(rect, rect1);
    TS_ASSERT_EQUALS(rect, rect2);

    // Getters
    TS_ASSERT_EQUALS(rect.x(), pos.x());
    TS_ASSERT_EQUALS(rect.y(), pos.y());
    TS_ASSERT_EQUALS(rect.w(), size.w());
    TS_ASSERT_EQUALS(rect.h(), size.h());
    TS_ASSERT_EQUALS(rect.getPosition(), pos);
    TS_ASSERT_EQUALS(rect.getSize(), size);

    // Setters
    rect.x(24);
    rect.y(52);
    rect.w(104);
    rect.h(102);
    TS_ASSERT_EQUALS(rect.x(), 24);
    TS_ASSERT_EQUALS(rect.y(), 52);
    TS_ASSERT_EQUALS(rect.w(), 104);
    TS_ASSERT_EQUALS(rect.h(), 102);

    rect.setPosition(Point(-5, -9));
    rect.setSize(Size(101, 12));
    TS_ASSERT_EQUALS(rect.x(), -5);
    TS_ASSERT_EQUALS(rect.y(), -9);
    TS_ASSERT_EQUALS(rect.w(), 101);
    TS_ASSERT_EQUALS(rect.h(), 12);
  }


  void testGetData() {
    const Rect crect(2, 5, 7, 11);
    TS_ASSERT_EQUALS(crect.getData()[0], crect.x());
    TS_ASSERT_EQUALS(crect.getData()[1], crect.y());
    TS_ASSERT_EQUALS(crect.getData()[2], crect.w());
    TS_ASSERT_EQUALS(crect.getData()[3], crect.h());
    Rect rect(4, 12, 42, 55);
    TS_ASSERT_EQUALS(crect.getData()[0], crect.x());
    TS_ASSERT_EQUALS(crect.getData()[1], crect.y());
    TS_ASSERT_EQUALS(crect.getData()[2], crect.w());
    TS_ASSERT_EQUALS(crect.getData()[3], crect.h());
    rect.getData()[0] = 101;
    rect.getData()[1] = 102;
    rect.getData()[2] = 103;
    rect.getData()[3] = 104;
    TS_ASSERT_EQUALS(rect.x(), 101);
    TS_ASSERT_EQUALS(rect.y(), 102);
    TS_ASSERT_EQUALS(rect.w(), 103);
    TS_ASSERT_EQUALS(rect.h(), 104);
  }

 private:
};

#endif  // MATH_TESTRECT_H
