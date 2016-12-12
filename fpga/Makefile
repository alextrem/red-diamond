PROJECT = RED_FPGA

GHDL_CMD = ghdl
GHDL_FLAGS = --ieee=synopsys --std=08 --workdir=work
MODULES = \
        red_diamond_top.vhd \
        aes3rx.vhd \
        i2s_tx.vhd \
        i2s_pkg.vhd \
        spdif_pkg.vhd

OBJS_FILES = $(patsubst %.vhd, %.o, $(notdir $(MODULES)) )

all:
	$(GHDL_CMD) -i $(GHDL_FLAGS) $(MODULES)
	$(GHDL_CMD) -m $(GHDL_FLAGS) $(PROJECT)

clean:
	@echo "Cleaning ..."
	rm -Rf *.cf
