CC        = g++

#
# Modify TOPDIR if you use your own include files and library files
#

TOPDIR    = ..
LIBDIR    = $(TOPDIR)/libs
INCDIR    = $(TOPDIR)/include
OBJDIR = $(TOPDIR)/.objs
BINDIR = $(TOPDIR)/bin

LIBS      = -lstdc++  # add any -l's that you need here
          # don't need any for the sample program

# Tuve que eliminar --ffast-math y -msse3 porque no lo pilla gcc 2.95 :)
CFLAGS    = -I$(INCDIR) -Wall -O3 -pipe -g #-ffast-math -msse3 
LDFLAGS   = -L$(LIBDIR) $(LIBS)

PROGRAM   = demo01     # name of the binary
SRCS      = raytracer.cpp camera.cpp sphere.cpp main.cpp rtobject.cpp image.cpp io.cpp material.cpp plane.cpp line.cpp cylinder.cpp
SRCSCXX = cmaterial.cxx
OBJS      = $(SRCSCXX:.cxx=.o) $(SRCS:.cpp=.o)


#
# Rules
#

#.SUFFIXES: .cpp .o

%.o : %.cpp raytracer.h
	$(CC) -c $(CFLAGS) $<

%.o : %.cxx raytracer.h
	$(CC) -c $(CFLAGS) $<
#
# Targets
#

default: all

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) $(LDFLAGS) $(CFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM) *~

