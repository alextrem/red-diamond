------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geissler
--
-- Create Date:      22:10:00 12/14/2016
-- Design Name:      i2s_tx_tb.vhd
-- Project Name:     red-diamond
-- Target Device:    EP4CE22C8N
-- Tool Versions:    16.0
-- Description:      This is a i2s tx modul Testbench.
--                   Two 24 bit shift registers clock data to a D/A.
--                   Key Features:
--                   - configure receiver/transmitter, clock master/slave
--                     word select master/slave
--                   - ARM AMBA AXI4-Lite Bus (in future)
--                   - Justification modes: normal, left, right
--                   - Up to 8 I2S instances, configurable in different ways
--                   - Testmodes
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.i2s_pkg.all;

use ieee.std_logic_textio.all;

entity tb_i2s_tx is
end entity tb_i2s_tx;

architecture sim of tb_i2s_tx is

  signal sl_reset : std_logic := '0';
  signal sl_clock : std_logic := '0';
  signal slv_addr : std_logic_vector(c_addr-1 downto 0);

  signal slv_l_channel : std_logic_vector(23 downto 0);
  signal slv_r_channel : std_logic_vector(23 downto 0);

  signal sl_wclk  : std_logic;
  signal sl_bclk  : std_logic;
  signal sl_sdata : std_logic;

begin

  sl_clock <= not sl_clock after 10 ns;
  sl_reset <= '0' after 20 ns, '1' after 60 ns;

  uut: i2s_tx port map (
    reset_n => sl_reset,
    mclk => sl_clock,
    -- input
    r_i2s_in.slv_l_channel => slv_l_channel,
    r_i2s_in.slv_r_channel => slv_r_channel,
    -- output
    r_i2s_out.sl_wclk  => sl_wclk,
    r_i2s_out.sl_bclk  => sl_bclk,
    r_i2s_out.sl_sdata => sl_sdata
  );

end sim;
