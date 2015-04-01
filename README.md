![Swarming Logic cross-platform framework](https://raw.githubusercontent.com/swarminglogic/sdl2-cross/master/doc/swl_logo.png)

SDL2, C++11, OpenGL 4.3, OpenGL ES 3, Android/Desktop Project.

A WIP project for game development currently supporting Android/Linux/Windows,
with the intention to be easily extended to OSX, iOS.

This isn't a game engine (at least not in its current state), but serves as a good basis for doing cross-platform development. It has support for cross-platform compilation of a large collection of libraries (see below). Development is done on linux, with all tools developed with that in mind.


### Implemented Features (summary):
* Project initialization scripts for detecting required dependencies. [screenshot](https://raw.githubusercontent.com/swarminglogic/sdl2-cross/master/doc/initialize_project.png)
* Utility for common development tasks (building android/linux, android logcat, unit testing, etc)
* `CxxTest` based unit-testing (linux, windows and android)
* Combined OpenGL/GLES files with `#ifdef ES`, `#else`, `#endif` directives.
* Build system that supports many external libraries (bullet, flite, libnoise, polyvox, umundo, angelscript)
* Script for generating html code coverage report.
* Windows 64-bit [cross-compilation](http://swarminglogic.com/article/2014_11_crosscompile2) using `mingw-w64-x86-64`
* Embedded mDNSResponder + ZMQ for automatic service detection and networking

### Future Features:
* Deferred rendering
* AngelScript based scripting language
* Add external library SDL_net
* `emscriptem` based HTML5 compilation


The framework relies on the following libraries

### External Libraries (not included):
| Library    | Version                | Description                                             |
| :--------- | :--------------------- | :------------------------------------------------------ |
| SDL2       | `2.0.4 (0495816fec54)` | OpenGL glue, touch/gesture and keyboard input, and more |
| SDL_image  | `2.0.0 (2b0ada991468)` | png/jpg loading                                         |
| SDL_mixer  | `2.0.0 (b28b41b93ba7)` | ogg loading, sound playback                             |
| SDL_ttf    | `2.0.12 (b773c1cd55a2)`| ttf loading                                             |
| bullet     | `2.82-r2704`           | physics                                                 |
| GLM        | `0.9.5.2`              | math library similar to GLSL (headers only)             |
| boost      | `1.55`                 | boost (headers only)                                    |

### External Libraries (included):
| Library    | Version                | Description                                             |
| :--------- | :--------------------- | :------------------------------------------------------ |
| flite      | `1.4`                  | text to speech synthesization engine                    |
| libnoise   | `1.0.0`                | noise generation library                                |
| polyvox    | `0.2.1`                | voxel library                                           |
| CxxTest    | `4.4`                  | unit test code generation utility                       |
| angelscrip | `2.29.2`               | scripting engine                                        |
| umundo     | `0.5.0`                | networking library that glues mDNSResponder + ZeroMQ    |

### External Libraries (script-download):
| Library       | Version                | Description                                             |
| :---------    | :--------------------- | :------------------------------------------------------ |
| mDNSResponder | `333.10`               | Apple's mDNS, DNS-SD implementation (aka Bonjour)       |
| ZeroMQ        | `4.1.0`                | Async network messaging library                         |
| re            | `0.4.7`                | SIP/SDP/RTCP/RTCP/DNS library                           |
| fastlz        | `0.1.0`                | Compression library                                     |


### Project Structure
```
├── android                    > Base android build files
│   ├── assets -> ../assets (symlink)
│   ├── jni
│   │   ├── bullet-android             { Android.mk for building bullet }
│   │   ├── bullet-src -> [Bullet Source]    (symlink)
│   │   ├── SDL ->        [SDL Source]       (symlink)
│   │   ├── SDL_image ->  [SDL_image Source] (symlink)
│   │   ├── SDL_mixer ->  [SDL_mixer Source] (symlink)
│   │   ├── SDL_ttf ->    [SDL_ttf Source]   (symlink)
│   │   └── src -> ../../src (symlink)
│   ├── res                        { Android specific assets }
│   └── src                        { JNI Java wrapper stuff }
├── assets                     { All resources used by the application }
│   ├── fonts
│   ├── images
│   ├── meshes
│   ├── music
│   ├── shaders
│   └── sounds
├── coveragehistory            { Unit test coverage history folder, used by gencoverage.sh}
├── external                   { External Libraries }
│   └── flite                      { flite, TTS synthesis engine }
│   └── noise                      { libnoise, noise generation library }
│   └── polyvox                    { polyvox, voxel library }
│   └── umundo                     { mDNS + ZMQ library }
└── src                        { All code source files used by the application, excl. shaders }
│   ├── audio                      { Audio and music playback }
│   ├── core                       { Base that uses all other parts }
│   ├── extern                     { External libraries that might be moved to ../external }
│   ├── graphics                   { Graphics and OpenGL related}
│   ├── io                         { Input/Output, keyboard, mouse, gamecontroller, }
│   ├── math                       { Basic math classes }
│   ├── model                      { Physics and world related }
│   └── util                       { Utility and helper classes }
└── utils                      { Misc utilities }
    ├── cxxtest                     { Unit testing library, downloaded by initialize_project.sh}
    ├── obj2cobj                    { Utility for compressing Wavefront OBJ files}
    ├── obj2info                    { Utility for printing Wabefront OBJ data summary}
    ├── patches                     { Patches used by initialize_project.sh}
    ├── scripts                     { Various minor helper scripts, some downloaded by initialize_project.sh}
    └── simpletextpreprocess        { Utility for preprocessing GL/GLES shader files}
```

### Dependency tree

To enforce decoupling, and quicker build times, the source is structured into
modules, compiled as shared libraries. This currently only applies to the
desktop build, but will soon also be used for android.

```
    +––––––––––––––––––––––––––––––––––––––––––––––––––––––+
    |                  Common libraries                    |
    |                                                      |
    |    +––––––––––––+      +––––––––––––+                |
    |    |    math    |      |   extern   |                |
    |    |            |      |            |                |
    |    |            |      |            |                |
    |    +–––––+––––––+      +––––––+–––––+                |
    |           \                  /                       |
    |            \______   _______/                        |
    |                   \ /                                |
    |             +––––––+––––––+                          |
    |             |    util     |                          |
    |             |             |                          |
    |             | SDL, noise  |                          |
    |             | angelscript |                          |
    |             +–––––––––––––+                          |
    |                                                      |
    |                                                      |
    +–––––––––––––––––––––––––+––––––––––––––––––––––––––––+
                              |
    +–––––––––––––––––––––––––+––––––––––––––––––––––––––––+
    |                                                      |
    |   +––––––––––––––––+   +–––––––––––––––––––––+       |
    |   |     audio      |   |     graphics        |       |
    |   |                |   |                     |       |
    |   | SDL, SDL_Mixer |   | SDL, SDL_image, GL  |       |
    |   | flite          |   | SDL_ttf             |       |
    |   +––––––––––––––––+   +–––––––––––––––––––––+       |
    |                                                      |
    |   +––––––––––––––––+   +–––––––––+   +–––––––––+     |
    |   |      model     |   |    io   |   |   net   |     |
    |   |                |   |         |   |         |     |
    |   | SDL, bullet    |   | SDL     |   | umundo  |     |
    |   | polyvox        |   +–––––––––+   +–––––––––+     |
    |   +––––––––––––––––+                                 |
    |                                                      |
    +–––––––––––––––––––––––––+––––––––––––––––––––––––––––+
                              |
    +–––––––––––––––––––––––––+––––––––––––––––––––––––––––+
    |                                                      |
    |             +–––––––––––––––––––––––+                |
    |             |         core          |                |
    |             |                       |                |
    |             |    all of the above   |                |
    |             +–––––––––––––––––––––––+                |
    |                                                      |
    +––––––––––––––––––––––––––––––––––––––––––––––––––––––+
```

### Special Files
* Special files:

| File    | Description |
| :--------------------------------- | :-------------------------------------------------------- |
| `./devtools.sh`                    | Utility for simplifying development commands              |
| `./src/util/gitrev.h`              | Automatically generated file for accessing current git id |

### Building & running
* Configuring dependencies and patching files, etc :
 Run `. ./devtools.sh init`
 Note that it is `. ./devtools.sh init` and not `./devtools.sh init`

 If everything passes, you are good to go. Otherwise, follow instructions.

* See `./devtools.sh help` for all development related commands.

### Suggested `.gitignore` content
```
common_build.pyc        
bin/
build/
lib/
android/bin/
android/gen/
android/libs/
android/obj/
android/project.properties
android/local.properties
android/jni/SDL_image
android/jni/SDL_mixer
android/jni/SDL_ttf
android/jni/SDL
android/jni/bullet-src
android/jni/flite
android/jni/noise
android/jni/polyvox
android/jni/angelscript
external/angelscript/add_on/
external/angelscript/include/
external/angelscript/source/
external/umundo/fastlz
external/umundo/mDNSResponder-333.10
external/umundo/re-0.4.7
external/umundo/zeromq-4.1.0
src/util/gitrev.h
html/
utils/cxxtest
utils/scripts/watchfile
utils/scripts/glslangValidator
coveragehistory/
src/main_android_tests.cpp
.coverage*
runtime.log
```


### Preferences/Settings File Format

The internal format for user preferences follows [`Boost.PropertyTree`'s INFO-format](http://www.boost.org/doc/libs/1_55_0/doc/html/boost_propertytree/parsers.html#boost_propertytree.parsers.info_parser).

This is a more versatile format than the typical `INI` format.

For `emacs` users, a major mode can be [found here](https://gist.github.com/swarminglogic/be3cd49fcba5ef891727)

