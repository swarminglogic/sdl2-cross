import os

Help("""
Type: 'scons [OPTIONS]' -> compiles program

OPTIONS:
      '--release'       -> no debug, optimized compile
      '--tests'         -> builds CxxTest unit tests
      '--win64'         -> cross-compiles program for win64 platform.
""")

env = Environment(ENV = {'PATH' : os.environ['PATH'],
                         'HOME' : os.environ['HOME'],
                         'TERM' : 'xterm'},
                  CXX='g++-4.9',
                  tools=['default'], toolpath=[''])

AddOption('--release',
          action='store_true',
          help='no debug, optimized compile',
          default=False)

AddOption('--win64',
          action='store_true',
          help='cross-compiles for windows',
          default=False)

AddOption('--tests',
          action='store_true',
          help='compile and run unit tests',
          default=False)


if GetOption('release'):
    env['cc_optflags'] = ['-O2', '-DNDEBUG']
    build_type = 'release'
else:
    env['cc_optflags'] = ['-O0', '-g', '-gdwarf-2']
    build_type = 'debug'

if GetOption('win64'):
    env.Tool('crossmingw', toolpath = ['./utils/scripts/'])
    os_target = 'win'
else:
    os_target = 'linux'

build_type_path = os_target + '/'+ build_type
env['base_vardir'] = '#build/' + build_type_path
env['base_exec_dir'] = '#/bin/' + build_type_path
env['base_lib_dir'] = '#/lib/' + build_type_path

Export('env')

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

vardir = env['base_vardir'] + '/src'
env.VariantDir(vardir, 'src')
Export('env')
SConscript(vardir + '/SConscript')

if GetOption('tests'):
    if not GetOption('win64'):
        SConscript(vardir + '/SConscript_tests')

