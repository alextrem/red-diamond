# Makefile to extract 7 zip files
#
print-% : ;@echo $* = $($*)

EXTERNAL = ChibiOS/ext
TARFILES = $(wildcard $(EXTERNAL)/*.7z)

default:
	$(foreach file, $(TARFILES), $(shell 7z x -y -o$(EXTERNAL) $(file)))

.PHONY: default
