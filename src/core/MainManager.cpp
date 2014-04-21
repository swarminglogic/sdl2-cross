#include <core/MainManager.h>

#include <cassert>
#include <sstream>

#include <audio/AudioPlayback.h>
#include <audio/AudioResourceManager.h>
#include <audio/SDL_mixer.h>
#include <graphics/GraphicsManager.h>
#include <graphics/SDL_image.h>
#include <graphics/SDL_opengl.h>
#include <graphics/SDL_ttf.h>
#include <util/Log.h>
#include <util/LogUtil.h>
#include <util/SDL.h>
#include <util/Timer.h>


MainManager::MainManager()
  : log_("MainManager"),
    audioResources_(new AudioResourceManager),
    graphics_(nullptr),
    runtime_(new Timer),
    isRunning_(true)
{
  initSDL();
  initSDLimg();
  initSDLttf();
  initSDLmixer();
}

MainManager::~MainManager()
{
}


MainManager& MainManager::instance()
{
  static MainManager instance;
  return instance;
}


void MainManager::initialize()
{
  log_.i("Initializing resources.");
  graphics_.reset(new GraphicsManager);
  sound_ = audioResources_->loadSound("audio.ogg");
  runtime_->start();
}


void MainManager::finalize()
{
  log_.i("Cleaning up resources.");
}


Uint32 MainManager::getRuntimeTics() const
{
  return runtime_->getTicks();
}

float MainManager::getRuntimeSecs() const
{
  return runtime_->getSeconds();
}

float MainManager::getCurrentTimeDelta() const
{
  return currentTimeDelta_;
}


void MainManager::run() {
  SDL_Event event;

  float previousTime = runtime_->getSeconds();

  while (isRunning_) {
#ifndef NDEBUG
    const char* err = SDL_GetError();
    if (err[0] != '\0') {
      log_.e() << "SDL_GetError(): " << err << Log::end;
    }
#endif
    SDL_ClearError();

    while (SDL_PollEvent(&event)) {
      handleEvent(event);
    }

    currentTimeDelta_ = runtime_->getSeconds() - previousTime;
    previousTime = runtime_->getSeconds();
    graphics_->swapBuffers();
  }
}


void MainManager::handleEvent(const SDL_Event& event)
{
#ifdef LOG_SDL_EVENTS_VERBOSELY
  LogUtil::log(event);
#endif

  if (event.type == SDL_WINDOWEVENT) {
    if (event.window.event == SDL_WINDOWEVENT_RESIZED ||
        event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
      const int width  = event.window.data1;
      const int height = event.window.data2;
      log_.i() << "Window resized to " << width << " x " << height << Log::end;
      graphics_->setScreenSize(Size(width, height));
    }
  }
  else if (event.type == SDL_MOUSEBUTTONDOWN) {
    // TODO swarminglogic, 2014-04-21: Remove sound test
    const int gWidth = graphics_->getScreenSize().w();
    float mSpan = (float)(event.button.x - gWidth/2) / (float)gWidth;
    AudioPlayback::sound().play(sound_, mSpan, 0.7f);
  }
  else if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ||
        event.key.keysym.scancode == SDL_SCANCODE_AC_BACK) {
      isRunning_ = false;
    }
  }
  else if (event.type == SDL_QUIT) {
    isRunning_ = false;
  }
}



void MainManager::initSDL()
{
  log_.i("Initializing SDL");
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    throw log_.exception("Failed to initialize SDL", SDL_GetError);

  // Write version information to log
  SDL_version compiled;
  SDL_version linked;
  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  logSDLVersion("SDL", compiled, linked, SDL_GetRevision());
}


void MainManager::initSDLimg()
{
  log_.i("Initializing SDL_image");
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  int imgFlagsInit = IMG_Init(imgFlags);
  if ((imgFlagsInit & imgFlags) != imgFlags)
    throw log_.exception("Failed to initialize SDL_image", IMG_GetError);

  // Write version information to log
  SDL_version compiled;
  SDL_IMAGE_VERSION(&compiled);
  logSDLVersion("SDL_image", compiled, *IMG_Linked_Version());
}


void MainManager::initSDLttf()
{
  log_.i("Initializing SDL_ttf");
  if ( TTF_Init() != 0)
    throw log_.exception("Failed to initialize SDL_ttf", SDL_GetError);

  // Write version information to log
  SDL_version compiled;
  SDL_TTF_VERSION(&compiled);
  logSDLVersion("SDL_ttf", compiled, *TTF_Linked_Version());
}


void MainManager::initSDLmixer()
{
  log_.i("Initializing SDL_mixer");
  int mixFlags = MIX_INIT_OGG;
  int mixFlagsInit = Mix_Init(mixFlags);
  if ((mixFlagsInit & mixFlags) != mixFlags)
    throw log_.exception("Failed to initialize SDL_mixer", Mix_GetError);
  if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
    throw log_.exception("Failed to aquire sound device", Mix_GetError);
  atexit(Mix_CloseAudio);
  atexit(Mix_Quit);

  // Write version information to log
  SDL_version compiled;
  SDL_MIXER_VERSION(&compiled);
  logSDLVersion("SDL_mixer", compiled, *Mix_Linked_Version());

  // Write music decoder information to log
  const int nMusicDecoders = Mix_GetNumMusicDecoders();
  std::stringstream ss;
  if (nMusicDecoders > 0)
    ss << Mix_GetMusicDecoder(0);
  for (int i = 1 ; i < nMusicDecoders ; ++i) {
    ss << ", " << Mix_GetMusicDecoder(i) << Log::end;
  }
  log_.d() << "Music decoders (" << nMusicDecoders << "): "
           << ss.str() << Log::end;


  // Write audio decoder information to log
  const int nChunkDecoders =  Mix_GetNumChunkDecoders();
  ss.str(std::string(""));
  if (nChunkDecoders > 0)
    ss << Mix_GetChunkDecoder(0);
  for (int i = 1 ; i < nChunkDecoders ; ++i) {
    ss << ", " << Mix_GetChunkDecoder(i) << Log::end;
  }
  log_.d() << "Audio decoders (" << nChunkDecoders << "): "
           << ss.str() << Log::end;

  (void)Mix_VolumeMusic(MIX_MAX_VOLUME);

  (void)Mix_Volume(-1, MIX_MAX_VOLUME);
  log_.d() << "Music Volume level: "
           << 100.0f * (float)Mix_VolumeMusic(-1) / MIX_MAX_VOLUME << "%"
           << Log::end;

  // TODO swarminglogic, 2014-02-08: Move to audio configuration setting.
  // Setting 64 channels to be played simulatenously
  Mix_AllocateChannels(64);
}


void MainManager::logSDLVersion(const std::string& what,
                                const SDL_version& compiled,
                                const SDL_version& linked)
{
  logSDLVersion(what, compiled, linked, std::string(""));
}

void MainManager::logSDLVersion(const std::string& what,
                                const SDL_version& compiled,
                                const SDL_version& linked,
                                std::string revision)
{
  std::stringstream ss;
  ss << what << " Version (Compiled): "
     << (int)compiled.major << "."
     << (int)compiled.minor << "."
     << (int)compiled.patch;
  if (!revision.empty())
    ss << " (" << revision << ")";
  log_.d(ss.str());

  log_.d() << what << " Version (Runtime):  "
           << linked.major << "." << linked.minor << "." << linked.patch
           << Log::end;
}
