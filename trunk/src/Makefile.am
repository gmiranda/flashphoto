bin_PROGRAMS = flash
flash_SOURCES = Decoupling.cpp main.cpp Shadow.cpp

# set the include path found by configure
INCLUDES= $(all_includes)

# the library search path.
flash_LDFLAGS = $(all_libraries)
flash_LDADD = -lX11 -lpthread -lm
noinst_HEADERS = Helper.h CImg.h Shadow.h
