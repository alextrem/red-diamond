------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geißler
--
-- Create Date:      23:40:00 11/26/2016
-- Design Name:      hdmi_pkg.vhd
-- Project Name:     red-diamond
-- Target Device:    EP4CE22C8N
-- Tool Versions:    16.0
-- Description:      This package contains stuff for the hdmi receiver
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

package hdmi_pkg is
  type t_hdmi_out is record
    -- I2C Interface
    scl : std_ulogic;
    sda : std_ulogic;
    csn : std_logic;

    reset : std_logic;
  end record t_hdmi_out;

  type t_hdmi_in is record
    lrck : std_logic; -- word clock
    sclk : std_logic; -- Bit clock
    ap   : std_logic_vector(4 downto 0);
    int  : std_logic; --interrupt
  end record t_hdmi_in;


  component hdmi is
    port (
      reset    : in std_logic;
      mclk     : in std_logic;

      hdmi_in  : in t_hdmi_in;
      hdmi_out : out t_hdmi_out
    );
  end component;

end package;
