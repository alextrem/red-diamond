---------------------------------------
-- Company:
-- Engineer:        Alexander Geißler
--
-- Create Date:     23:40:00 02/26/2015
-- Design Name:
-- Project Name:    red-diamond
-- Target Device:   EP4CE22C8N
-- Tool Versions:    14.0
-- Description:
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
--------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

package spdif_pkg is
    constant X_PREAMBLE : std_logic_vector(7 downto 0) := "01000111";
    constant Y_PREAMBLE : std_logic_vector(7 downto 0) := "00100111";
    constant Z_PREAMBLE : std_logic_vector(7 downto 0) := "00010111";
    -- AES3/EBU or SPDIF stream constants
    constant FRAMES : std_logic_vector(7 downto 0) := "11000000";
    constant CHANNELS : std_logic;
    constant SUBFRAME : std_logic_vector(5 downto 0) := "100000";

    -- RX data will be 5 times oversampled
    -- 192000 Hz : 24,576  MHz
    -- 176400 Hz : 22,5792 MHz
    --  96000 Hz : 12,288  MHz
    --  88200 Hz : 11,2896 MHz
    --  48000 Hz :  6,144  MHz
    --  44100 Hz :  5,6448 MHz
    --  32000 Hz :  4,096  MHz

    -- 
end spdif_pkg;
