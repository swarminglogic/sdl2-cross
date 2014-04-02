Android/Desktop SDL2-based C++11 Project
=========================================

A WIP project to work to do game development targetting Android/Linux,
with the intention to be easily extended to OSX, iOS, Win7.

The framework will rely on the following libraries
SDL2      (OpenGL glue, touch/gesture and keyboard input, and more)
SDL_mixer (sound)
SDL_image (png/jpg loading)
GLM       (math library similar to GLSL)
bullet    (physics)


Troubleshooting
==========================================

Trouble building SDL_mixer ("selected processor does not support Thumb mode ...")
-----------------------------------------------------------------------------------
There were various issues with compiling the newest verions of SDL_mixer for ARM.
A patch that fixes these issues is kept in ./android/fixes/SDL_mixer.[HG-ID].android.fix.patch

Apply the patch from the SDL_mixer directory, using:
$ hg import --no-commit [PATH_TO_REPO]/android/fixes/SDL_mixer.[HG-ID].android.fix.patch

If the patch failes to be properly applied, or fails to correct the build issues, try
checking out the SDL_mixer repo at the changeset ID matching [HD-ID] before applying the patch.

To revert this patch, before updating the repo, use
$ hg update --clean
