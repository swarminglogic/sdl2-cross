#include <util/LogUtil.h>

#include <cassert>

#include <util/Log.h>
#include <util/SDL.h>

// LCOV_EXCL_START
void LogUtil::log(const SDL_Event& event)
{
  Log log("LogUtil");

  switch (event.type) {
  case SDL_DOLLARGESTURE:
    log.e() << "SDL_DOLLARGESTURE" << Log::end;
    break;
  case SDL_DROPFILE:
    log.e() << "SDL_DROPFILE" << Log::end;
    break;
  case SDL_FINGERMOTION:
  case SDL_FINGERDOWN:
  case SDL_FINGERUP:
    log.d() << "SDL_FINGER" << Log::end;
    logFingerEvent(log, event);
    break;
  case SDL_KEYDOWN:
  case SDL_KEYUP:
    log.d() << "SDL_KEY" << Log::end;
    logKeyEvent(log, event);
    break;
  case SDL_JOYAXISMOTION:
    log.e() << "SDL_JOYAXISMOTION" << Log::end;
    break;
  case SDL_JOYBALLMOTION:
    log.e() << "SDL_JOYBALLMOTION" << Log::end;
    break;
  case SDL_JOYHATMOTION:
    log.e() << "SDL_JOYHATMOTION" << Log::end;
    break;
  case SDL_JOYBUTTONUP:
  case SDL_JOYBUTTONDOWN:
    log.e() << "SDL_JOYBUTTON" << Log::end;
    break;
  case SDL_MOUSEMOTION:
    log.d() << "SDL_MOUSEMOTION" << Log::end;
    logMouseMotionEvent(log, event);
    break;
  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEBUTTONDOWN:
    log.i() << "SDL_MOUSEBUTTON" << Log::end;
    logMouseButtonEvent(log, event);
    break;
  case SDL_MOUSEWHEEL:
    log.i() << "SDL_MOUSEWHEEL" << Log::end;
    logMouseWheelEvent(log, event);
    break;
  case SDL_MULTIGESTURE:
    log.w() << "SDL_MULTIGESTURE" << Log::end;
    logMultiGestureEvent(log, event);
    break;
  case SDL_QUIT:
    log.w() << "SDL_QUIT" << Log::end;
    break;
  case SDL_SYSWMEVENT:
    log.e() << "SDL_SYSWMEVENT" << Log::end;
    break;
  case SDL_TEXTEDITING:
    log.e() << "SDL_TEXTEDITING" << Log::end;
    break;
  case SDL_TEXTINPUT:
    log.w() << "SDL_TEXTINPUT" << Log::end;
    logTextInput(log, event);
    break;
  case SDL_USEREVENT:
    log.e() << "SDL_USEREVENT" << Log::end;
    break;
  case SDL_WINDOWEVENT:
    log.w() << "SDL_WINDOWEVENT" << Log::end;
    logWindowEvent(log, event);
    break;
  default:
    break;
  }
}

void LogUtil::logFingerEvent(Log& logger,
                             const SDL_Event& event)
{
  assert(event.type == SDL_FINGERMOTION ||
         event.type == SDL_FINGERDOWN   ||
         event.type == SDL_FINGERUP);
  if (event.type == SDL_FINGERMOTION)
    logger << " └─ SDL_FINGERMOTION" << Log::end;
  else if (event.type == SDL_FINGERDOWN)
    logger << " └─ SDL_FINGERDOWN" << Log::end;
  else if (event.type == SDL_FINGERUP)
    logger << " └─ SDL_FINGERUP" << Log::end;

  logger << "Device-" << (int)event.tfinger.touchId
         << ", finger " << (int)event.tfinger.fingerId
         << ", at ("
         << event.tfinger.x << ", "
         << event.tfinger.y << ")  velocity ("
         << event.tfinger.dx << ", "
         << event.tfinger.dy << ") pressure: "
         << event.tfinger.pressure
         << Log::end;
}


void LogUtil::logKeyEvent(Log& logger,
                          const SDL_Event& event)
{
  assert(event.type == SDL_KEYDOWN ||
         event.type == SDL_KEYUP);
  if (event.type == SDL_KEYDOWN)
    logger << " └─ SDL_KEYDOWN" << Log::end;
  else if (event.type == SDL_KEYUP)
    logger << " └─ SDL_KEYUP" << Log::end;

  assert(event.key.state == SDL_PRESSED || event.key.state == SDL_RELEASED);
  logger << "Key [" << SDL_GetScancodeName(event.key.keysym.scancode)
         << ", #" << (uint)event.key.keysym.scancode << "] "
         << ((event.key.state == SDL_PRESSED)? "pressed" : "released")
         << ", mapping ["  << SDL_GetKeyName(event.key.keysym.sym)
         << ", #" << (uint)event.key.keysym.sym << "] "
         << (event.key.repeat ? " [repeat]" : "");
  if (event.key.keysym.mod | KMOD_NONE) {
    logger << " modifiers: [";
    if (event.key.keysym.mod & KMOD_LSHIFT)
      logger << " LSHIFT";
    if (event.key.keysym.mod & KMOD_RSHIFT)
      logger << " RSHIFT";
    if (event.key.keysym.mod & KMOD_LCTRL)
      logger << " LCTRL";
    if (event.key.keysym.mod & KMOD_RCTRL)
      logger << " RCTRL";
    if (event.key.keysym.mod & KMOD_LALT)
      logger << " LALT";
    if (event.key.keysym.mod & KMOD_RALT)
      logger << " RALT";
    if (event.key.keysym.mod & KMOD_LGUI)
      logger << " LGUI";
    if (event.key.keysym.mod & KMOD_RGUI)
      logger << " RGUI";
    if (event.key.keysym.mod & KMOD_NUM)
      logger << " NUM";
    if (event.key.keysym.mod & KMOD_CAPS)
      logger << " CAPS";
    logger << " ]";
  }
  logger << Log::end;
}


void LogUtil::logMouseMotionEvent(Log& logger,
                                  const SDL_Event& event)
{
  assert(event.type == SDL_MOUSEMOTION);
  logger << "Mouse-" << event.motion.which << " moved at ("
         << (int)event.motion.x << ", " << (int)event.motion.y
         << ")   relative ("
         << (int)event.motion.xrel << ", " << (int)event.motion.yrel
         << ")   state [";
  if (event.motion.state & SDL_BUTTON_LMASK)
    logger << " L";
  if (event.motion.state & SDL_BUTTON_MMASK)
    logger << " M";
  if (event.motion.state & SDL_BUTTON_RMASK)
    logger << " R";
  if (event.motion.state & SDL_BUTTON_X1MASK)
    logger << " X1";
  if (event.motion.state & SDL_BUTTON_X2MASK)
    logger << " X2";
  logger << " ]" << Log::end;
}


void LogUtil::logMouseButtonEvent(Log& logger,
                                  const SDL_Event& event)
{
  assert(event.type == SDL_MOUSEBUTTONDOWN ||
         event.type == SDL_MOUSEBUTTONUP);
  if (event.type == SDL_MOUSEBUTTONDOWN)
    logger << " └─ SDL_MOUSEBUTTONDOWN" << Log::end;
  else if (event.type == SDL_MOUSEBUTTONUP)
    logger << " └─ SDL_MOUSEBUTTONUP" << Log::end;

  std::string button;
  if (event.button.button == SDL_BUTTON_LEFT)
    button = "LEFT-button   ";
  else if (event.button.button == SDL_BUTTON_MIDDLE)
    button = "MIDDLE-button ";
  else if (event.button.button == SDL_BUTTON_RIGHT)
    button = "RIGHT-button  ";
  else if (event.button.button == SDL_BUTTON_X1)
    button = "X1-button     ";
  else if (event.button.button == SDL_BUTTON_X2)
    button = "X2-button     ";
  else
    button = "UNKNOWN";

  logger << "Mouse-" << event.button.which << " "
         << button
         << (event.button.state == SDL_PRESSED ? "pressed  " : "released ")
         << " at ("
         << (int)event.button.x << ", " << (int)event.button.y
         << ")  n-clicks: " << (int)event.button.clicks
         << Log::end;
}


void LogUtil::logMouseWheelEvent(Log& logger,
                                 const SDL_Event& event)
{
  assert(event.type == SDL_MOUSEWHEEL);
  logger << "Mouse-" << event.wheel.which << " wheel  ("
         << event.wheel.x << ", " << event.wheel.y << ")"
         << Log::end;
}


void LogUtil::logMultiGestureEvent(Log& logger,
                                   const SDL_Event& event)
{
  assert(event.type == SDL_MULTIGESTURE);
  logger << "Device-" << (int)event.mgesture.touchId
         << " " << event.mgesture.numFingers << " fingers   at ("
         << event.mgesture.x << ", " << event.mgesture.y << ")"
         << " dTheta (" << event.mgesture.dTheta << ")"
         << " dDist (" << event.mgesture.dDist << ")"
         << Log::end;
  (void)logger; (void)event;
}


void LogUtil::logTextInput(Log& logger,
                           const SDL_Event& event)
{
  assert(event.type == SDL_TEXTINPUT);
  logger << "Text input char " << event.text.text << Log::end;
}


void LogUtil::logWindowEvent(Log& logger,
                             const SDL_Event& event)
{
  assert(event.type == SDL_WINDOWEVENT);
  switch (event.window.event) {
  case SDL_WINDOWEVENT_SHOWN:
    logger << " └─ SDL_WINDOWEVENT_SHOWN" << Log::end;
    logger << "Window " << (int)event.window.windowID << " shown" << Log::end;
    break;
  case SDL_WINDOWEVENT_HIDDEN:
    logger << " └─ SDL_WINDOWEVENT_HIDDEN" << Log::end;
    logger << "Window " << (int)event.window.windowID << " hidden" << Log::end;
    break;
  case SDL_WINDOWEVENT_EXPOSED:
    logger << " └─ SDL_WINDOWEVENT_EXPOSED" << Log::end;
    logger << "Window " << (int)event.window.windowID << " exposed" << Log::end;
    break;
  case SDL_WINDOWEVENT_MOVED:
    logger << " └─ SDL_WINDOWEVENT_MOVED" << Log::end;
    logger << "Window " << (int)event.window.windowID << " moved to "
           << event.window.data1 << ", " << event.window.data2 << Log::end;
    break;
  case SDL_WINDOWEVENT_RESIZED:
    logger << " └─ SDL_WINDOWEVENT_RESIZED" << Log::end;
    logger << "Window " << (int)event.window.windowID << " resized to "
           << event.window.data1 << "x" << event.window.data2 << Log::end;
    break;
  case SDL_WINDOWEVENT_SIZE_CHANGED:
    logger << " └─ SDL_WINDOWEVENT_SIZE_CHANGED" << Log::end;
    logger << "Window " << (int)event.window.windowID << " changed size to "
           << event.window.data1 << "x" << event.window.data2 << Log::end;
  case SDL_WINDOWEVENT_MINIMIZED:
    logger << " └─ SDL_WINDOWEVENT_MINIMIZED" << Log::end;
    logger << "Window " << (int)event.window.windowID
           << " minimized" << Log::end;
    break;
  case SDL_WINDOWEVENT_MAXIMIZED:
    logger << " └─ SDL_WINDOWEVENT_MAXIMIZED" << Log::end;
    logger << "Window " << (int)event.window.windowID
           << " maximized" << Log::end;
    break;
  case SDL_WINDOWEVENT_RESTORED:
    logger << " └─ SDL_WINDOWEVENT_RESTORED" << Log::end;
    logger << "Window " << (int)event.window.windowID
           << " restored" << Log::end;
    break;
  case SDL_WINDOWEVENT_ENTER:
    logger << " └─ SDL_WINDOWEVENT_ENTER" << Log::end;
    logger << "Window " << (int)event.window.windowID
           << " gained mouse focus" << Log::end;
    break;
  case SDL_WINDOWEVENT_LEAVE:
    logger << " └─ SDL_WINDOWEVENT_LEAVE" << Log::end;
    logger << "Window " << (int)event.window.windowID
           << " lost mouse focus" << Log::end;
    break;
  case SDL_WINDOWEVENT_FOCUS_GAINED:
    logger << " └─ SDL_WINDOWEVENT_FOCUS_GAINED" << Log::end;
    logger << "Window " << (int)event.window.windowID
           << " gained keyboard focus" << Log::end;
    break;
  case SDL_WINDOWEVENT_FOCUS_LOST:
    logger << " └─ SDL_WINDOWEVENT_FOCUS_LOST" << Log::end;
    logger << "Window " << (int)event.window.windowID
           << " lost keyboard focus" << Log::end;
    break;
  case SDL_WINDOWEVENT_CLOSE:
    logger << " └─ SDL_WINDOWEVENT_CLOSE" << Log::end;
    logger << "Window " << (int)event.window.windowID << " closed" << Log::end;
    break;
  default:
    logger << "Window " << (int)event.window.windowID
           << " got an unknown event: " << event.window.event << Log::end;
    break;
  }
}

LogUtil::~LogUtil()
{
}
// LCOV_EXCL_STOP
