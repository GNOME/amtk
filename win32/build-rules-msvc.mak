# NMake Makefile portion for compilation rules
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.  The format
# of NMake Makefiles here are different from the GNU
# Makefiles.  Please see the comments about these formats.

# Inference rules for compiling the .obj files.
# Used for libs and programs with more than a single source file.
# Format is as follows
# (all dirs must have a trailing '\'):
#
# {$(srcdir)}.$(srcext){$(destdir)}.obj::
# 	$(CC)|$(CXX) $(cflags) /Fo$(destdir) /c @<<
# $<
# <<
{..\amtk\}.c{$(OUTDIR)\amtk\}.obj::
	@if not exist $(OUTDIR)\amtk\ md $(OUTDIR)\amtk
	@if not exist $(OUTDIR)\amtk\config.h copy config.h.win32 $(OUTDIR)\amtk\config.h
	$(CC) $(AMTK_CFLAGS) $(AMTK_INCLUDES) $(AMTK_LOG_DOMAIN) /Fo$(OUTDIR)\amtk\ /Fd$(OUTDIR)\amtk\ /c @<<
$<
<<

{$(OUTDIR)\amtk\}.c{$(OUTDIR)\amtk\}.obj::
	$(CC) $(AMTK_CFLAGS) $(AMTK_INCLUDES) $(AMTK_LOG_DOMAIN) /Fo$(OUTDIR)\amtk\ /Fd$(OUTDIR)\amtk\ /c @<<
$<
<<

$(OUTDIR)\amtk\amtk-enum-types.obj: $(OUTDIR)\amtk\amtk-enum-types.h $(OUTDIR)\amtk\amtk-enum-types.c

# Rules for building .lib files
vs$(PDBVER)\$(CFG)\$(PLAT)\amtk-$(AMTK_VER).lib: $(AMTK_DLL)

# Rules for linking DLLs
# Format is as follows (the mt command is needed for MSVC 2005/2008 builds):
# $(dll_name_with_path): $(dependent_libs_files_objects_and_items)
#	link /DLL [$(linker_flags)] [$(dependent_libs)] [/def:$(def_file_if_used)] [/implib:$(lib_name_if_needed)] -out:$@ @<<
# $(dependent_objects)
# <<
# 	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
$(AMTK_DLL):	\
$(amtk_OBJS)
	link /DLL $(LDFLAGS) 	\
	$(BASE_DEP_LIBS)	\
	/implib:vs$(PDBVER)\$(CFG)\$(PLAT)\amtk-$(AMTK_VER).lib	\
	-out:$@ @<<
$(amtk_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2

!include amtk_progs.mak

!if [del /f /q amtk_progs.mak]
!endif

$(amtk_testsuite_progs):
	@if not exist $(@D)\amtk-testsuite\ md $(@D)\amtk-testsuite
	$(CC) $(BASE_CFLAGS) $(AMTK_INCLUDES) /Fo$(@D)\amtk-testsuite\ /Fd$(@D)\amtk-testsuite\ $** /Fe$@ /link $(LDFLAGS) $(BASE_DEP_LIBS)
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

$(amtk_test_progs):
	@if not exist $(@D)\amtk-tests\ md $(@D)\amtk-tests
	$(CC) $(BASE_CFLAGS) $(AMTK_INCLUDES) /Fo$(@D)\amtk-tests\ /Fd$(@D)\amtk-tests\ $** /Fe$@ /link $(LDFLAGS) $(BASE_DEP_LIBS)
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

!ifdef INTROSPECTION
$(OUTDIR)\Amtk-$(AMTK_API_VER).gir: vs$(PDBVER)\$(CFG)\$(PLAT)\amtk-$(AMTK_VER).lib $(OUTDIR)\amtk\Amtk_$(AMTK_VER)_gir_list
	@-echo Generating $@...
	@set PATH=$(BINDIR);$(PATH)
	$(PYTHON) $(G_IR_SCANNER)	\
	--verbose -no-libtool	\
	--namespace=Amtk	\
	--nsversion=$(AMTK_VER)	\
	--pkg=gtk+-3.0	\
	--library=amtk-$(AMTK_VER)	\
	--add-include-path=$(G_IR_INCLUDEDIR)	\
	--include=Gtk-3.0	\
	--pkg-export=amtk-$(AMTK_VER)	\
	--cflags-begin	\
	$(EXTRA_BASE_CFLAGS:/=-)		\
	-DAMTK_COMPILATION			\
	$(AMTK_INCLUDES:/I=-I)		\
	--cflags-end	\
	--c-include=amtk/amtk.h	\
	--warn-all	\
	--filelist=$(OUTDIR)\amtk\Amtk_$(AMTK_VER)_gir_list	\
	-L.\$(OUTDIR) -L$(LIBDIR) -L$(BINDIR)	\
	-o $@

$(OUTDIR)\Amtk-$(AMTK_VER).typelib: $(OUTDIR)\Amtk-$(AMTK_VER).gir
	@-echo Compiling $@...
	$(G_IR_COMPILER)	\
	--includedir=. --includedir=$(G_IR_TYPELIBDIR) --debug --verbose	\
	$(@D:\=/)/$(@B).gir	\
	-o $@
!endif

clean:
	@if exist $(OUTDIR)\Amtk-$(AMTK_API_VER).typelib del /f /q $(OUTDIR)\Amtk-$(AMTK_API_VER).typelib
	@if exist $(OUTDIR)\Amtk-$(AMTK_API_VER).gir del /f /q $(OUTDIR)\Amtk-$(AMTK_API_VER).gir
	@-del /f /q $(OUTDIR)\*.exe
	@-del /f /q $(OUTDIR)\*.exe.manifest
	@-del /f /q $(OUTDIR)\*.dll
	@-del /f /q $(OUTDIR)\*.dll.manifest
	@-del /f /q $(OUTDIR)\*.pdb
	@-del /f /q $(OUTDIR)\*.ilk
	@-del /f /q $(OUTDIR)\*.exp
	@-del /f /q $(OUTDIR)\*.lib
	@-del /s /q $(OUTDIR)\amtk-testsuite\*.pdb
	@-del /s /q $(OUTDIR)\amtk-testsuite\*.obj
	@-del /s /q $(OUTDIR)\amtk-tests\*.pdb
	@-del /s /q $(OUTDIR)\amtk-tests\*.obj
	@-del /s /q $(OUTDIR)\amtk\Amtk_$(AMTK_API_VER)_gir_list
	@-del /s /q $(OUTDIR)\amtk\*.obj
	@-del /s /q $(OUTDIR)\amtk\*.pdb
	@-del /s /q $(OUTDIR)\amtk\amtk-enum-types.*
	@-del /s /q $(OUTDIR)\amtk\config.h
	@-rd $(OUTDIR)\amtk-testsuite $(OUTDIR)\amtk-tests $(OUTDIR)\amtk
