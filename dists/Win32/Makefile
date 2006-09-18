#
# Makefile for the Windows Exe of Flash Photography
# http://developer.berlios.de/projects/flashphoto
#

# Options
INNO	= $(ProgramFiles)\Inno Setup 5
ISCC	= $(INNO)\ISCC.exe
VERSION	= 4_exp
TARGET	= ..\flash_photo$(VERSION)_win32.exe

# TOOLS
RM_FILE	= del /Q
CP_FILE	= copy


# Source files
SOURCES		= setup.iss
FLASHEXE	= ..\..\bin\flash.exe
FLASHGUIEXE	= ..\..\bin\FlashGUI.exe
SAMPLEIMAGES	= ..\..\bin\images\*.png


# Build targets
all:		$(TARGET)

# ToDo: automatically compile win32 flash
flash:
		$(CP_FILE) $(FLASHEXE)
flashGUI:
		$(CP_FILE) $(FLASHGUIEXE)
samples:
		$(CP_FILE) $(SAMPLEIMAGES)

libs:	
		$(CP_FILE) $(QTDIR)\bin\QtCore4.dll
		$(CP_FILE) $(QTDIR)\bin\QtGui4.dll

$(TARGET):	libs flash flashGUI samples
		$(ISCC)  $(SOURCES) /f $(TARGET) /q
		
clean:
		$(RM_FILE) flash.exe
		$(RM_FILE) FlashGUI.exe
		$(RM_FILE) *.png
		$(RM_FILE) QtCore4.dll
		$(RM_FILE) QtGui4.dll

distclean:	clean
		$(RM_FILE) $(TARGET)
