The original `PolyVox` project is hosted here:

  `http://www.volumesoffun.com/`

Compete source code was retrieved through

  `https://bitbucket.org/volumesoffun/polyvox.git`

No changes to original `PolyVox` source files have been made.

For the most part, only core library sources files have been retained.

`SCons` and `Android` based build files have been added.
These are files named `SConscript` and `Android.mk`, respectively.


```
  Files retained:
-----------------------------
    CHANGELOG.txt
    CREDITS.txt
    INSTALL.txt
    LICENSE.TXT
    README.rst
    library/PolyVoxCore/*
    library/PolyVoxUtil/*


  Files removed:
-----------------------------
    build/*
    documentation/*
    examples/*
    library/CMakeLists.txt
    library/Doxyfile.in
    library/Mainpage.dox
    library/PolyVoxConfig.cmake.in
    library/polyvox.qhcp.in
    library/bindings/*
    library/PolyVoxCore/CMakeLists.txt
    library/PolyVoxUtil/CMakeLists.txt
    tests/*
    CMakeLists.txt
    CTestConfig.cmake
    Packaging.cmake
    TODO.txt


  Filed added:
-----------------------------
    sdl2-cross-changes.md
    Android.mk
    SConscript
    library/Android.mk
    library/PolyVoxUtil/Android.mk
    library/PolyVoxUtil/SConscript
	  library/PolyVoxCore/Android.mk
	  library/PolyVoxCore/SConscript
```
