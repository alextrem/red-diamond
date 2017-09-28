------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geissler
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
-- Revision 0.2 - Added sinus lookup table creation
------------------------------------------------------------------------------

library ieee;
use ieee.math_real.all;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

package i2s_pkg is

-------------------------------------------------------------------------------
-- Constants
-------------------------------------------------------------------------------

  constant c_addr    : integer := 12;

  type t_i2s_in is record
    l_channel : std_logic_vector(23 downto 0);
    r_channel : std_logic_vector(23 downto 0);
  end record;

  type t_i2s_out is record
    wclk  : std_ulogic;
    bclk  : std_ulogic;
    sdata : std_logic;
  end record;

  type mem_array is array(0 to (2**c_addr)-1) of std_logic_vector(23 downto 0);

-------------------------------------------------------------------------------
-- Functions
-------------------------------------------------------------------------------

  function cos_lut
  return mem_array;

-------------------------------------------------------------------------------
-- Components
-------------------------------------------------------------------------------

  component i2s_tx
  port (
    reset_n : in std_logic;
    mclk    : in std_logic;
    --
    i2s_in  : in t_i2s_in;
    i2s_out : out t_i2s_out
  );
  end component;

end i2s_pkg;

package body i2s_pkg is

  function cos_lut
  return mem_array is
    constant N     : integer := 2**c_addr;
    constant N1    : real := real(N);
    variable w, k1 : real;
    variable memx  : mem_array;
  begin
    for k in 0 to N-1 loop
      k1      := (real(k)+0.5)/N1;
      w       := cos(math_pi_over_2 * k1); -- first quadrant of cosine wave
      memx(k) := std_logic_vector(conv_signed(integer(round(8388608.0*w)),24));
    end loop;
    return memx;
  end function cos_lut;

end;
