#include <core/MainManager.h>

#include <sstream>

#include <audio/AudioPlayback.h>
#include <audio/AudioResourceManager.h>
#include <audio/FliteUtil.h>
#include <audio/SDL_mixer.h>
#include <audio/flite.h>
#include <graphics/GlState.h>
#include <graphics/GraphicsManager.h>
#include <graphics/ImageResourceManager.h>
#include <graphics/Renderer2dImage.h>
#include <graphics/Renderer2dText.h>
#include <graphics/SDL_image.h>
#include <graphics/SDL_opengl.h>
#include <graphics/SDL_ttf.h>
#include <math/Random.h>
#include <util/Assert.h>
#include <util/CharMap.h>
#include <util/Log.h>
#include <util/LogUtil.h>
#include <util/SDL.h>
#include <util/TextBoxText.h>
#include <util/Timer.h>


MainManager::MainManager()
: log_("MainManager"),
  preferences_(FileInfo("preferences_default.info",
                        FileInfo::TYPE_ASSET),
               FileInfo(
#ifdef __ANDROID__
                   "preferences_android.info",
#else
                   "preferences_linux.info",
#endif
                   FileInfo::TYPE_ASSET),
               FileInfo("preferences_user.info",
                        FileInfo::TYPE_WRITABLE)),
  audioResources_(new AudioResourceManager),
  graphics_(nullptr),
  imageRenderer_(nullptr),
  imageResources_(new ImageResourceManager),
  runtime_(new Timer),
  isRunning_(true),
  testImage_(nullptr),
  fpsCounter_(30)
{
  initLogger();
  initRandom();
  initSDL();
  initSDLimg();
  initSDLttf();
  initSDLmixer();
  initFlite();
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
  graphics_.reset(new GraphicsManager(preferences_));

  testImage_ = imageResources_->loadImage("uv_colorgrid.png");
  testImage_->setIsMaxFiltering(true);
  assert(testImage_);
  testImage_->prepareForGl();

  imageRenderer_.reset(new Renderer2dImage);
  imageRenderer_->initialize();
  imageRenderer_->setSurface(testImage_);
  imageRenderer_->handleResize(graphics_->getScreenSize().w(),
                               graphics_->getScreenSize().h());

  TextBoxText tbt;
  tbt.setHeight(12u);
  tbt.setWidthFixed(5u);
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
  SurfaceShPtr charmapImage = imageResources_->loadImage("gnsh-bitmapfont.png");
  assert(charmapImage);
  charmapImage->setIsMaxFiltering(false);
  charmapImage->prepareForGl();

  textRenderer_.reset(new Renderer2dText);
  textRenderer_->setTextDataObjects(tbt, cm, charmapImage);
  textRenderer_->setZoomFactor(3);
  textRenderer_->setPosition(Point(40, 40));

  textRenderer_->handleResize(graphics_->getScreenSize().w(),
                              graphics_->getScreenSize().h());
  textRenderer_->postConfigureInitialize();
  textRenderer_->setText("FPS: 60");

  sound_ = audioResources_->loadSound("audio.ogg");
  voice_.reset(new SoundChunk(FliteUtil::textToSpeech(R"(Test Voice)")));

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
  uint64_t frameNumber = 0u;

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

    imageRenderer_->update(runtime_->getSeconds());
    textRenderer_->update(runtime_->getSeconds());

    currentTimeDelta_ = runtime_->getSeconds() - previousTime;
    previousTime = runtime_->getSeconds();

    GlState::clearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    textRenderer_->render(runtime_->getSeconds());
    // imageRenderer_->render(Point(0, 0), 0);
    graphics_->swapBuffers();
    ++frameNumber;

    // SDL_Delay(40);

    fpsCounter_.tic();
    if (frameNumber % 30 == 0) {
       std::stringstream ss;
       ss.precision(2);
       ss << std::fixed <<  "FPS: " << fpsCounter_.getFps();
       textRenderer_->setText(ss.str());
    }
  }
}


void MainManager::handleEvent(const SDL_Event& event)
{
#ifdef LOG_SDL_EVENTS_VERBOSELY
  // LogUtil::log(event);
#endif

  if (event.type == SDL_WINDOWEVENT) {
    if (event.window.event == SDL_WINDOWEVENT_RESIZED ||
        event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
      const int width  = event.window.data1;
      const int height = event.window.data2;
      log_.i() << "Window resized to " << width << " x " << height << Log::end;
      graphics_->setScreenSize(Size(width, height));
      textRenderer_->handleResize(width, height);
    }
  } else if (event.type == SDL_MOUSEBUTTONDOWN) {
    // TODO swarminglogic, 2014-04-21: Remove sound test
    const int gWidth = graphics_->getScreenSize().w();
    float mSpan = static_cast<float>(event.button.x - gWidth/2) /
        static_cast<float>(gWidth);
    // AudioPlayback::sound().play(sound_, mSpan, 0.7f);
    AudioPlayback::sound().play(voice_, mSpan, 0.7f);
  } else if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ||
        event.key.keysym.scancode == SDL_SCANCODE_AC_BACK) {
      isRunning_ = false;
    }
  } else if (event.type == SDL_QUIT) {
    isRunning_ = false;
  }
}


void MainManager::initRandom()
{
  log_.i("Initializing Randomizer");
  Random::init();
  std::stringstream ss;
  Random::seed().param(std::ostream_iterator<int>(ss, " "));
  log_.d() << "Randomizer seeded with: " << ss.str() << Log::end;
}

namespace {
LogManager::LogLevel stringToLogLevel(const std::string& txt) {
  if (txt == "DEBUG")
    return LogManager::LEVEL_DEBUG;
  else if (txt == "INFO")
    return LogManager::LEVEL_INFO;
  else if (txt == "WARN")
    return LogManager::LEVEL_WARNING;
  else if (txt == "ERROR")
    return LogManager::LEVEL_ERROR;
  else if (txt == "NONE")
    return LogManager::LEVEL_NONE;
  else
    throw Exception("Bad log level in preference file: " + txt);
}
}

void MainManager::initLogger()
{
#ifndef __ANDROID__
  LogManager::instance().setStreamTarget(std::cout);
#endif
#ifdef __WIN32__
  LogManager::instance().setStreamColorMode(LogManager::COLORMODE_NONE);
#endif

  // Setting logger levels
  std::string streamLevelTxt = "";
  std::string fileLevelTxt = "";
  try {
    streamLevelTxt = preferences_.get<std::string>("Logger.stream_level");
    fileLevelTxt = preferences_.get<std::string>("Logger.file_level");
    LogManager::LogLevel streamLevel =
        ::stringToLogLevel(streamLevelTxt);
    LogManager::LogLevel fileLevel =
        ::stringToLogLevel(fileLevelTxt);
    LogManager::instance().setFileLogLevel(fileLevel);
    LogManager::instance().setStreamLogLevel(streamLevel);
  }
  catch (const Exception& e) {
    LogManager::instance().setStreamLogLevel(LogManager::LEVEL_DEBUG);
    LogManager::instance().logColumnHeaders();
    throw e;
  }
  // Setting file log filename
  const std::string filelogFilename =
      preferences_.get("Logger.file_filename", std::string(""));
  LogManager::instance().setLogfileName(filelogFilename);

  // Outputing log columns headers
  LogManager::instance().logColumnHeaders();

  // Logging the specified log level
  log_.i() << "LogLevel (file):   " << fileLevelTxt  << Log::end;
  log_.i() << "LogLevel (stream): " << streamLevelTxt  << Log::end;
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
  if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1)
    throw log_.exception("Failed to acquire sound device", Mix_GetError);
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
    ss << ", " << Mix_GetMusicDecoder(i);
  }
  log_.d() << "Music decoders (" << nMusicDecoders << "): "
           << ss.str() << Log::end;


  // Write audio decoder information to log
  const int nChunkDecoders =  Mix_GetNumChunkDecoders();
  ss.str(std::string(""));
  if (nChunkDecoders > 0)
    ss << Mix_GetChunkDecoder(0);
  for (int i = 1 ; i < nChunkDecoders ; ++i) {
    ss << ", " << Mix_GetChunkDecoder(i);
  }
  log_.d() << "Audio decoders (" << nChunkDecoders << "): "
           << ss.str() << Log::end;

  (void)Mix_VolumeMusic(MIX_MAX_VOLUME);

  (void)Mix_Volume(-1, MIX_MAX_VOLUME);
  log_.d() << "Music Volume level: "
           << 100.0f * static_cast<float>(Mix_VolumeMusic(-1)) / MIX_MAX_VOLUME
           << "%" << Log::end;

  // TODO swarminglogic, 2014-02-08: Move to audio configuration setting.
  // Setting 64 channels to be played simulatenously
  Mix_AllocateChannels(64);
}


void MainManager::initFlite()
{
  log_.i("Initializing flite TTS");
  log_.d() << "flite Version (Compiled): "
           << FLITE_PROJECT_VERSION << Log::end;

  flite_init();
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
     << static_cast<int>(compiled.major) << "."
     << static_cast<int>(compiled.minor) << "."
     << static_cast<int>(compiled.patch);
  if (!revision.empty())
    ss << " (" << revision << ")";
  log_.d(ss.str());

  log_.d() << what << " Version (Runtime):  "
           << static_cast<int>(linked.major) << "."
           << static_cast<int>(linked.minor) << "."
           << static_cast<int>(linked.patch)
           << Log::end;
}
