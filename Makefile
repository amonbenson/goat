cflags += -Iinclude
cflags += -std=c11
cflags += -Wall -Wextra -pedantic

lib.name = goat~
class.sources = src/goat~.c
common.sources =
datafiles = goat_tilde.pd

# use pd-lib-builder
include pd-lib-builder/Makefile.pdlibbuilder

alldebug: c.flags += -O0 -DDEBUG
alldebug: cxx.flags += -O0 -DDEBUG
