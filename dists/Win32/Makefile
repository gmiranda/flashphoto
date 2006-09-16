#
# Makefile for the Windows MSI Installer of Flash Photography
# http://developer.berlios.de/projects/flashphoto
#

# Options
WIXPATH	= $(ProgramFiles)\Wix
CANDLE	= $(WIXPATH)\candle.exe
LIGHT		= $(WIXPATH)\light.exe
VERSION	= 4_exp
TARGET	= ..\flash_photo$(VERSION)_win32.msi


# Source files
SOURCES	= flashphoto.xml
OBJECTS	= flashphoto.wixobj
FLASHEXE	= ..\..\bin\flash.exe
FLASHGUIEXE	= ..\..\bin\FlashGUI.exe
SAMPLEIMAGES	= ..\..\bin\images\*.png

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
samples:
		copy $(SAMPLEIMAGES)

libs:	
		copy $(QTDIR)\bin\QtCore4.dll
		copy $(QTDIR)\bin\QtGui4.dll

$(TARGET):	libs flash flashGUI samples $(OBJECTS)
		$(LIGHT) $(OBJECTS) -out $(TARGET)