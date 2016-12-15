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
use ieee.std_logic_1164.all

package hdmi is
  type t_hdmi_out is record
    -- I2C Interface
    scl : std_ulogic;
    sda : std_ulogic;
    csn : std_logic;

  end record t_hdmi_out;
end package;
