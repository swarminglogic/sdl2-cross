#ifndef MATH_TESTRECTF_H
#define MATH_TESTRECTF_H

#include <math/Rectf.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Rectf class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestRectf : public CxxTest::TestSuite
{
public:
  void testRectf()
  {
    // == !=, constructors
    TS_ASSERT_EQUALS(Rectf(2, 4, 6, 8), Rectf(2, 4, 6, 8));
    TS_ASSERT_DIFFERS(Rectf(2, 4, 6, 8), Rectf(2, 6, 4, 8));
    TS_ASSERT_EQUALS(Rectf(Pointf(2, 6), Sizef(4,8)), Rectf(2, 6, 4, 8));
    TS_ASSERT_EQUALS(Rectf(Sizef(4,8)), Rectf(0, 0, 4, 8));

    TS_ASSERT_EQUALS(Rectf(2.4f, 4, 6.1f, 8), Rectf(2.4f, 4.0f, 6.1f, 8));
    TS_ASSERT_DIFFERS(Rectf(2.5f, 4, 6.6f, 8.3f), Rectf(2, 6, 4, 8));
    TS_ASSERT_EQUALS(Rectf(Pointf(2.0f, 6.2f), Sizef(4,8)),
                     Rectf(2.0f, 6.2f, 4, 8));
    TS_ASSERT_EQUALS(Rectf(Sizef(4,8)), Rectf(0.0f, 0.0f, 4.0f, 8.0f));

    // Copy
    Pointf pos(10.5f, 20.7f);
    Sizef size(30.9f, 40.9f);
    Rectf rect(pos, size);
    Rectf rect1 = rect;
    Rectf rect2(rect);
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
    rect.x(24.5f);
    rect.y(52);
    rect.w(104.112f);
    rect.h(102);
    TS_ASSERT_EQUALS(rect.x(), 24.5f);
    TS_ASSERT_EQUALS(rect.y(), 52.0f);
    TS_ASSERT_EQUALS(rect.w(), 104.112f);
    TS_ASSERT_EQUALS(rect.h(), 102);

    rect.setPosition(Pointf(-5.2f, -9));
    rect.setSize(Sizef(101.94f, 12));
    TS_ASSERT_EQUALS(rect.x(), -5.2f);
    TS_ASSERT_EQUALS(rect.y(), -9);
    TS_ASSERT_EQUALS(rect.w(), 101.94f);
    TS_ASSERT_EQUALS(rect.h(), 12);
  }


  void testGetData() {
    const Rectf crect(2.5f, 5.4f, 7.4f, 11.4f);
    TS_ASSERT_EQUALS(crect.getData()[0], crect.x());
    TS_ASSERT_EQUALS(crect.getData()[1], crect.y());
    TS_ASSERT_EQUALS(crect.getData()[2], crect.w());
    TS_ASSERT_EQUALS(crect.getData()[3], crect.h());

    Rectf rect(4, 12, 42, 55);
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

#endif
