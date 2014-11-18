![Swarming Logic cross-platform framework](https://raw.githubusercontent.com/swarminglogic/sdl2-cross/master/doc/swl_logo.png)

SDL2, C++11, OpenGL 4.3, OpenGL ES 3, Android/Desktop Project.

A WIP project to work to do game development targetting Android/Linux,
with the intention to be easily extended to OSX, iOS, Win7.

This isn't a game engine (at least not in its current state), but serves as a good basis for doing cross-platform development.

### Platform Features (summary):
* Project initialization script for detecting required dependencies. [screenshot](https://raw.githubusercontent.com/swarminglogic/sdl2-cross/master/doc/initialize_project.png)
* Utility for common development tasks (building android/linux, android logcat, unit testing, etc)
* `CxxTest` based unit-testing (both linux and android)
* Combined OpenGL/GLES files with `#ifdef ES`, `#else`, `#endif` directives.
* Several libraries with android build files (bullet, flite, libnoise, polyvox)
* Script for generating html code coverage report.
* Windows 64-bit [cross-compilation](http://swarminglogic.com/article/2014_11_crosscompile2) using `mingw-w64-x86-64`
* ...

### Future Features:
* Deferred rendering
* AngelScript based scripting language
* Add external library SDL_net
* `emscriptem` based HTML5 compilation


### External Libraries (not included):
The framework relies on the following libraries

| Library    | Version                | Description                                             |
| :--------- | :--------------------- | :------------------------------------------------------ |
| SDL2       | `2.0.4 (bc074eca6597)` | OpenGL glue, touch/gesture and keyboard input, and more |
| SDL_image  | `2.0.0 (e8573815cc1f)` | png/jpg loading                                         |
| SDL_mixer  | `2.0.0 (9599bb0ff844)` | ogg loading, sound playback                             |
| SDL_ttf    | `2.0.12 (f28f7b0f72ae)`| ttf loading                                             |
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
    |              +–––––+––––––+                          |
    |              |   util     |                          |
    |              |            |                          |
    |              | SDL, noise |                          |
    |              +––––––––––––+                          |
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
    |   +––––––––––––––––+   +–––––––––+                   |
    |   |      model     |   |    io   |                   |
    |   |                |   |         |                   |
    |   | SDL, bullet    |   | SDL     |                   |
    |   | polyvox        |   +–––––––––+                   |
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

### Compilation & Special Files
* Special files:

| File    | Description |
| :--------------------------------- | :-------------------------------------------------------- |
| `./initialize_project.sh`          | Check all dependencies and sets up symbolic links         |
| `./compile.sh`                     | Utility for simplifying build commands                    |
| `./devtools.sh`                    | Another utility for simplifying development commands      |
| `./validateshaders.sh`             | Utility for preprocessing and validating GL/GLES shaders  |
| `./src/util/gitrev.h`              | Automatically generated file for accessing current git id |

* Linux/Android:

1. Run `. ./initialize_project.sh` and pay attention to output

   Note that it is `. ./initialize_project.sh` and not `./initialize_project.sh`

   It is recommended that you define the detected enviroment variable paths, so that this script isn't a required step.

1. `./devtools.sh ball` (builds android and linux)

See `./initialize_project.sh`, `./devtools.sh` and `./compile.sh` for details.

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


### Flite license and changelog

In compliance with the `flite` license, modifications and changes are explained in

`./external/flite/sdl2-cross-changes.md`

