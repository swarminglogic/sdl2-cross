import os

Help("""
Type: 'scons [OPTIONS]' -> compiles program

OPTIONS:
      '--tests'         -> builds CxxTest unit tests
      '--win64'         -> cross-compiles program for win64 platform.
""")

env = Environment(ENV = {'PATH' : os.environ['PATH'],
                         'HOME' : os.environ['HOME'],
                         'TERM' : 'xterm'},
                  CXX='g++-4.9',
                  tools=['default'], toolpath=[''])

AddOption('--win64',
          action='store_true',
          help='cross-compiles for windows',
          default=False)

AddOption('--tests',
          action='store_true',
          help='compile and run unit tests',
          default=False)

# Build uMundo external library
SConscript('external/umundo/SConscript')

# Build flite external library
SConscript('external/flite/SConscript')

# Build libnoise external library
SConscript('external/noise/SConscript')

# Build libnoise external library
SConscript('external/polyvox/SConscript')

# Build angelscript external library
SConscript('external/angelscript/SConscript')


if GetOption('win64'):
    env.Tool('crossmingw', toolpath = ['./utils/scripts/'])
    vardir='build/win/src'
else:
    vardir='build/linux/src'

env.VariantDir(vardir, 'src')
Export('env')
SConscript(vardir + '/SConscript')

if GetOption('tests'):
    if not GetOption('win64'):
        SConscript(vardir + '/SConscript_tests')

