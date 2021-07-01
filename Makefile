cflags += -Iinclude
cflags += -std=c11
cflags += -Wall -Wextra -pedantic

lib.name = goat~
class.sources = src/goat~.c
common.sources =
datafiles = goat_tilde.pd

# use pd-lib-builder
include pd-lib-builder/Makefile.pdlibbuilder

# disable optimizations for debugging
alldebug: c.flags += -O0 -DDEBUG
alldebug: cxx.flags += -O0 -DDEBUG

# create the documentation
docs:
	mkdir -p docs
	doxygen

# clean the documentation directory
.PHONY: docs.clean
clean: docs.clean
docs.clean:
	rm -rf docs
