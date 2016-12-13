------------------------------------------------------------------------------
-- Company:             Red Diamond
-- Engineer:            Alexander Geissler
--
-- Create Date:         23:40:00 02/26/2015
-- Design Name:         red_diamond_top
-- Project Name:        red-diamond
-- Target Device:       EP4CE22C8N
-- Tool Versions:       16.0
-- Description:	        This AES3/EBU and SPDIF receiver is compliant with
--                      IEC61937, IEC60958-3 and IEC60958-4
--                      The input is sampled in by either 
--                      49.152 MHz for 48kHz, 96kHz and 192kHz samplerates
--                      45.1584 MHz for 44.1kHz, 88.2kHz or 176.4 kHz
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
-- Revision 0.2 - Added first simple I2S transmitter
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.all;
use work.spdif_pkg.all;
use work.i2s_pkg.all;

entity red_diamond_top is
port (
  -- clock
  clk_48   : in std_logic;
  clk_44_1 : in std_logic;
  pll_lock : out std_logic;
	
  -- aes3/ebu or spdif input
  aes_din  : in std_logic;
  aes_lock : out std_logic;

  -- a/d input
  ad_bclk  : out std_logic := '0';
  ad_lrck  : out std_logic := '0';
  ad_data  : out std_logic := '0';
	
  -- hdmi input

  -- d/a output
  da_bclk	: out std_logic := '0';
  da_lrck	: out std_logic := '0';
  da_data	: out std_logic := '0';
	
  -- processor i2c connection
  --mcu_i2c_scl	: in std_logic;
  --mcu_i2c_sda	: inout std_logic;
	
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

component sys_pll
  port (
    areset    : in std_logic  := '0';
    clkswitch : in std_logic := '0';
    inclk0    : in std_logic  := '0';
    inclk1    : in std_logic  := '0';
    c0	      : out std_logic;
    locked    : out std_logic 
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
    aes_in.data => aes_din,
    aes_out.lock => aes_lock
  );

  inst_i2s_tx : i2s_tx
  port map (
    reset_n          => '1',
    mclk             => sl_clk,
    i2s_in.l_channel => x"00FFFF",
    i2s_in.r_channel => x"FFFF00",
    i2s_out.wclk     => da_lrck,
    i2s_out.bclk     => da_bclk,
    i2s_out.sdata    => da_data
  );

  inst_heartbeat: heartbeat
  port map (
    clk => sl_clk,
    counter_out => heartbeat_led
  );

end rtl;
