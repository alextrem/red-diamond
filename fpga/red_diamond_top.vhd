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

entity red_diamond_top is
port (
	clk	: in std_logic;
	
	-- aes3/ebu or spdif input
	din	: in std_logic;

	-- a/d input
	ad_bclk	: out std_logic := '0';
	ad_lrck	: out std_logic := '0';
	ad_data	: out std_logic := '0';
	
	-- hdmi input
	
	-- d/a output
	da_bclk	: out std_logic := '0';
	da_lrck	: out std_logic := '0';
	da_data	: out std_logic := '0';
	
	-- processor spi connection
	mcu_spi_clk		: in std_logic;
	mcu_spi_miso	: out std_logic := '0';
	mcu_spi_mosi	: in std_logic;
	mcu_spi_cs_n	: in std_logic
);
end red_diamond_top;

architecture rtl of red_diamond_top is

signal sl_clk	: std_logic;
component aes3rx
  port ( 
	reset   			: in std_logic;
   clk     			: in std_logic;
   aes3    			: in std_logic;
	audio_left		: out std_logic_vector(23 downto 0);
	audio_right		: out std_logic_vector(23 downto 0);
	validity_bit	: out std_logic;
	parity_bit		: out std_logic;
   lock    			: out std_logic
);
end component;

component clock
	PORT
	(
		areset	: IN STD_LOGIC  := '0';
		inclk0	: IN STD_LOGIC  := '0';
		c0			: OUT STD_LOGIC ;
		locked	: OUT STD_LOGIC 
	);
end component;
begin

inst_aes3rx: aes3rx
port map (
	reset => '0',
	clk => sl_clk,
	aes3 => din
);

inst_pll: clock
port map (
	areset => '0',
	inclk0 => clk,
	c0 => sl_clk,
	locked => open
);
end rtl;