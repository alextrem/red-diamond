PROJECT = RED_FPGA

GHDL_CMD = ghdl
GHDL_FLAGS = --ieee=synopsys --std=08
MODULES = \
        red_diamond_top.vhd\
		aes3rx.vhd\
		spdif_pkg.vhd

OBJS_FILES = $(patsubst %.vhd, %.o, $(notdir $(MODULES)) )

all:
	$(GHDL_CMD) -i $(GHDL_FLAGS) $(MODULES)
	$(GHDL_CMD) -m $(GHDL_FLAGS) $(PROJECT)
