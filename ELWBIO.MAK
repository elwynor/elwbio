###############################################################################
#####                 This makefile generates ELWBIO.DLL                  #####
###############################################################################
!include $(WGDEV)\src\gcrules.mak

DEFILE =

VIRFILES =

!if $(OS) == WNT
DLLLIB = $(DLLLIB) wgserver.lib galgsbl.lib
MATHDLL =
DEFILE = 
!endif

!if $(OS) == WG2
DLLLIB = $(DLLLIB) wgserver.lib galgsbl.lib $(BORLAND)\lib\mathh.lib ${DOSEXT}\bc4\lib\phapi.lib
MATHDLL = $(DOSEXT)\bc4\lib\fpdmy
DEFILE = $(WGLIBDIR)\mathdef.def
!endif

OBJFILES = \
     ${TEMPDIR}\elwbio.obj 

start:              \
     $(GCBUILD)     \
     elwbio.dll

elwbio.dll:         \
     $(OBJFILES)    \
     $(DLLLIB)      \
     $(DEFILE)      \
     $(VIRFILES)
     $(LINK) $(LIBCMD) @&&|
$(PRODLL) ${MATHDLL} $(OBJFILES)
$(RUNDIR)\$&
$(RUNDIR)\$&
$(DLLLIB) $(LINKDLL)
$(DEFILE)
|
