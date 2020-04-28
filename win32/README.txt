Please do not compile amtk in a path with spaces to avoid potential
problems during the build and/or during the usage of the amtk
library.

Please refer to the following GNOME Live! page for more detailed
instructions on building amtk and its dependencies with Visual C++:

https://live.gnome.org/GTK%2B/Win32/MSVCCompilationOfGTKStack

This set of NMake Makefiles is intended to be used in a amtk source tree
unpacked from a tarball.  For building, you will need the following libraries
(headers, .lib's, DLLs, EXEs and scripts), with all of their dependencies:

-GLib 2.52.0 or later
-Gtk+-3.22.0 or later
-GObject-Introspection (optional, for building/using introspection files)


You will also need the following tools:
-Visual Studio 2008 or later, with C/C++ compilation support (MSVC).
-Python (optional, recommended, to generate the pkg-config files and
 build the introspection files; if building the introspection files, the
 Python installation must match the version, architecture and configuration
 of the Python installation that was used to build your copy of
 GObject-Introspection).
-For introspection builds, the pkg-config (or compatible) tool is also needed
 and the introspection files and pkg-config files for the dependent libraries
 (if applicable) are also needed.  You will need to set PKG_CONFIG_PATH
 if the pkg-config files cannot be found from the default locations that
 pkg-config will look for.

It is recommended that the dependent libraries are built with the same version
of Visual Studio that is being used to build amtk, as far as possible.

If building from a git checkout is desired, you will need to open the following
*.in files, and replace any items that are surrounded by the '@' characters,
and save those files without the .in file extension:

(open file)                           -> (save as file)
===========                              ==============
config-msvc.mak.in                    -> config-msvc.mak
config.h.win32.in                     -> config.h.win32

From this directory in a Visual Studio command prompt, run the following:

 nmake /f Makefile.vc CFG=<CFG> <target> <path_options> <other_options>

Where:
<CFG> is the build configuration, i.e. release or debug.  This is mandatory
for all targets.

<target> is as follows:
-(not specified): builds the amtk DLL and tests and sample programs.
                  If `INTROSPECTION=1` is specified, this will also build
                  the introspection files (.gir/.typelib) for amtk.
-all: see (not specified).
-clean: Removes all build files
-install: Same as (not specified) and also copies the built DLLs, .lib's, headers,
          tools and possibly introspection files to appropriate locations under
          $(PREFIX).  This will also create and copy the amtk-5.pc pkg-config
          file if Python can be found.

<path_options> is as follows:
-PREFIX: Root directory where built files will be copied to with the 'install' target.
         This also determines the root directory from which the dependent headers,
         .lib's and DLLs/.typelib's/.gir's are looked for, if INCLUDEDIR, LIBDIR and/or
         BINDIR are not respectively specified.  Default is
         $(srcroot)\..\vs<vs_short_ver>\<arch>, where vs_short_ver is 9 for Visual
         Studio 2008, 10 for VS2010, 11 for VS2012, 12 for VS2013 and 14 for VS 2015~2019.
-INCLUDEDIR: Base directory where headers are looked for, which is PREFIX\include by
             default.  Note that GLib headers are looked for in INCLUDEDIR\glib-2.0
             and LIBDIR\glib-2.0\include.
-LIBDIR: Base directory where .lib's and arch-dependent headers are looked for, which
         is PREFIX\lib by default.
-BINDIR: Base directory where dependent DLLs and tools (.exe's and scripts) are looked
         for, which is PREFIX\bin by default.  Note that for introspection builds,
         this means the introspection Python scripts and modules are in
         BINDIR\..\lib\gobject-introspection and the dependent introspection files are
         looked for in BINDIR\..\share\gir-1.0 and BINDIR\..\lib\girepository-1.0
         respectively for .gir files and .typelib files.
-PYTHON: Path to your Python interpreter executable, if not already in your %PATH% or
         using a different installation of Python is desired.  Please see note above
         on Python usage.  If Python cannot be found, you will not be able to build
         introspection files and the amtk-5.pc pkg-config file will not be
         generated using the 'install' build target.  It is also required for running
         recent versions of glib-mkenums, which is used during the build.
-PERL: Path to your PERL interpreter executable, if not already in your %PATH%
       It is also required for running older versions of glib-mkenums (when
       LEGACY_GLIB_MKENUMS is used), which is used during the build.
-PKG_CONFIG: Path to your pkg-config (or compatible) tool, if not already in your
             %PATH%.  This is required for introspection builds.
-LIBINTL_LIB: Full file name of your gettext-runtime library .lib file, if it is not
              intl.lib.  This should be in the directories indicated by %LIB% or in
              $(LIBDIR), or should be passed in with the full path.  Note that its
              DLL, if applicable, should be found in %PATH% or in $(BINDIR) as well,
              for building the introspection files or for creating the GDK-Pixbuf
              loaders cache file.

<other_options> is as follows, activate the options using <option>=1:
-INTROSPECTION: Build the introspection files.  Please see notes above.
-LEGACY_GLIB_MKENUMS: Use this option if using an older version of GLib which ships
                      with glib-mkenums that is written in PERL.  Please also see notes
                      on the PERL path option above
-LIBTOOL_DLL_NAME: Build the amtk DLL with libtool-style naming
