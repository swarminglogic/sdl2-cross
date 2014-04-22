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

VariantDir('build', 'src')
Export('env')

SConscript('build/SConscript')
SConscript('build/SConscript_tests')
