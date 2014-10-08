#ifndef UTIL_TESTCHARMAP_H
#define UTIL_TESTCHARMAP_H

#include <math/Point.h>
#include <math/Size.h>
#include <util/CharMap.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the CharMap class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestCharMap : public CxxTest::TestSuite
{
 public:
  CharMap init()
  {
    CharMap cm(Size(5, 12));
    std::vector<CharMap::Trait> traits = {
      CharMap::C_GOLDEN,
      CharMap::C_CYAN,
      CharMap::C_RED,
      CharMap::C_MAGENTA,
      CharMap::C_GREY,
      CharMap::C_WHITE,
      CharMap::C_GREEN,
      CharMap::C_BLUE,
      CharMap::C_OLIVE };
    cm.setTraits(traits);
    TS_ASSERT_EQUALS(cm.getNTraits(), traits.size());
    TS_ASSERT_EQUALS(cm.getCharSize(), Size(5, 12));
    return cm;
  }

  void commonTest(CharMap& cm)
  {
    Point tb = Point(5 * 20, 12 * 5);
    TS_ASSERT_EQUALS(tb, cm.getCharOffset(CharMap::C_MAGENTA, ' '));
    TS_ASSERT_EQUALS(Point(tb.x(), 0) , cm.getCharOffset(CharMap::C_RED, ' '));
    TS_ASSERT_EQUALS(2*Point(tb.x(), 0),
                     cm.getCharOffset(CharMap::C_GREEN, ' '));

    // Default trait fallback
    TS_ASSERT_EQUALS(Point(0, 0), cm.getCharOffset(CharMap::NO_TRAIT, ' '));
    cm.setDefaultTrait(CharMap::C_RED);
    TS_ASSERT_EQUALS(Point(tb.x(), 0) ,
                     cm.getCharOffset(CharMap::NO_TRAIT, ' '));
    cm.setDefaultTrait(CharMap::C_GREEN);
    Point tb2(2 * tb.x(), 0);
    TS_ASSERT_EQUALS(tb2.x(), cm.getCharOffset(CharMap::NO_TRAIT, ' ').x());
    TS_ASSERT_EQUALS(tb2.y(), cm.getCharOffset(CharMap::NO_TRAIT, ' ').y());
  }

  void testBasic()
  {
    CharMap cm;
    TS_ASSERT_EQUALS(cm.getCharSize(), Size(0, 0));
    cm.setCharSize(Size(5, 12));
    TS_ASSERT_EQUALS(cm.getCharSize(), Size(5, 12));

    CharMap cm2 = cm;

    std::vector<CharMap::Trait> traits;
    traits.push_back(CharMap::C_GOLDEN);
    cm2.setTraits(traits);
    cm.addTrait(CharMap::C_GOLDEN);

    TS_ASSERT_EQUALS(cm.getNTraits(), cm.getTraits().size());
    TS_ASSERT_EQUALS(1, cm.getTraits().size());
    TS_ASSERT_EQUALS(cm2.getNTraits(), cm2.getTraits().size());
    TS_ASSERT_EQUALS(cm.getTraits(), cm2.getTraits());

    cm.addTrait(CharMap::C_CYAN);
    TS_ASSERT_DIFFERS(cm.getTraits(), cm2.getTraits());
    traits.push_back(CharMap::C_CYAN);
    cm2.setTraits(traits);
    TS_ASSERT_EQUALS(cm.getTraits(), cm2.getTraits());
  }

  void testCharMap()
  {
    CharMap cm = init();
    commonTest(cm);
  }

  void testCopyConstructor()
  {
    CharMap cm(init());
    commonTest(cm);
  }

  void testStatics()
  {
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(0),  Point(0, 0));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(1),  Point(0, 1));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(2),  Point(1, 0));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(3),  Point(1, 1));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(4),  Point(0, 2));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(5),  Point(1, 2));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(6),  Point(2, 0));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(7),  Point(2, 1));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(8),  Point(2, 2));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(9),  Point(0, 3));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(10), Point(1, 3));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(11), Point(2, 3));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(12), Point(3, 0));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(13), Point(3, 1));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(14), Point(3, 2));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(15), Point(3, 3));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(16), Point(0, 4));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(17), Point(1, 4));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(18), Point(2, 4));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(19), Point(3, 4));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(20), Point(4, 0));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(21), Point(4, 1));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(22), Point(4, 2));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(23), Point(4, 3));
    TS_ASSERT_EQUALS(CharMap::convertIndexToTBoxPosition(24), Point(4, 4));

    // Test char to tbox index.
    // | !"#$%&'()*+,-./0123|
    // |456789:;<=>?@ABCDEFG|
    // |HIJKLMNOPQRSTUVWXYZ[|
    // |\]^_`abcdefghijklmno|
    // |pqrstuvwxyz{|}~     |

    TS_ASSERT_EQUALS(CharMap::convertCharToTBoxIndex(' '), Point(0, 0));
    TS_ASSERT_EQUALS(CharMap::convertCharToTBoxIndex('c'), Point(7, 3));
    TS_ASSERT_EQUALS(CharMap::convertCharToTBoxIndex('{'), Point(11, 4));
  }

  void testCharIndexOffest()
  {
    // 0  C_GOLDEN
    // 1  C_CYAN
    // 2  C_RED
    // 3  C_MAGENTA
    // 4  C_GREY
    // 5  C_WHITE
    // 6  C_GREEN
    // 7  C_BLUE
    // 8  C_OLIVE
    //
    //    [ 0 ] [ 2 ] [ 6 ] [ 12] [ 20]
    //    [ 1 ] [ 3 ] [ 7 ] [ 13] [ 21]
    //    [ 4 ] [ 5 ] [ 8 ] [ 14] [ 22]
    //    [ 9 ] [ 10] [ 11] [ 15] [ 23]
    //    [ 16] [ 17] [ 18] [ 19] [ 24]

    CharMap cm = init();
    TS_ASSERT_EQUALS(cm.getCharIndexOffset(CharMap::C_GOLDEN, 'c'),
                     Point(7, 3));
    TS_ASSERT_EQUALS(cm.getCharIndexOffset(CharMap::C_CYAN, 'c'),
                     Point(7, 3 + 5));
    TS_ASSERT_EQUALS(cm.getCharIndexOffset(CharMap::C_RED, 'c'),
                     Point(7 + 20, 3));
    TS_ASSERT_EQUALS(cm.getCharIndexOffset(CharMap::C_MAGENTA, 'c'),
                     Point(7 + 20, 3 + 5));
    TS_ASSERT_EQUALS(cm.getCharIndexOffset(CharMap::C_GREY, 'c'),
                     Point(7, 3 + 10));
    TS_ASSERT_EQUALS(cm.getCharIndexOffset(CharMap::C_BLUE, 'c'),
                     Point(7 + 40, 3 + 5));
  }


 private:
};

#endif  // UTIL_TESTCHARMAP_H
