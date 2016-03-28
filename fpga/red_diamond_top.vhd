---------------------------------------
-- Company:
-- Engineer:        Alexander Geißler
--
-- Create Date:     23:40:00 02/26/2015
-- Design Name:
-- Project Name:    red-diamond
-- Target Device:   EP4CE22C8N
-- Tool Versions:   14.0
-- Description:
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
--------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use work.spdif_pkg.ALL;

entity red_diamond_top is
port (
   -- clock
	clk_48	: in std_logic;
	clk_44_1 : in std_logic;
	pll_lock	: out std_logic;
	
	-- aes3/ebu or spdif input
	din		: in std_logic;
	aes_lock	: out std_logic;

	-- a/d input
	ad_bclk	: out std_logic := '0';
	ad_lrck	: out std_logic := '0';
	ad_data	: out std_logic := '0';
	
	-- hdmi input

	-- d/a output
	da_bclk	: out std_logic := '0';
	da_lrck	: out std_logic := '0';
	da_data	: out std_logic := '0';
	
	-- processor i2c connection
	mcu_i2c_scl	: in std_logic;
	mcu_i2c_sda	: inout std_logic;
	
	-- LEDs
	heartbeat_led	: out std_logic
	
	-- display
	
);
end red_diamond_top;

architecture rtl of red_diamond_top is

signal sl_clk	: std_logic;

component heartbeat
	port (
		clk : in std_logic;
		counter_out : out std_logic
	);
end component;
	
component aes3rx
  port ( 
	reset   			: in std_logic := '0';
   clk     			: in std_logic;
   aes3    			: in std_logic;
	audio_left		: out std_logic_vector(23 downto 0);
	audio_right		: out std_logic_vector(23 downto 0);
	validity_bit	: out std_logic;
	parity_bit		: out std_logic;
   lock    			: out std_logic
);
end component aes3rx;

component sys_pll
	PORT
	(
		areset	: in std_logic  := '0';
		clkswitch	: in std_logic := '0';
		inclk0	: in std_logic  := '0';
		inclk1   : in std_logic  := '0';
		c0			: out std_logic;
		locked	: out std_logic 
	);
end component sys_pll;
begin

inst_pll: sys_pll
port map (
	areset => '0',
	clkswitch => '1',
	inclk0 => clk_48,
	inclk1 => clk_44_1,
	c0 => sl_clk,
	locked => pll_lock
);

inst_aes3rx: aes3rx
port map (
	reset => '0',
	clk => sl_clk,
	aes3 => din,
	lock => aes_lock
);

inst_heartbeat: heartbeat
port map (
	clk => sl_clk,
	counter_out => heartbeat_led
);

end rtl;