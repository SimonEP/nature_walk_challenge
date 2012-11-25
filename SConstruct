VariantDir('build', 'src', duplicate=0)
env = Environment()
flags='-Wall -std=gnu++0x -g'
execFlags = flags+' -static'
libraries = ['NatureWalk.a', 'm', 'pthread', 'dl', 'Xext', 'opencv_core', 'opencv_imgproc', 'opencv_highgui', 'gtest', 'boost_program_options']
libpath = 'build/'
include = ['/usr/include/opencv2/', '/usr/include/boost/', '/usr/include/gtest/']


env.Library('build/NatureWalk', Glob('build/*.cpp'), CCFLAGS = flags, CPPPATH = include )
env.Program('build/path_detect', 'build/test/path_detect.cpp', CPPPATH=include, LIBS = libraries, LIBPATH = libpath, CCFLAGS = execFlags )

