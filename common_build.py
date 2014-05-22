import os
# Common variables for SCons script

base ='#/src'

pathBoost   = os.environ["BOOST_DIR"];
pathSdl     = os.environ["SDL2_DIR"];
pathGlm     = os.environ["GLM_DIR"];
pathBullet  = os.environ["BULLET_DIR"];
pathCxxTest = os.environ['CXXTEST_DIR'];

lSDL      = ['SDL2']
lSDLimage = ['SDL2_image']
lSDLmixer = ['SDL2_mixer']
lSDLttf   = ['SDL2_ttf']
lBullet   = ['BulletCollision', 'BulletDynamics', 'LinearMath']
lGL       = ['GL', 'GLU']
lOther    = ['m', 'dl', 'rt']
lAll  = lSDL + lSDLimage + lSDLmixer + lSDLttf + lBullet + lGL + lOther

libpaths = [pathSdl    + '/lib',
            pathBullet + '/lib']

# Macros definitions
macroDefinitions = [
   'USING_SDL',            # Should always be set in SDL projects.
#   'LOGGING_DISABLED',    # Disables all logging
#   'LOG2STREAM_DISABLED', # Disables logging to stream
#   'LOG2FILE_DISABLED',   # Disables logging to file
   'LOG_SDL_EVENTS_VERBOSELY'
]

gccWarningLevel = [
   '-Wall', '-Wextra', '-Wcast-align', '-Wcast-qual',
   '-fpermissive',
   '-Wconversion', '-Wdisabled-optimization', #'-Weffc++',
   '-Wfloat-equal', '-Wformat=2', '-Wimport', '-Winit-self',
   '-Winline', '-Winvalid-pch', '-Wlong-long',
   '-Wmissing-format-attribute', '-Wmissing-include-dirs',
   '-Wmissing-noreturn', '-Wpacked', '-Wpointer-arith',
   '-Wredundant-decls', '-Wshadow', '-Wstack-protector',
   '-Wstrict-aliasing=2', '-Wunreachable-code',
   '-Wunsafe-loop-optimizations', '-Wunused',
   '-Wvariadic-macros', '-Wwrite-strings', '-pedantic',
   '-pedantic-errors', '-Woverloaded-virtual',
   '-Wswitch-enum', '-Werror'
]

# CPPFLAGS
####################
cppflags = ['-O0', '-g', '-gdwarf-2']
#cppflags = ['-O3']
cppflags.extend(['-fno-strict-aliasing',
                 '-std=c++11',
                 '-D_REENTRANT',
                 '-DGLM_FORCE_RADIANS'])
cppflags.extend(['-isystem', pathBoost  + '/include',
                 '-isystem', pathSdl    + '/include/',
                 '-isystem', pathGlm,
                 '-isystem', pathBullet + '/include/bullet'])
cppflags.extend(gccWarningLevel)
for macro in macroDefinitions:
   cppflags.append('-D' + macro)

sourcepaths = [
   base,
   pathBoost  + '/include',
   pathGlm,
   pathBullet + '/include/bullet/',
   pathSdl    + "/include/"]
