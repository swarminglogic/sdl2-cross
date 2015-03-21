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
  void testEmptyCreate()
  {
    auto wg = WordGeneratorFactory::create(3, 3);
    TS_ASSERT_EQUALS(wg->getN(), 3);
    TS_ASSERT_EQUALS(wg->getN_init(), 3);

    auto wg2 = WordGeneratorFactory::create(3, 0);
    TS_ASSERT_EQUALS(wg2->getN(), 3);
    TS_ASSERT_EQUALS(wg2->getN_init(), 3);

    auto wg3 = WordGeneratorFactory::create(
        WordGeneratorInterface::MAX_ORDER, 5);
    TS_ASSERT_EQUALS(wg3->getN(), WordGeneratorInterface::MAX_ORDER);
    TS_ASSERT_EQUALS(wg3->getN_init(), 5);
  }
};

#endif  // UTIL_TESTWORDGENERATORFACTORY_H
