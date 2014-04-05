#include <cstdlib>
#include <iostream>

#if defined(__IPHONEOS__) || defined(__ANDROID__)
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <cpu-features.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#endif

#include <btBulletDynamicsCommon.h>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

void logSDLVersion(const std::string& what,
                   const SDL_version& compiled,
                   const SDL_version& linked,
                   std::string revision = std::string(""))
{
  if (revision.empty())
    SDL_Log("%s Version (Compiled) %d.%d.%d",
            what.c_str(),
            (int)compiled.major,
            (int)compiled.minor,
            (int)compiled.patch);
  else
    SDL_Log("%s Version (Compiled) %d.%d.%d (%s)",
            what.c_str(),
            (int)compiled.major,
            (int)compiled.minor,
            (int)compiled.patch,
            revision.c_str());

  SDL_Log("%s Version (Runtime) %d.%d.%d",
          what.c_str(),
          (int)linked.major,
          (int)linked.minor,
          (int)linked.patch);
}

#ifndef __ANDROID__
[[ noreturn ]]
#endif
void quit(int returnCode) {
  // SDL_image
  IMG_Quit();

  // SDL_mixer
  // const int nOpenAudio = Mix_QuerySpec(nullptr, nullptr, nullptr);
  const int nOpenAudio = Mix_QuerySpec(NULL, NULL, NULL);
  for (int i = 0 ; i < nOpenAudio ; ++i)
    Mix_CloseAudio();
  while(Mix_Init(0))
    Mix_Quit();

  // SDL
  SDL_Quit();
  exit(returnCode);
}

void bulletTest() {
  btBroadphaseInterface* broadphase = new btDbvtBroadphase();

  btDefaultCollisionConfiguration* collisionConfiguration =
    new btDefaultCollisionConfiguration();

  btCollisionDispatcher* dispatcher =
    new btCollisionDispatcher(collisionConfiguration);

  btSequentialImpulseConstraintSolver* solver =
    new btSequentialImpulseConstraintSolver;

  btDiscreteDynamicsWorld* dynamicsWorld =
    new btDiscreteDynamicsWorld(dispatcher,
                                broadphase,
                                solver,
                                collisionConfiguration);

  dynamicsWorld->setGravity(btVector3(0,-10,0));

  btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
  btCollisionShape* fallShape = new btSphereShape(1);
  btDefaultMotionState* groundMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
                                         btVector3(0,-1,0)));
  btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
  btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
  dynamicsWorld->addRigidBody(groundRigidBody);

  btDefaultMotionState* fallMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
                                         btVector3(0,50,0)));
  btScalar mass = 1;
  btVector3 fallInertia(0,0,0);
  fallShape->calculateLocalInertia(mass,fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,
                                                           fallMotionState,
                                                           fallShape,
                                                           fallInertia);
  btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
  dynamicsWorld->addRigidBody(fallRigidBody);

  for (int i=0 ; i<300 ; i++) {
    dynamicsWorld->stepSimulation(1/60.f,10);

    btTransform trans;
    fallRigidBody->getMotionState()->getWorldTransform(trans);

    SDL_Log("sphere height: %f", trans.getOrigin().getY());
  }

  dynamicsWorld->removeRigidBody(fallRigidBody);
  delete fallRigidBody->getMotionState();
  delete fallRigidBody;

  dynamicsWorld->removeRigidBody(groundRigidBody);
  delete groundRigidBody->getMotionState();
  delete groundRigidBody;

  delete fallShape;
  delete groundShape;

  delete dynamicsWorld;
  delete solver;
  delete collisionConfiguration;
  delete dispatcher;
  delete broadphase;
}


int main(int argc, char *argv[]) {
  (void)argc; (void)argv;

  SDL_Log("Initializing SDL");
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
  }

  // Write version information to log
  SDL_version compiled;
  SDL_version linked;
  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  logSDLVersion("SDL", compiled, linked, SDL_GetRevision());

  SDL_Log("Initializing SDL_image");
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  int imgFlagsInit = IMG_Init(imgFlags);
  if ((imgFlagsInit & imgFlags) != imgFlags)
    SDL_Log("Failed to initialize SDL_img: %s", IMG_GetError());
  SDL_IMAGE_VERSION(&compiled);
  logSDLVersion("SDL_image", compiled, *IMG_Linked_Version());

  SDL_Log("Initializing SDL_mixer");
  int mixFlags = MIX_INIT_OGG;
  int mixFlagsInit = Mix_Init(mixFlags);
  if ((mixFlagsInit & mixFlags) != mixFlags)
    SDL_Log("Failed to initialize SDL_mixer: %s", Mix_GetError());
  //   // throw log_.exception("Failed to initialize SDL_mixer", Mix_GetError);
  if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
    SDL_Log("Failed to aquire sound device: %s", Mix_GetError());

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
  SDL_Log("Music decoders (%i): %s", nMusicDecoders, ss.str().c_str());

  // Write audio decoder information to log
  const int nChunkDecoders = Mix_GetNumChunkDecoders();
  ss.str(std::string(""));
  if (nChunkDecoders > 0)
    ss << Mix_GetChunkDecoder(0);
  for (int i = 1 ; i < nChunkDecoders ; ++i) {
    ss << ", " << Mix_GetChunkDecoder(i);
  }
  SDL_Log("Audio decoders (%i): %s", nChunkDecoders, ss.str().c_str());


  // load sample.png into image
  SDL_Surface *image;
#ifndef __ANDROID__
  image=IMG_Load("assets/sample.png");
#else
  image=IMG_Load("sample.png");
#endif
  if(!image) {
    SDL_Log("IMG_Load: %s\n", IMG_GetError());
  } else {
    SDL_Log("Image loaded: %d x %i\n", image->w, image->h);
  }

  // Load sound chunk.
  Mix_Chunk* sound;
#ifndef __ANDROID__
  sound = Mix_LoadWAV("assets/audio.ogg");
#else
  sound = Mix_LoadWAV("audio.ogg");
#endif
  if (!sound)
    SDL_Log("Mix_LoadWav: %s", Mix_GetError);
  else
    Mix_PlayChannel( -1, sound, 0 );

#if defined(__ANDROID__)
  if (android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM) {
    SDL_Log("Device CPU is of ARM family");
  }
  SDL_Log("Device has %i cores", android_getCpuCount());
#endif

  bulletTest();
  std::int8_t test = 23;
  SDL_Log("My int8_t: %i", test);

  SDL_Delay(2000);

  quit(EXIT_SUCCESS);
  return 0;
}
