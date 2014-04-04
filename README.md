SDL2-cross (gamedev framework)
===========================================================

SDL2, C++11, OpenGL 4, OpenGL ES 3, Android/Desktop Project.

A WIP project to work to do game development targetting Android/Linux,
with the intention to be easily extended to OSX, iOS, Win7.

### External Libraries:
The framework will rely on the following libraries

| Library    | Description |
| :--------- | :------------------------------------------------------ |
| SDL2       | OpenGL glue, touch/gesture and keyboard input, and more |
| SDL_mixer  | sound                                                   |
| SDL_image  | png/jpg loading                                         |
| GLM        | math library similar to GLSL                            |
| bullet     | physics                                                 |


### Project Structure
```
├── android                         { Base android build files }
│   ├── assets -> ../assets (symlink)
│   ├── fixes                       { Various fixes for external libraries, etc. }
│   ├── jni
│   │   ├── SDL ->        [SDL Source]       (symlink)
│   │   ├── SDL_image ->  [SDL_image Source] (symlink)
│   │   ├── SDL_mixer ->  [SDL_mixer Source] (symlink)
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
```

### Compilation & Special Files
* Special files:

| File    | Description |
| :--------------------------------- | :--------------------------------------------------- |
| `./android/initialize_project.sh`  | Sets up symbolic links, pay attention to all errors  |
| `./build.sh`                       | Utility for simplifying build commands               |
| `./devtools.sh`                    | Another utility for simplifying development commands |

* Linux/Android:
See `./devtools.sh` and `./build.sh`.

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
android/jni/SDL_image
android/jni/SDL_mixer
android/jni/SDL
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
