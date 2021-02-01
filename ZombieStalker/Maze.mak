# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=Maze - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Maze - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Maze - Win32 Release" && "$(CFG)" != "Maze - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Maze.mak" CFG="Maze - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Maze - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Maze - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Maze - Win32 Debug"
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "Maze - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
OUTDIR=.
INTDIR=.

ALL : "$(OUTDIR)\Maze.exe"

CLEAN : 
	-@erase ".\Maze.exe"
	-@erase ".\Maze.obj"
	-@erase ".\Wiredemo.obj"

# ADD BASE CPP /nologo /W3 /GX /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /Fp"Maze.pch"\
 /YX /c 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Maze.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/Maze.pdb" /machine:I386 /out:"$(OUTDIR)/Maze.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Maze.obj" \
	"$(INTDIR)/Wiredemo.obj"

"$(OUTDIR)\Maze.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Maze - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
OUTDIR=.
INTDIR=.

ALL : "$(OUTDIR)\Maze.exe"

CLEAN : 
	-@erase ".\vc40.pdb"
	-@erase ".\vc40.idb"
	-@erase ".\Maze.exe"
	-@erase ".\Maze.obj"
	-@erase ".\Wiredemo.obj"
	-@erase ".\Maze.ilk"
	-@erase ".\Maze.pdb"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"Maze.pch" /YX /c 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Maze.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/Maze.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Maze.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Maze.obj" \
	"$(INTDIR)/Wiredemo.obj"

"$(OUTDIR)\Maze.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx.obj:
   $(CPP) $(CPP_PROJ) $<  

.c.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Maze - Win32 Release"
# Name "Maze - Win32 Debug"

!IF  "$(CFG)" == "Maze - Win32 Release"

!ELSEIF  "$(CFG)" == "Maze - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Maze.cpp
DEP_CPP_MAZE_=\
	".\pcx.h"\
	".\keyboard.h"\
	".\sound.h"\
	".\graph.h"\
	

"$(INTDIR)\Maze.obj" : $(SOURCE) $(DEP_CPP_MAZE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\collin\Msc\Chap_11\Wiredemo.c

"$(INTDIR)\Wiredemo.obj" : $(SOURCE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
