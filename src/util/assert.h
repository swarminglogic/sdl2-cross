#ifndef UTIL_ASSERT_H
#define UTIL_ASSERT_H

/**
 * Utility header for unifying assertion calls cross platform.
 * - On Android: __android_log_assert
 * - On Linux/Windows/etc: standard stl <cassert> header.
 *
 * @author SwarmingLogic
 */


#ifdef __ANDROID__
#include <android/log.h>
#  undef assert
#  ifdef NDEBUG
#    define assert(e) ((void)0)
#  else
#    define assert(e) ((e) ? ((void)0) : \
                       __android_log_assert(0, \
                                            "SWL", \
                                            "ASSERT-FAIL: %s(%s:%d) >> %s ", \
                                            __func__, __FILE__, __LINE__, #e))
#  endif
#else
#include <cassert>
#endif

#endif  // UTIL_ASSERT_H
