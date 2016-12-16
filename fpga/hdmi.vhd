------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geißler
--
-- Create Date:      23:40:00 11/26/2016
-- Design Name:      hdmi.vhd
-- Project Name:     red-diamond
-- Target Device:    EP4CE22C8N
-- Tool Versions:    16.0
-- Description:      This module interfaces an hdmi receiver
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.ALL;
use work.hdmi_pkg.all;

entity hdmi is
  port (
    -- Synchrounous reset
    reset   : in std_logic;
    mclk    : in std_logic;

    hdmi_in : in t_hdmi_in;
    hdmi_out : out t_hdmi_out
  );
end entity;

architecture rtl of hdmi is

begin

end rtl;
