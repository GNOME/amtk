SUBDIRS = po amtk tests testsuite docs win32

include $(top_srcdir)/aminclude_static.am
clean-local: code-coverage-clean
distclean-local: code-coverage-dist-clean

AM_DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc --enable-introspection

pkgconfig_DATA = amtk-$(AMTK_API_VERSION).pc

DISTCLEANFILES = $(pkgconfig_DATA)

EXTRA_DIST =		\
	amtk.doap	\
	amtk.pc.in	\
	HACKING

MAINTAINERCLEANFILES = \
	$(GITIGNORE_MAINTAINERCLEANFILES_TOPLEVEL) \
	$(GITIGNORE_MAINTAINERCLEANFILES_MAKEFILE_IN) \
	$(GITIGNORE_MAINTAINERCLEANFILES_M4_LIBTOOL) \
	aminclude_static.am \
	gtk-doc.make \
	INSTALL

-include $(top_srcdir)/git.mk
