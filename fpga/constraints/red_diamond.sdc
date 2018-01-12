# clock inputs
create_clock -period "24.576MHz" -waveform{0.6 5.6} get_ports[clk]

# generate the pll clocks
derive_pll_clocks
derive_clockuncertainty

set_false_path -from * -to [get_ports *led]
