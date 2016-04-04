######
# This is a Makefile to automatically extract zip sources
#

EXTERNAL = ChibiOS/ext

TARFILES = $(wildcard $EXTERNAL/*.7z EXTERNAL/*.tar.gz EXTERNAL/*.tgz)

EXTRACT_TARGETS := $(addprefix .extract_, $(TARFILES))
CLEAN_TARGETS := $(addprefix .clean_, $(TARFILES))

default: $(EXTRACT_TARGETS)

clean: $(CLEAN_TARGETS)

$(CLEAN_TARGETS): .clean_%: .extract_%
	rm -rf "$(shell tar tf $(patsubst .extract_%,%,$<) | head -1)"
	rm -f $<

$(EXTRACT_TARGETS): .extract_%: %
	7z x $<
	touch $@

.PHONY: default clean $(CLEAN_TARGETS)
