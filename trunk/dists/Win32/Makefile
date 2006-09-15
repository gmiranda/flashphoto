#
# Makefile for the Windows MSI Installer of Flash Photography
# http://developer.berlios.de/projects/flashphoto
#

# Options
WIXPATH	= $(ProgramFiles)\Wix
CANDLE	= $(WIXPATH)\candle.exe
LIGHT		= $(WIXPATH)\light.exe
VERSION	= 4.0
TARGET	= msi


# Source files
SOURCES	= flashphoto.xml
OBJECTS	= flashphoto.wixobj
FLASHEXE	= ..\..\bin\flash.exe
FLASHGUIEXE	= ..\..\bin\FlashGUI.exe

# Implicit rules

.SUFFIXES: .xml .wixobj

.xml.wixobj:
	$(CANDLE)  $<  -out $@

# Build targets
all:		$(TARGET)

# ToDo: automatically compile win32 flash
flash:
		copy $(FLASHEXE)
flashGUI:
		copy $(FLASHGUIEXE)

qtlibs:	
		copy $(QTDIR)\bin\QtCore4.dll
		copy $(QTDIR)\bin\QtGui4.dll

$(TARGET):	$(OBJECTS) qtlibs flash flashGUI
		$(LIGHT) $(OBJECTS)