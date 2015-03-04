#ifndef __CONFIG_H
#define __CONFIG_H

/** Platform macros */
#define UNIX
// suppress warnings with already define -DWIN32 macro
#ifndef WIN32
/* #undef WIN32 */
#endif
/* #undef APPLE */
/* #undef CYGWIN */
/* #undef IOS */
/* #undef IOSSIM */
/* #undef WITH_CPP11 */

// suppress warnings with already define -DANDROID macro
#ifndef ANDROID
/* #undef ANDROID */
#endif

#define PATH_SEPERATOR '/'

/** Implementation macros */
#define NET_ZEROMQ
#define NET_RTP
#define S11N_PROTOBUF
#define DISC_BONJOUR
#define DISC_BONJOUR_EMBED
/* #undef DISC_AVAHI */
#define DISC_BROADCAST
#ifndef THREAD_PTHREAD
#define THREAD_PTHREAD
#endif
#ifndef THREAD_WIN32
/* #undef THREAD_WIN32 */
#endif

/** Loglevels */
#define LOGLEVEL_COMMON 1
#define LOGLEVEL_DISC 1
#define LOGLEVEL_NET 1
#define LOGLEVEL_S11N 1

/** version */
#define UMUNDO_VERSION_MAJOR "0"
#define UMUNDO_VERSION_MINOR "5"
#define UMUNDO_VERSION_PATCH "0"
#define UMUNDO_VERSION "0.5.0"

/** build type */
#define CMAKE_BUILD_TYPE "Release"

/** miscellaneous */
#define PROJECT_SOURCE_DIR "#"

/* #undef DIST_PREPARE */
#define HAVE_STDBOOL_H 1

/* #undef BUILD_WITH_COMPRESSION_MINIZ */
#define BUILD_WITH_COMPRESSION_FASTLZ

/** Implementation specific */
#define NET_ZEROMQ_SND_HWM 300000
#define NET_ZEROMQ_RCV_HWM 300000

/* #undef BONJOUR_HAS_KDNSSERVICEERR_TIMEOUT */
#endif
