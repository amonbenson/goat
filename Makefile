cflags += -Iinclude
cflags += -std=gnu11
cflags += -Wall -Wextra

lib.name = goat~
goat~.class.sources = src/goat_tilde.c
common.sources = $(filter-out $(goat~.class.sources), $(shell find "src" -name "*.c"))
$(info common.sources: $(common.sources))
datafiles = $(wildcard goat_tilde.pd *.wav preset_*.txt)

# use pd-lib-builder
include pd-lib-builder/Makefile.pdlibbuilder

# disable optimizations for debugging
alldebug: c.flags += -O0 -DDEBUG
alldebug: cxx.flags += -O0 -DDEBUG

# create the documentation
DOXYGEN=doxygen
DOXYGEN_DIR=docs
.PHONY: docs docs.clean

docs:
	mkdir -p $(DOXYGEN_DIR)
	$(DOXYGEN)

# clean the documentation directory
clean: docs.clean
docs.clean:
	rm -rf $(DOXYGEN_DIR)

# valgrind command for leak checks
VG=valgrind
VG_LOG="valgrind.log"
VG_FLAGS=--log-file="$(VG_LOG)" --leak-check=full --show-reachable=yes --track-origins=yes -s
.PHONY: valgrind valgrind.clean

valgrind: alldebug
	$(VG) $(VG_FLAGS) pd -noaudio *.pd

clean: valgrind.clean
valgrind.clean:
	rm -f $(VG_LOG)
