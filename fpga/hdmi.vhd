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

entity hdmi is
    port (
        -- Synchrounous reset
        reset   : out std_ulogic;
        -- I2C interface
        scl     : out std_ulogic;
        sda     : inout std_ulogic;
        csn     : out std_ulogic;
        -- I2S interface
        -- Data outputclock for left and right channel
        lrclk   : in std_logic;
        -- Bit clock
        sclk    : in std_logic;
        -- Audio master clock output
        mclk    : in std_logic;
        ap0     : in std_logic;
        ap1     : in std_logic;
        ap2     : in std_logic;
        ap3     : in std_logic;
        ap4     : in std_logic;
        -- interrupt
        interrupt     : in std_logic_vector(1 downto 0)
        );
end entity;

architecture rtl of hdmi is

begin

end rtl;
