cflags += -Iinclude
cflags += -std=c11
cflags += -Wall -Wextra -pedantic

lib.name = helloworld
class.sources = src/helloworld.c # add your source files containing pure data classes here
common.sources = # add all other source files (containing your algorithms, etc.) here
datafiles = helloworld.pd # add all aditional files (help patches, .wav files, etc.) here

# use pd-lib-builder
include pd-lib-builder/Makefile.pdlibbuilder
