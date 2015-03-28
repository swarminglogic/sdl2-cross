#ifndef UTIL_TESTWORDGENERATORFACTORY_H
#define UTIL_TESTWORDGENERATORFACTORY_H

#include <memory>
#include <util/WordGenerator.h>
#include <util/WordGeneratorFactory.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the WordGeneratorFactory class.
 *
 * @author SwarmingLogic
 */
class TestWordGeneratorFactory : public CxxTest::TestSuite
{
 public:
  // FRIENDLY
  void testEmptyCreate()
  {
    auto wg = WordGeneratorFactory::create(3, 2);
    auto wgc = dynamic_cast<WordGenerator<3>*>(wg.get());
    TS_ASSERT(wgc);
    TS_ASSERT_EQUALS(wgc->N_init_, 2);
    auto wgcBad = dynamic_cast<WordGenerator<4>*>(wg.get());
    TS_ASSERT(!wgcBad);

    auto wg2 = WordGeneratorFactory::create(3, 0);
    auto wgc2 = dynamic_cast<WordGenerator<3>*>(wg2.get());
    TS_ASSERT(wgc2);
    TS_ASSERT_EQUALS(wgc2->N_init_, 3);

    auto wg3 = WordGeneratorFactory::create(
        WordGeneratorFactory::MAX_ORDER, 5);
    auto wgc3 = dynamic_cast<
      WordGenerator<WordGeneratorFactory::MAX_ORDER>*>(wg3.get());
    TS_ASSERT(wgc3);
    TS_ASSERT_EQUALS(wgc3->N_init_, 5);

    auto wg4 = WordGeneratorFactory::create(-1, 4);
    TS_ASSERT(!wg4);

    auto wg5 =
        WordGeneratorFactory::create(WordGeneratorFactory::MAX_ORDER + 1, 0);
    TS_ASSERT(!wg5);

    auto wg6 =
        WordGeneratorFactory::create(WordGeneratorFactory::MAX_ORDER - 1, 0);
    TS_ASSERT(wg6);
    auto wgc6 = dynamic_cast<
      WordGenerator<WordGeneratorFactory::MAX_ORDER - 1>*>(wg6.get());
    TS_ASSERT(wgc6);
    TS_ASSERT_EQUALS(wgc6->N_init_, WordGeneratorFactory::MAX_ORDER - 1);
  }
};

#endif  // UTIL_TESTWORDGENERATORFACTORY_H
