#
# Makefile for building Flash Photography command line application
#

# Options
CXX			= c++
DEFINES		=
FPNG		= /Library/Frameworks/libpng.framework
FJPEG		= /Library/Frameworks/libjpeg.framework
CXXFLAGS	= -pipe -O3  -fomit-frame-pointer -finline-functions -Wall -arch ppc -arch i386 -isysroot /Developer/SDKs/MacOSX10.4u.sdk  $(DEFINES)
INCPATH		= -I$(FPNG)/Headers -I$(FJPEG)/Headers
LFLAGS		= -arch ppc -arch i386 -Wl,-syslibroot,/Developer/SDKs/MacOSX10.4u.sdk 
LIBS		= -F/Library/Frameworks -lz -framework libpng -framework libjpeg -L/usr/X11R6/lib

COPY		= cp -f
COPY_FILE	= cp -f
DEL_FILE	= rm -f
CHK_DIR_EXISTS = test -d
COPY_DIR	= cp -f -R
MKDIR		= mkdir -p

# Output directory

OBJECTS_DIR	= .objs
DEST_DIR = ../../bin
BUNDLE = $(DEST_DIR)/FlashPhotography.app

# Source Files
SOURCES		= Decoupling.cpp \
			  Shadow.cpp \
			  main.cpp

OBJECTS		= Decoupling.o \
			  Shadow.o \
			  main.o

TARGET		= flash

# Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<


# Build rules
all:			$(TARGET)

$(TARGET):	$(OBJECTS)
			$(CXX) $(OBJECTS)  $(LFLAGS) -o $(TARGET) $(LIBS)

#Decoupling.o:	Decoupling.cpp Decoupling.h CImg.h Pixel.h Helper.h
#Shadow.o:		Shadow.cpp Shadow.h CImg.h Pixel.h Helper.h
#main.o:			main.cpp CImg.h Helper.h Decoupling.h Shadow.h

# This target will copy the flash executable into the flashgui bundle
# See http://doc.trolltech.com/4.1/deployment-mac.html
bundle:			$(TARGET) bundle_frameworks
				$(COPY_FILE) $(TARGET) $(BUNDLE)/Contents/MacOS/
# This target will copy the libjpeg and libpng frameworks
# See http://www.clanlib.org/download-binaries-osx-gcc40-universal.html
bundle_frameworks:
				@$(CHK_DIR_EXISTS) $(BUNDLE)/Contents/Frameworks || $(MKDIR) $(BUNDLE)/Contents/Frameworks
				$(COPY_DIR) $(FJPEG) $(BUNDLE)/Contents/Frameworks
				$(COPY_DIR) $(FPNG) $(BUNDLE)/Contents/Frameworks
	

# Housecleaning
clean:			
				$(DEL_FILE) $(OBJECTS)
				$(DEL_FILE) *~ *core *.core
distclean:		clean
				$(DEL_FILE) $(TARGET)