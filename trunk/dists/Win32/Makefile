#
# Makefile for the Windows MSI Installer of Flash Photography
# http://developer.berlios.de/projects/flashphoto
#

# Options
WIXPATH	= %PROGRAMFILES%\Wix
CANDLE	= $(WIXPATH)\candle.exe
LIGHT		= $(WIXPATH)\light.exe
VERSION	= 4.0
TARGET	= msi

# Source files
SOURCES	= flashphoto.xml
OBJECTS	= flashphoto.wixobj

# Implicit rules

.SUFFIXES: .xml .wixobj

.xml.wixobj:
	$(CANDLE)  $<  -out $@

# Build targets
all:		$(TARGET)

$(TARGET):	$(OBJECTS)
		$(LIGHT) $(OBJECTS)