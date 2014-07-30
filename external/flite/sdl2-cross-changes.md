The license for flite requires that modification must be clearly marked.

The original project is hosted here:

  `http://www.festvox.org/flite/`

Compete source code was retrieved through

  `http://www.festvox.org/flite/packed/flite-1.4/flite-1.4-release.tar.bz2`

No changes to original flite source files have been made.

Many files have been stripped away, and only th1e following have been retained:
```
  include/*
  lang/*
  src/*
  ACKNOWLEDGEMENTS
  COPYING
  README
```

Two files generated from removed tools have been added:
```
  include/flite_version.h
  main/flite_voice_list.c
```

The tool to generate `flite_voice_list.c` has been moved from `tools/` to `main/`

Aditionally, all Makefiles and Visual Studio related files are removed.
