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

docs:
	mkdir -p docs
	doxygen

# add a target to clean the docs directory
.PHONY: clean_docs
clean: clean_docs
clean_docs:
	rm -rf docs
