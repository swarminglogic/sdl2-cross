import os

Help("""
Type: 'scons'             -> build program
""")

#  TODO (swarminglogic, 2013-06-25): Add following targets
#      'scons app'         ->  -- " --
#      'scons release'     -> build program without debug information
#      'scons tests'       -> build all individual tests
#      'scons all'         -> [app] + [tests]
#      'scons check'       -> build and run all modified tests
#      'scons check-all    -> build and run all tests


env = Environment(ENV = {'PATH' : os.environ['PATH'],
                         'HOME' : os.environ['HOME'],
                         'TERM' : 'xterm'},
                  # CXX='clang',
                  CXX='g++',
                  tools=['default'], toolpath=[''])

# VariantDir('build', 'src')
# Export('env')
# SConscript('build/SConscript')
# SConscript('build/SConscript_tests')
