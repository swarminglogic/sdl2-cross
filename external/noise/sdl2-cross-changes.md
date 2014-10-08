The license for libnoise requires that modification be clearly marked.

The original project is hosted here:

  `http://libnoise.sourceforge.net/`

Compete source code was retrieved through

  `http://prdownloads.sourceforge.net/libnoise/libnoisesrc-1.0.0.zip`

No changes to original libnoise source files have been made.

Many files have been stripped away, and only the following have been retained:
```
     COPYING.txt (moved from parent directory)
     INSTALL
     include/*
     src/* (except src/Sources, src/win32/*)
```

Header files have been moved to the /include directory

`SCons` and `Android` based build files have been added.
These are files named `SConscript` and `Android.mk`, respectively.


```
List of new files:
---------------------
Android.mk
SConscript
sdl2-cross-changes.md
src/Android.mk
src/SConscript
```