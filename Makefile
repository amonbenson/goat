cflags += -Iinclude
cflags += -std=c11
cflags += -Wall -Wextra -pedantic

lib.name = helloworld
class.sources = src/helloworld.c

# use pd-lib-builder
include pd-lib-builder/Makefile.pdlibbuilder
