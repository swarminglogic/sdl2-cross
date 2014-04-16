#include <graphics/GraphicsManager.h>

#include <cassert>

#include <graphics/SDL_opengl.h>
#include <graphics/ViewConfig.h>


#ifdef __ANDROID__
#define USE_OPENGLES
#endif

GraphicsManager::GraphicsManager()
  : log_("GraphicsManager"),
    window_(nullptr),
    screenSize_(0, 0),
    context_(nullptr),
    isFullScreen_(false),
    isVSync_(true),
    isOpenGlDebugEnabled_(true)
{

  // TODO swarminglogic, 2014-04-16: Move this out somehow.
  // Either to MainManager, and/or configuration file.
  ViewConfig viewConfig(Size(1196, 768), "SDL Game");
  viewConfig.setIsFullScreen(false);
  viewConfig.setIsResizeable(false);
  viewConfig.setIsVSync(true);

  isFullScreen_ = viewConfig.isFullScreen();
  initalizeOpenGL(viewConfig);
}


GraphicsManager::~GraphicsManager()
{
}


void GraphicsManager::swapBuffers()
{
  SDL_GL_SwapWindow(window_.get());
}


void GraphicsManager::initalizeOpenGL(const ViewConfig& viewConfig)
{
  #ifndef USE_OPENGLES
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_CORE);
  #else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_ES);
  #endif
  if (isOpenGlDebugEnabled_)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

  int sdlWindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
  if (viewConfig.isFullScreen())
    sdlWindowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  if (viewConfig.isResizeable())
    sdlWindowFlags |= SDL_WINDOW_RESIZABLE;

  // Create SDL Window
  window_.reset(SDL_CreateWindow(viewConfig.getWindowTitle().c_str(),
                                 500, 200,
                                 viewConfig.getScreenWidth(),
                                 viewConfig.getScreenHeight(),
                                 sdlWindowFlags));
  if (!window_)
    throw log_.exception("Failed to initialize OpenGL", SDL_GetError);

  // Store window dimensions
  setScreenSize(viewConfig.getScreenSize());


  // Create OpenGL Context
  log_.i("Creating OpenGL Context");

  // Lists opengl versions intended to try
  const std::pair<int, int> glVersions[11]
     {{4,4}, {4,3}, {4,2}, {4,1}, {4,0},
      {3,3}, {3,2}, {3,1}, {3,0},
      {2,1}, {2,0}
    };

  SDL_GLContext context = nullptr;
  for (auto& glVersion : glVersions) {
    log_.d() << "Trying to create GL " << glVersion.first << "."
             << glVersion.second << " context" << Log::end;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glVersion.first);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glVersion.second);
    context = SDL_GL_CreateContext(window_.get());
    if (context != nullptr)
      break;
  }
  if (context == nullptr)
    throw log_.exception("Failed to create OpenGL Context", SDL_GetError);
  context_.reset(new SDL_GLContext(context));
  SDL_ClearError();

  // Set VSync
  if(SDL_GL_SetSwapInterval(static_cast<int>(viewConfig.isVSync())) < 0)
    log_.w() << "Failed to change vsync mode: " << SDL_GetError() << Log::end;

  // // Make it the current context
  SDL_GL_MakeCurrent(window_.get(), *context_);

  logStaticOpenGLInfo();
  logGraphicsDriverInfo();
  logOpenGLContextInfo();
}


void GraphicsManager::logStaticOpenGLInfo() const
{
  #ifdef GL_GLEXT_VERSION
  log_.d() << "OpenGL GLEXT version: " << GL_GLEXT_VERSION << Log::end;
  #endif
}


void GraphicsManager::logOpenGLContextInfo() const
{
  assert(context_ && "Missing OpenGL Context");
  std::stringstream ss;
  ss << std::setw(20) << std::left << "OpenGL Version: "
     << glGetString(GL_VERSION);
  log_.i(ss.str()); ss.str("");
  ss << std::setw(20) << std::left << "OpenGL GLSL: "
     << glGetString(GL_SHADING_LANGUAGE_VERSION);
  log_.i(ss.str()); ss.str("");
  ss << std::setw(20) << std::left << "OpenGL Renderer: "
     << glGetString(GL_RENDERER);
  log_.i(ss.str()); ss.str("");
  ss << std::setw(20) << std::left << "OpenGL Vendor: "
     << glGetString(GL_VENDOR);
  log_.i(ss.str()); ss.str("");

  // supported extensions:
  GLint nExtensions;
  glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
  ss << std::setw(20) << std::left << "OpenGL #EXT: "
     << nExtensions;
  log_.d(ss.str()); ss.str("");

  #ifdef DEBUG_OPENGL_EXT
  for (GLint i = 0 ; i < nExtensions ; ++i) {
    ss << glGetStringi(GL_EXTENSIONS, i);
    log_.d(ss.str()); ss.str("");
  }
  #endif
}


void GraphicsManager::logGraphicsDriverInfo() const
{
  assert(context_ && "Missing OpenGL Context");

  const int nVideoDrivers = SDL_GetNumVideoDrivers();
  log_.d() << "Found " << nVideoDrivers << " video drivers" << Log::end;

  const std::string currentVideoDriver(SDL_GetCurrentVideoDriver());
  for (int i = 0; i < nVideoDrivers; i++) {
    const std::string videoDriver(SDL_GetVideoDriver(i));
    log_.d() << "Video Driver #" << i << ": " << videoDriver;
    if (currentVideoDriver == videoDriver)
      log_ << " (Current)";
    log_ << Log::end;
  }

  const int nRenderDrivers = SDL_GetNumRenderDrivers();
  log_.d() << "Found " << nRenderDrivers << " render drivers" << Log::end;

  SDL_RendererInfo info;
  for (int i = 0 ; i < nRenderDrivers ; ++i) {
    SDL_GetRenderDriverInfo(i, &info);
    log_.d() << "Render Driver #" << i << ": " << info.name;

    bool isSoftware      = info.flags & SDL_RENDERER_SOFTWARE;
    bool isHardware      = info.flags & SDL_RENDERER_ACCELERATED;
    bool isVSyncEnabled  = info.flags & SDL_RENDERER_PRESENTVSYNC;
    bool isTargetTexture = info.flags & SDL_RENDERER_TARGETTEXTURE;

    log_ << "\t [";
    if (isSoftware) log_ << "SW";
    if (isHardware) log_ << "HW";
    if (isVSyncEnabled) log_ << ", VSync";
    if (isTargetTexture) log_ << ", TT";
    log_ << "]" << Log::end;
  }
}


Size GraphicsManager::getScreenSize() const
{return screenSize_;}
void GraphicsManager::setScreenSize(Size screenSize)
{screenSize_ = screenSize;}
