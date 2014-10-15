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

Additionaly, a `noiseutils` library has been merged into this libnoise library.
The files share the same license. The source files were obtained here:

  `http://libnoise.sourceforge.net/downloads/noiseutils.zip`

Minor modifications to noiseutils has been made.
The diff patch is provided in the bottom.

```
List of new files:
---------------------
Android.mk
SConscript
sdl2-cross-changes.md
src/Android.mk
src/SConscript
src/noiseutils.cpp    (from noiseutils.zip, see patch below)
include/noiseutils.h  (from noiseutils.zip, see patch below)
```

Patch of files from `noiseutils.zip`:

```
diff --git a/external/noise/include/noiseutils.h b/external/noise/include/noiseutils.h
index fe39dc1..04ae88c 100644
--- a/external/noise/include/noiseutils.h
+++ b/external/noise/include/noiseutils.h
@@ -27,9 +27,7 @@
 #include <string.h>
 #include <string>
 
-#include <noise.h>
-
-using namespace noise;
+#include "noise.h"
 
 namespace noise
 {
diff --git a/external/noise/src/noiseutils.cpp b/external/noise/src/noiseutils.cpp
index 6936dff..0690809 100644
--- a/external/noise/src/noiseutils.cpp
+++ b/external/noise/src/noiseutils.cpp
@@ -22,9 +22,8 @@
 
 #include <fstream>
 
-#include <noise/interp.h>
-#include <noise/mathconsts.h>
-
+#include "interp.h"
+#include "mathconsts.h"
 #include "noiseutils.h"
 
 using namespace noise;

```





