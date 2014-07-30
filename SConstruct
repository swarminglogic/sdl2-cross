import os

Help("""
Type: 'scons'             -> build program
""")

env = Environment(ENV = {'PATH' : os.environ['PATH'],
                         'HOME' : os.environ['HOME'],
                         'TERM' : 'xterm'},
                  # CXX='clang',
                  CXX='g++',
                  tools=['default'], toolpath=[''])
Export('env')
SConscript('external/flite/SConscript')


AddOption('--tests',
          action='store_true',
          help='compile and run unit tests',
          default=False)

VariantDir('build', 'src')
SConscript('build/SConscript')
if GetOption('tests'):
    SConscript('build/SConscript_tests')

