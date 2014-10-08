#ifndef UTIL_ASSERT_H
#define UTIL_ASSERT_H

#include <cassert>


/**
 * Header only replacement for assertion macros
 * Adds LCOV ignores to assertions, and provides functioning assertion for
 * android (todo)
 *
 * @author SwarmingLogic
 */
#define ASSERT(x) assert(x)

// TODO swarminglogic, 2014-05-28: add #ifdef for android, etc


#endif  // UTIL_ASSERT_H
