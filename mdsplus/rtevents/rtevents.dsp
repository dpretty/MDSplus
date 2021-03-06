# Microsoft Developer Studio Project File - Name="rtevents" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=rtevents - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rtevents.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rtevents.mak" CFG="rtevents - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rtevents - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "rtevents - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rtevents - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTEVENTS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTEVENTS_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "rtevents - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTEVENTS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "HAVE_WINDOWS_H" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RTEVENTS_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\mdsshr\Debug\mdsshr.lib ..\mdstcpip\Debug\mdsipshr.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /dll /debug /machine:I386 /out:"../Debug/RtEventsShr.dll" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "rtevents - Win32 Release"
# Name "rtevents - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AllocationManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Event.cpp
# End Source File
# Begin Source File

SOURCE=.\EventConnector.cpp
# End Source File
# Begin Source File

SOURCE=.\EventHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\EventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MdsEvents.c
# End Source File
# Begin Source File

SOURCE=.\Notifier.cpp
# End Source File
# Begin Source File

SOURCE=.\RelativePointer.cpp
# End Source File
# Begin Source File

SOURCE=.\TCPNetworkManager.cpp
# End Source File
# Begin Source File

SOURCE=.\UDPNetworkManager.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowsExitHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowsSharedMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowsThread.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AllocationManager.h
# End Source File
# Begin Source File

SOURCE=.\Delay.h
# End Source File
# Begin Source File

SOURCE=.\Event.h
# End Source File
# Begin Source File

SOURCE=.\EventHandler.h
# End Source File
# Begin Source File

SOURCE=.\EventManager.h
# End Source File
# Begin Source File

SOURCE=.\ExitHandler.h
# End Source File
# Begin Source File

SOURCE=.\GlobalLock.h
# End Source File
# Begin Source File

SOURCE=.\IPAddress.h
# End Source File
# Begin Source File

SOURCE=.\Lock.h
# End Source File
# Begin Source File

SOURCE=.\MessageReceiver.h
# End Source File
# Begin Source File

SOURCE=.\NamedSemaphore.h
# End Source File
# Begin Source File

SOURCE=.\NetworkAddress.h
# End Source File
# Begin Source File

SOURCE=.\Notifier.h
# End Source File
# Begin Source File

SOURCE=.\RelativePointer.h
# End Source File
# Begin Source File

SOURCE=.\Runnable.h
# End Source File
# Begin Source File

SOURCE=.\SharedMemManager.h
# End Source File
# Begin Source File

SOURCE=.\SharedMemory.h
# End Source File
# Begin Source File

SOURCE=.\SharedMemState.h
# End Source File
# Begin Source File

SOURCE=.\SystemException.h
# End Source File
# Begin Source File

SOURCE=.\TCPMessageManager.h
# End Source File
# Begin Source File

SOURCE=.\TCPMessageSender.h
# End Source File
# Begin Source File

SOURCE=.\Thread.h
# End Source File
# Begin Source File

SOURCE=.\Timeout.h
# End Source File
# Begin Source File

SOURCE=.\UnnamedSemaphore.h
# End Source File
# Begin Source File

SOURCE=.\WindowsDelay.h
# End Source File
# Begin Source File

SOURCE=.\WindowsNamedSemaphore.h
# End Source File
# Begin Source File

SOURCE=.\WindowsSharedMemory.h
# End Source File
# Begin Source File

SOURCE=.\WindowsSystemException.h
# End Source File
# Begin Source File

SOURCE=.\WindowsThread.h
# End Source File
# Begin Source File

SOURCE=.\WindowsUnnamedSemaphore.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
