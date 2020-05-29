dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.64)

# Package version of the form 'major.minor.micro'.
m4_define(amtk_package_version, 5.1.1)

AC_INIT([Amtk],
	[amtk_package_version],
	[https://gitlab.gnome.org/GNOME/amtk/issues],
	[amtk],
	[https://wiki.gnome.org/Projects/Amtk])

AX_REQUIRE_DEFINED([AX_IS_RELEASE])
AX_IS_RELEASE([git-directory])

# Libtool versioning
#
# For development releases (if the minor package version is odd), keep the same
# Libtool version.
#
# For a new minor stable release (when incrementing the minor package version
# to an even number), apply the following algorithm step by step:
# 1. If the library source code has changed at all since the last
#    update, then increment REVISION.
# 2. If any exported functions or data have been added, removed, or
#    changed since the last update, increment CURRENT and set REVISION
#    to 0.
# 3. If any exported functions or data have been added since the last
#    public release, increment AGE.
# 4. If any exported functions or data have been removed since the last
#    public release, set AGE to 0.
#
# When incrementing the API version (usually for a new major package version),
# set CURRENT, REVISION and AGE to 0 since it's like a new library.
AMTK_LT_CURRENT=0
AMTK_LT_REVISION=0
AMTK_LT_AGE=0
AMTK_LT_VERSION="$AMTK_LT_CURRENT:$AMTK_LT_REVISION:$AMTK_LT_AGE"
AC_SUBST([AMTK_LT_VERSION])

# API version, used for parallel installability.
# Not used in:
# - po/Makevars
# - the docs/reference/amtk-5.0-sections.txt filename
# Because as far as I've tested, it is not easily feasible to not hardcode the
# API version in those places (swilmet).
AMTK_API_VERSION=5
AC_SUBST([AMTK_API_VERSION])

# Dependencies
glib_req=2.56
gtk_req=3.22

AC_CONFIG_SRCDIR([amtk/amtk.h])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

# gnu strictness to generate the INSTALL file
AM_INIT_AUTOMAKE([1.14 tar-ustar dist-xz no-dist-gzip gnu -Wno-portability])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Check for programs
AC_PROG_CC
AC_PROG_INSTALL

AX_REQUIRE_DEFINED([PKG_PROG_PKG_CONFIG])
AX_REQUIRE_DEFINED([PKG_INSTALLDIR])
PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

AC_PATH_PROG([GLIB_MKENUMS], [glib-mkenums])

# Initialize libtool
LT_PREREQ([2.2.6])
LT_INIT([disable-static])

# Compile warnings. There can be deprecation warnings at any time, so disable
# -Werror by default with the [yes] argument.
AX_REQUIRE_DEFINED([AX_COMPILER_FLAGS])
AX_COMPILER_FLAGS([WARN_CFLAGS], [WARN_LDFLAGS], [yes])

# Dynamic linking dependencies
AX_REQUIRE_DEFINED([AX_PKG_CHECK_MODULES])
AX_PKG_CHECK_MODULES([AMTK_DEP],
		     [glib-2.0 >= $glib_req  gtk+-3.0 >= $gtk_req],
		     [])

# i18n
AM_GNU_GETTEXT([external])
# FIXME: Remove AM_GNU_GETTEXT_VERSION once autoreconf supports AM_GNU_GETTEXT_REQUIRE_VERSION.
AM_GNU_GETTEXT_VERSION([0.19.6])
AM_GNU_GETTEXT_REQUIRE_VERSION([0.19.6])
AC_SUBST([GETTEXT_PACKAGE], [amtk-$AMTK_API_VERSION])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Define to the gettext package name.])

# Documentation
AX_REQUIRE_DEFINED([GTK_DOC_CHECK])
GTK_DOC_CHECK([1.25], [--flavour=no-tmpl])

# Installed Tests for gnome continuous integration
AC_ARG_ENABLE(installed_tests,
	      AS_HELP_STRING([--enable-installed-tests],
			     [Install test programs (default: no)]),,
	      [enable_installed_tests=no])

AM_CONDITIONAL(INSTALLED_TESTS, test "x$enable_installed_tests" = "xyes")

# Introspection
AX_REQUIRE_DEFINED([GOBJECT_INTROSPECTION_CHECK])
GOBJECT_INTROSPECTION_CHECK([1.42.0])

# Code coverage for unit tests
AX_REQUIRE_DEFINED([AX_CODE_COVERAGE])
AX_CODE_COVERAGE

# Valgrind check
AX_REQUIRE_DEFINED([AX_VALGRIND_CHECK])
AX_VALGRIND_CHECK

AX_REQUIRE_DEFINED([PKG_CHECK_VAR])
PKG_CHECK_VAR([GLIB_PREFIX], [glib-2.0], [prefix])

# Output files
AC_CONFIG_FILES([
	amtk-${AMTK_API_VERSION}.pc:amtk.pc.in
	Makefile
	amtk/Makefile
	docs/Makefile
	docs/reference/amtk-docs.xml
	docs/reference/amtk-intro.xml
	docs/reference/Makefile
	po/Makefile.in
	tests/Makefile
	testsuite/Makefile
	win32/config.h.win32
	win32/config-msvc.mak
	win32/Makefile
])

AC_OUTPUT

echo "
Configuration:

	${PACKAGE_NAME} version ${PACKAGE_VERSION}

	Source code location:	${srcdir}
	Prefix:			${prefix}
	Compiler:		${CC}
	Documentation:		${enable_gtk_doc}
	GObject introspection:	${found_introspection}
	Code coverage:		${enable_code_coverage}
	Valgrind check:		${enable_valgrind}
	Installed tests:	${enable_installed_tests}
"
