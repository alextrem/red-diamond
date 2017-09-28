# Copyright (C) 2017  Intel Corporation. All rights reserved.
# Your use of Intel Corporation's design tools, logic functions 
# and other software and tools, and its AMPP partner logic 
# functions, and any output files from any of the foregoing 
# (including device programming or simulation files), and any 
# associated documentation or information are expressly subject 
# to the terms and conditions of the Intel Program License 
# Subscription Agreement, the Intel Quartus Prime License Agreement,
# the Intel MegaCore Function License Agreement, or other 
# applicable license agreement, including, without limitation, 
# that your use is for the sole purpose of programming logic 
# devices manufactured by Intel and sold by Intel or its 
# authorized distributors.  Please refer to the applicable 
# agreement for further details.

# Quartus Prime: Generate Tcl File for Project
# File: red_diamond.tcl
# Generated on: Thu Sep 28 23:51:53 2017

# Load Quartus Prime Tcl Project package
package require ::quartus::project

set need_to_close_project 0
set make_assignments 1

# Check that the right project is open
if {[is_project_open]} {
	if {[string compare $quartus(project) "red_diamond"]} {
		puts "Project red_diamond is not open"
		set make_assignments 0
	}
} else {
	# Only open if not already open
	if {[project_exists red_diamond]} {
		project_open -revision red_diamond red_diamond
	} else {
		project_new -revision red_diamond red_diamond
	}
	set need_to_close_project 1
}

# Make assignments
if {$make_assignments} {
	set_global_assignment -name FAMILY "Cyclone IV E"
	set_global_assignment -name DEVICE EP4CE22E22C7
	set_global_assignment -name TOP_LEVEL_ENTITY red_diamond_top
	set_global_assignment -name ORIGINAL_QUARTUS_VERSION 14.0
	set_global_assignment -name PROJECT_CREATION_TIME_DATE "21:14:26  OCTOBER 29, 2015"
	set_global_assignment -name LAST_QUARTUS_VERSION "17.0.0 Lite Edition"
	set_global_assignment -name PROJECT_OUTPUT_DIRECTORY output_files
	set_global_assignment -name MIN_CORE_JUNCTION_TEMP 0
	set_global_assignment -name MAX_CORE_JUNCTION_TEMP 85
	set_global_assignment -name DEVICE_FILTER_PACKAGE "ANY QFP"
	set_global_assignment -name DEVICE_FILTER_PIN_COUNT 144
	set_global_assignment -name DEVICE_FILTER_SPEED_GRADE 7
	set_global_assignment -name ERROR_CHECK_FREQUENCY_DIVISOR 1
	set_global_assignment -name NOMINAL_CORE_SUPPLY_VOLTAGE 1.2V
	set_global_assignment -name EDA_SIMULATION_TOOL "ModelSim-Altera (VHDL)"
	set_global_assignment -name EDA_OUTPUT_DATA_FORMAT VHDL -section_id eda_simulation
	set_global_assignment -name EDA_BOARD_DESIGN_SIGNAL_INTEGRITY_TOOL "IBIS (Signal Integrity)"
	set_global_assignment -name EDA_OUTPUT_DATA_FORMAT IBIS -section_id eda_board_design_signal_integrity
	set_global_assignment -name PARTITION_NETLIST_TYPE SOURCE -section_id Top
	set_global_assignment -name PARTITION_FITTER_PRESERVATION_LEVEL PLACEMENT_AND_ROUTING -section_id Top
	set_global_assignment -name PARTITION_COLOR 16764057 -section_id Top
	set_global_assignment -name POWER_PRESET_COOLING_SOLUTION "15 MM HEAT SINK WITH STILL AIR"
	set_global_assignment -name POWER_BOARD_THERMAL_MODEL TYPICAL
	set_global_assignment -name STRATIX_DEVICE_IO_STANDARD "2.5 V"
	set_global_assignment -name POWER_USE_INPUT_FILES ON
	set_global_assignment -name POWER_USE_DEVICE_CHARACTERISTICS MAXIMUM
	set_global_assignment -name EDA_IBIS_MODEL_SELECTOR ON -section_id eda_board_design_signal_integrity
	set_global_assignment -name OPTIMIZE_HOLD_TIMING "ALL PATHS"
	set_global_assignment -name OPTIMIZE_MULTI_CORNER_TIMING ON
	set_global_assignment -name OPTIMIZE_POWER_DURING_FITTING "EXTRA EFFORT"
	set_global_assignment -name FITTER_EFFORT "STANDARD FIT"
	set_global_assignment -name CYCLONEII_OPTIMIZATION_TECHNIQUE SPEED
	set_global_assignment -name PHYSICAL_SYNTHESIS_COMBO_LOGIC ON
	set_global_assignment -name PHYSICAL_SYNTHESIS_REGISTER_DUPLICATION ON
	set_global_assignment -name PHYSICAL_SYNTHESIS_REGISTER_RETIMING ON
	set_global_assignment -name ROUTER_LCELL_INSERTION_AND_LOGIC_DUPLICATION ON
	set_global_assignment -name ROUTER_TIMING_OPTIMIZATION_LEVEL MAXIMUM
	set_global_assignment -name QII_AUTO_PACKED_REGISTERS NORMAL
	set_global_assignment -name ROUTER_CLOCKING_TOPOLOGY_ANALYSIS ON
	set_global_assignment -name EDA_RUN_TOOL_AUTOMATICALLY OFF -section_id eda_simulation
	set_global_assignment -name EDA_BOARD_DESIGN_BOUNDARY_SCAN_TOOL "BSDL (Boundary Scan)"
	set_global_assignment -name EDA_OUTPUT_DATA_FORMAT BSDL -section_id eda_board_design_boundary_scan
	set_global_assignment -name VHDL_INPUT_VERSION VHDL_2008
	set_global_assignment -name VHDL_SHOW_LMF_MAPPING_MESSAGES OFF
	set_global_assignment -name PROJECT_IP_REGENERATION_POLICY ALWAYS_REGENERATE_IP
	set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS ON
	set_global_assignment -name SMART_RECOMPILE ON
	set_global_assignment -name NUM_PARALLEL_PROCESSORS ALL
	set_global_assignment -name EDA_BOARD_DESIGN_TIMING_TOOL "Stamp (Timing)"
	set_global_assignment -name EDA_OUTPUT_DATA_FORMAT STAMP -section_id eda_board_design_timing
	set_global_assignment -name EDA_DESIGN_ENTRY_SYNTHESIS_TOOL Custom
	set_global_assignment -name EDA_INPUT_DATA_FORMAT VHDL -section_id eda_design_synthesis
	set_global_assignment -name SDC_FILE red_diamond.sdc
	set_global_assignment -name VHDL_FILE vhdl/i2s_tx.vhd
	set_global_assignment -name VHDL_FILE vhdl/red_diamond_top.vhd
	set_global_assignment -name VHDL_FILE vhdl/hdmi_pkg.vhd
	set_global_assignment -name VHDL_FILE vhdl/spdif_pkg.vhd
	set_global_assignment -name VHDL_FILE vhdl/i2s_pkg.vhd
	set_global_assignment -name VHDL_FILE vhdl/i2s.vhd
	set_global_assignment -name VHDL_FILE vhdl/heartbeat.vhd
	set_global_assignment -name VHDL_FILE vhdl/hdmi.vhd
	set_global_assignment -name VHDL_FILE vhdl/aes3rx.vhd
	set_global_assignment -name QIP_FILE vhdl/ip/altera/pll/pll.qip
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to ad_bclk
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to ad_data
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to ad_lrck
	set_location_assignment PIN_49 -to da_bclk
	set_location_assignment PIN_50 -to da_data
	set_location_assignment PIN_51 -to da_lrck
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to pll_lock
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to da_bclk
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to da_data
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to da_lrck
	set_location_assignment PIN_30 -to heartbeat_led
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to heartbeat_led
	set_location_assignment PIN_28 -to pll_lock
	set_location_assignment PIN_53 -to clk_44_1
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to clk_44_1
	set_location_assignment PIN_52 -to clk_48
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to clk_48
	set_location_assignment PIN_58 -to aes_lock
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to aes_lock
	set_instance_assignment -name IO_STANDARD "3.3-V LVCMOS" -to aes_din
	set_location_assignment PIN_39 -to aes_din
	set_instance_assignment -name PARTITION_HIERARCHY root_partition -to | -section_id Top

	# Commit assignments
	export_assignments

	# Close project
	if {$need_to_close_project} {
		project_close
	}
}
