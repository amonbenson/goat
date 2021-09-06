cflags += -Iinclude
cflags += -std=c11
cflags += -Wall -Wextra

lib.name = goat~
goat~.class.sources = src/goat_tilde.c
common.sources = src/goat.c src/util/circbuf.c src/granular/granular.c src/graintable/graintable.c src/scheduler/scheduler.c src/evelopbuf/evelopbuf.c src/synthesizer/synthesizer.c
datafiles = goat_tilde.pd Radiohead-reconer_piano.wav

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
