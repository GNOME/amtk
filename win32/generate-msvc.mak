# NMake Makefile portion for code generation and
# intermediate build directory creation
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.

# Copy the pre-defined config.h.win32
$(OUTDIR)\amtk\config.h: config.h.win32
	@if not exist $(@D)\ md $(@D)
	@-copy $** $@

$(OUTDIR)\amtk\amtk-enum-types.h: ..\amtk\amtk-enum-types.h.template $(amtk_headers_real)
$(OUTDIR)\amtk\amtk-enum-types.c: ..\amtk\amtk-enum-types.c.template $(amtk_headers_real)

$(OUTDIR)\amtk\amtk-enum-types.h $(OUTDIR)\amtk\amtk-enum-types.c:
	@if not exist $(@D)\ md $(@D)
	@cd ..\amtk
	@if "$(LEGACY_GLIB_MKENUMS)" == "" ($(PYTHON) $(GLIB_MKENUMS) --template $(@F).template $(amtk_public_headers)>..\win32\$@)
	@if not "$(LEGACY_GLIB_MKENUMS)" == "" ($(PERL) $(GLIB_MKENUMS) --template $(@F).template $(amtk_public_headers)>..\win32\$@)
	@cd ..\win32

# Generate listing file for introspection
$(OUTDIR)\amtk\Amtk_$(AMTK_VER)_gir_list:	\
$(amtk_headers_real)		\
$(amtk_srcs_real)			\
$(OUTDIR)\amtk\amtk-enum-types.h	\
$(OUTDIR)\amtk\amtk-enum-types.c
	@if exist $@ del $@
	@for %%s in ($**) do @echo %%s >> $@
