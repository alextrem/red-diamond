------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geißler
--
-- Create Date:      23:40:00 11/27/2016
-- Design Name:      i2s_pkg.vhd
-- Project Name:     red-diamond
-- Target Device:    EP4CE22C8N
-- Tool Versions:    16.0
-- Description:      This I2S package contains a
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package i2s_pkg is

  type t_i2s_in is record
    l_channel : std_logic_vector(23 downto 0);
    r_channel : std_logic_vector(23 downto 0);
  end record;

  type t_i2s_out is record
    wclk  : std_ulogic;
    bclk  : std_ulogic;
    sdata : std_logic;
  end record;

  component i2s_tx
  port (
    reset_n : in std_ulogic;
    mclk    : in std_ulogic;
    -- 
    i2s_in  : in t_i2s_in;
    i2s_out : out t_i2s_out
  );
  end component;

end i2s_pkg;
