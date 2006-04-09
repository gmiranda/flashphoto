CC        = gcc

#
# Modify TOPDIR if you use your own include files and library files
#

TOPDIR    = .
LIBDIR    = $(TOPDIR)/libs
INCDIR    = $(TOPDIR)/include

LIBS      = -lm -lstdc++  # add any -l's that you need here
          # don't need any for the sample program

CFLAGS    = -I$(INCDIR)
LDFLAGS   = -L$(LIBDIR) $(LIBS)

PROGRAM   = demo01     # name of the binary
SRCS      = raytracer.cpp camera.cpp sphere.cpp main.cpp rtobject.cpp image.cpp io.cpp material.cpp plane.cpp line.cpp
OBJS      = $(SRCS:.cpp=.o)


#
# Rules
#

#.SUFFIXES: .cpp .o

%.o : %.cpp raytracer.h
	$(CC) -c $(CFLAGS) $<

#
# Targets
#

default: all

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJS) $(PROGRAM)

