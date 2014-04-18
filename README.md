![Swarming Logic cross-platform framework](/swl_logo.png)

SDL2, C++11, OpenGL 4, OpenGL ES 3, Android/Desktop Project.

A WIP project to work to do game development targetting Android/Linux,
with the intention to be easily extended to OSX, iOS, Win7.

### External Libraries:
The framework will rely on the following libraries

| Library    | Description |
| :--------- | :------------------------------------------------------ |
| SDL2       | OpenGL glue, touch/gesture and keyboard input, and more |
| SDL_image  | png/jpg loading                                         |
| SDL_mixer  | ogg loading, sound playback                             |
| SDL_ttf    | ttf loading                                             |
| bullet     | physics                                                 |
| GLM        | math library similar to GLSL (headers only)             |
| boost      | boost (headers only)                                    |


### Project Structure
```
├── android                         { Base android build files }
│   ├── assets -> ../assets (symlink)
│   ├── fixes                       { Various fixes for external libraries, etc. }
│   ├── jni
│   │   ├── bullet-android          { Android.mk for building bullet library }
│   │   ├── bullet-src -> [Bullet Source]    (symlink)
│   │   ├── SDL ->        [SDL Source]       (symlink)
│   │   ├── SDL_image ->  [SDL_image Source] (symlink)
│   │   ├── SDL_mixer ->  [SDL_mixer Source] (symlink)
│   │   ├── SDL_ttf ->    [SDL_ttf Source]   (symlink)
│   │   └── src -> ../../src (symlink)
│   ├── res                         { Android specific assets }
│   │   ├── drawable-hdpi
│   │   ├── drawable-ldpi
│   │   ├── drawable-mdpi
│   │   ├── drawable-xhdpi
│   │   ├── drawable-xxhdpi
│   │   ├── layout
│   │   └── values
│   └── src
│       └── org
│           └── libsdl
│               └── app             { JNI Java wrapper stuff }
├── assets                          { All resources used by the application }
└── src                             { All code source files used by the application, excl. shaders }
    ├── audio                       { linked w/SDL, SDL_mixer}
    ├── core                        { linked w/All }
    ├── extern                      { base library }
    ├── graphics                    { linked w/SDL, SDL_image, SDL_opengl, SDL_ttf, GL}
    ├── io                          { linked w/SDL }
    ├── math                        { base library }
    ├── model                       { linked w/SDL, bullet }
    └── util                        { base library }
```

### Dependancy tree

To enforce decoupling, and quicker build times, the source is structured into
modules, compiled as shared libraries. This currently only applies to the
desktop build, but will soon also be used for android.

```
    +––––––––––––––––––––––––––––––––––––––––––––––––––––––+
    |                  Common libraries                    |
    |                                                      |
    |    +––––––––––––+   +–––––––––––+   +––––––––––+     |
    |    |    math    |   |   util    |   |  extern  |     |
    |    |            |   |           |   |          |     |
    |    |            |   | SDL       |   |          |     |
    |    +––––––––––––+   +–––––––––––+   +––––––––––+     |
    |                                                      |
    +–––––––––––––––––––––––––+––––––––––––––––––––––––––––+
                              |
    +–––––––––––––––––––––––––+––––––––––––––––––––––––––––+
    |                                                      |
    |   +––––––––––––––––+   +–––––––––––––––––––––+       |
    |   |     audio      |   |     graphics        |       |
    |   |                |   |                     |       |
    |   | SDL, SDL_Mixer |   | SDL, SDL_image, GL  |       |
    |   +––––––––––––––––+   | SDL_ttf             |       |
    |                        +–––––––––––––––––––––+       |
    |   +––––––––––––––––+   +–––––––––+                   |
    |   |      model     |   |    io   |                   |
    |   |                |   |         |                   |
    |   | SDL, bullet    |   | SDL     |                   |
    |   +––––––––––––––––+   +–––––––––+                   |
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
| `./initialize_project.sh`          | Check all dependancies and sets up symbolic links         |
| `./compile.sh`                     | Utility for simplifying build commands                    |
| `./devtools.sh`                    | Another utility for simplifying development commands      |
| `./src/util/gitrev.h`              | Automatically generated file for accessing current git id |

* Linux/Android:

1. Run `. ./initialize_project.sh` and pay attention to output

   Note that it is `. ./initialize_project.sh` and not `./initialize_project.sh`

   It is recommended that you define the detected enviroment variable paths, so that this script isn't necessary.

1. `./devtools.sh ball` (builds android and linux)

See `./initialize_project.sh`, `./devtools.sh` and `./compile.sh` for details.

### Suggested `.gitignore` content
```
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
src/util/gitrev.h
```

### Troubleshooting

- **Trouble building SDL_mixer on android ("selected processor does not support Thumb mode ...")**
  There were various issues with compiling the newest verions of SDL_mixer for ARM.

  A patch that fixes these issues is kept in `./android/fixes/SDL_mixer.[HG-ID].android.fix.patch`

  Apply the patch from the SDL_mixer directory, using:

  `$ hg import --no-commit [PATH_TO_REPO]/android/fixes/SDL_mixer.[HG-ID].android.fix.patch`

  If the patch failes to be properly applied, or fails to correct the build issues, try
  checking out the SDL_mixer repo at the changeset ID matching [HD-ID] before applying the patch.

  To revert this patch, before updating the repo, use

  `$ hg update --clean`
