include $(top_srcdir)/aminclude_static.am
include $(srcdir)/amtk-sources.mak
clean-local: code-coverage-clean
distclean-local: code-coverage-dist-clean

AM_CPPFLAGS =				\
	-DDATADIR=\""$(datadir)"\"	\
	-DG_LOG_DOMAIN=\"Amtk\"		\
	-DAMTK_COMPILATION		\
	-I$(top_builddir)		\
	-I$(top_srcdir)			\
	$(WARN_CFLAGS)			\
	$(CODE_COVERAGE_CPPFLAGS)	\
	$(AMTK_DEP_CFLAGS)

# Helper Libtool library, so that the private functions can be used in unit
# tests.
noinst_LTLIBRARIES = libamtk-core.la

libamtk_core_la_SOURCES =	\
	$(amtk_public_c_files)	\
	$(amtk_public_headers)

nodist_libamtk_core_la_SOURCES =	\
	$(amtk_built_public_c_files)	\
	$(amtk_built_public_headers)

BUILT_SOURCES =				\
	$(amtk_built_public_c_files)	\
	$(amtk_built_public_headers)

libamtk_core_la_CFLAGS =	\
	$(CODE_COVERAGE_CFLAGS)

libamtk_core_la_LDFLAGS =	\
	-no-undefined		\
	$(WARN_LDFLAGS)

libamtk_core_la_LIBADD =	\
	$(CODE_COVERAGE_LIBS)

# The real library.
lib_LTLIBRARIES = libamtk-@AMTK_API_VERSION@.la

libamtk_@AMTK_API_VERSION@_la_SOURCES =

libamtk_@AMTK_API_VERSION@_la_LIBADD =	\
	libamtk-core.la			\
	$(AMTK_DEP_LIBS)

libamtk_@AMTK_API_VERSION@_la_LDFLAGS =		\
	-version-info $(AMTK_LT_VERSION)	\
	-no-undefined				\
	-export-symbols-regex "^amtk_.*"	\
	$(WARN_LDFLAGS)

libamtk_includedir = $(includedir)/amtk-@AMTK_API_VERSION@/amtk
libamtk_include_HEADERS = $(amtk_public_headers)
nodist_libamtk_include_HEADERS = $(amtk_built_public_headers)

ENUM_TYPES = $(amtk_public_headers)

amtk-enum-types.h: amtk-enum-types.h.template $(ENUM_TYPES) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template amtk-enum-types.h.template $(ENUM_TYPES)) > $@

amtk-enum-types.c: amtk-enum-types.c.template $(ENUM_TYPES) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template amtk-enum-types.c.template $(ENUM_TYPES)) > $@

EXTRA_DIST =				\
	amtk-enum-types.c.template	\
	amtk-enum-types.h.template	\
	NOTES

CLEANFILES = $(BUILT_SOURCES)

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS = Amtk-@AMTK_API_VERSION@.gir

INTROSPECTION_SCANNER_ENV = CC="$(CC)"

Amtk-@AMTK_API_VERSION@.gir: libamtk-@AMTK_API_VERSION@.la

INTROSPECTION_SCANNER_ARGS =		\
	-I$(top_srcdir)			\
	-I$(top_builddir)		\
	--c-include=amtk/amtk.h		\
	--warn-all

Amtk_@AMTK_API_VERSION@_gir_NAMESPACE = Amtk
Amtk_@AMTK_API_VERSION@_gir_VERSION = @AMTK_API_VERSION@
Amtk_@AMTK_API_VERSION@_gir_CFLAGS = $(AMTK_DEP_CFLAGS)
Amtk_@AMTK_API_VERSION@_gir_SCANNERFLAGS = $(WARN_SCANNERFLAGS)
Amtk_@AMTK_API_VERSION@_gir_LIBS = libamtk-@AMTK_API_VERSION@.la
Amtk_@AMTK_API_VERSION@_gir_FILES =	\
	$(amtk_public_headers)		\
	$(amtk_public_c_files)		\
	$(amtk_built_public_headers)	\
	$(amtk_built_public_c_files)

Amtk_@AMTK_API_VERSION@_gir_INCLUDES = Gtk-3.0
Amtk_@AMTK_API_VERSION@_gir_PACKAGES = gtk+-3.0
Amtk_@AMTK_API_VERSION@_gir_EXPORT_PACKAGES = amtk-@AMTK_API_VERSION@

girdir = $(datadir)/gir-1.0
gir_DATA = Amtk-@AMTK_API_VERSION@.gir

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = Amtk-@AMTK_API_VERSION@.typelib

CLEANFILES +=		\
	$(gir_DATA)	\
	$(typelib_DATA)

endif # HAVE_INTROSPECTION

-include $(top_srcdir)/git.mk
