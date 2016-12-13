PROJECT = red_diamond

GHDL_CMD = ghdl
GHDL_FLAGS = --ieee=synopsys --warn-no-vital-generic --std=02
MODULES = \
        red_diamond_top.vhd \
        aes3rx.vhd \
        i2s_tx.vhd \
        i2s_pkg.vhd \
        spdif_pkg.vhd

analyze:
	$(GHDL_CMD) -a $(GHDL_FLAGS) --workdir=simu --work=work $(MODULES)

all:
	$(GHDL_CMD) -i $(GHDL_FLAGS) --workdir=simu --work=work $(MODULES)
	$(GHDL_CMD) -m $(GHDL_FLAGS) --workdir=simu --work=work $(PROJECT)_top

clean:
	@echo "Cleaning ..."
	rm -Rf *.cf
