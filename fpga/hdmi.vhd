---------------------------------------
-- Company:
-- Engineer:        Alexander Geißler
--
-- Create Date:     23:40:00 02/26/2015
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

entity hdmi is
    port (
        -- Synchrounous reset
        reset   : out std_logic;
        -- I2C interface
        scl     : out std_logic;
        sda     : inout std_logic;
        csn     : out std_logic;
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

    type state is (idle);
    signal current_state : state:=idle;

begin

end rtl;
