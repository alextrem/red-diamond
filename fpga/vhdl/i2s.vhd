---------------------------------------
-- Company:
-- Engineer:        Alexander Geißler
--
-- Create Date:     16:00:00 07/04/2015
-- Design Name:
-- Project Name:    red-diamond
-- Target Device:   EP4CE22C8N
-- Tool Versions:   14.0
-- Description:     Audio interface of
-- ADV7612
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
--------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity i2s is
    port (
        -- Synchrounous reset
        reset   : in std_logic;
        -- I2S interface
        -- Data outputclock for left and right channel
        lrclk   : in std_logic;
        -- Bit clock
        sclk    : in std_logic;
        -- Audio master clock output
        mclk    : in std_logic
         );
end entity;

architecture rtl of i2s is


begin

end rtl;
