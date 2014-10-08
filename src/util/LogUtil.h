#ifndef UTIL_LOGUTIL_H
#define UTIL_LOGUTIL_H

#include <util/LogManager.h>
#include <util/SDL_Event_fwd.h>

class Log;

/**
 * LogUtil class.
 * A static class dedicated to logging various data, which is mostly used for
 * debugging.
 *
 * note: It is excluded from test coverage.
 *
 * @author SwarmingLogic
 */
class LogUtil
{
 public:
  /**
   * This is an extremely verbose SDL_Event output.
   *
   * It also abandons the regular concept of logging levels.
   *
   * Here DEBUG is used for the most verbose event types (e.g mouse movement),
   * INFO and WARNING used for common and rare events. ERROR is used for events
   * that are not thoroughly handled by this function.
   *
   * @param event  The event to handle.[
   */
  static void log(const SDL_Event& event);


 private:

  static void logFingerEvent          (Log& logger, const SDL_Event& event);
  static void logKeyEvent             (Log& logger, const SDL_Event& event);
  static void logMouseMotionEvent     (Log& logger, const SDL_Event& event);
  static void logMouseButtonEvent     (Log& logger, const SDL_Event& event);
  static void logMouseWheelEvent      (Log& logger, const SDL_Event& event);
  static void logMultiGestureEvent    (Log& logger, const SDL_Event& event);
  static void logTextInput            (Log& logger, const SDL_Event& event);
  static void logWindowEvent          (Log& logger, const SDL_Event& event);

  LogUtil();
  ~LogUtil();

  // NonCopyable
  LogUtil(const LogUtil& c);
  LogUtil& operator=(const LogUtil& c);
};

#endif  // UTIL_LOGUTIL_H
