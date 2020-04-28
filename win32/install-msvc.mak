# NMake Makefile snippet for copying the built libraries, utilities and headers to
# a path under $(PREFIX).

install: all
	@if not exist $(PREFIX)\bin\ mkdir $(PREFIX)\bin
	@if not exist $(PREFIX)\lib\ mkdir $(PREFIX)\lib
	@if not exist $(PREFIX)\include\amtk-$(AMTK_VER)\amtk\ @mkdir $(PREFIX)\include\amtk-$(AMTK_VER)\amtk
	@for %%x in (dll pdb) do copy /b $(AMTK_DLL:.dll=).%%x $(PREFIX)\bin
	@copy /b vs$(PDBVER)\$(CFG)\$(PLAT)\amtk-$(AMTK_VER).lib $(PREFIX)\lib
	@for %%h in ($(amtk_public_headers)) do @copy ..\amtk\%%h $(PREFIX)\include\amtk-$(AMTK_VER)\amtk
	@copy vs$(PDBVER)\$(CFG)\$(PLAT)\amtk\amtk-enum-types.h $(PREFIX)\include\amtk-$(AMTK_VER)\amtk
	@rem Copy the generated introspection files, if built
	@if exist $(OUTDIR)\Amtk-$(AMTK_VER).gir if not exist $(PREFIX)\share\gir-1.0\ mkdir $(PREFIX)\share\gir-1.0
	@if exist $(OUTDIR)\Amtk-$(AMTK_VER).gir copy $(OUTDIR)\Amtk-$(AMTK_VER).gir $(PREFIX)\share\gir-1.0
	@if exist $(OUTDIR)\Amtk-$(AMTK_VER).typelib if not exist $(PREFIX)\lib\girepository-1.0\ mkdir $(PREFIX)\lib\girepository-1.0
	@if exist $(OUTDIR)\Amtk-$(AMTK_VER).typelib copy /b $(OUTDIR)\Amtk-$(AMTK_VER).typelib $(PREFIX)\lib\girepository-1.0
	@-$(PYTHON) amtkpc.py --version=$(AMTK_PKG_VERSION) --prefix=$(PREFIX)
	@if not exist amtk-$(AMTK_VER).pc echo *** amtk-$(AMTK_VER) is not generated!  Generate it later using ^$(PYTHON) --version=$(AMTK_PKG_VERSION) --prefix=^$(PREFIX)
	@if exist amtk-$(AMTK_VER).pc if not exist $(PREFIX)\lib\pkgconfig mkdir $(PREFIX)\lib\pkgconfig
	@if exist amtk-$(AMTK_VER).pc move amtk-$(AMTK_VER).pc $(PREFIX)\lib\pkgconfig
