------------------------------------------------------------------------------
-- Company:             None
-- Engineer:        	Alexander Geißler
--
-- Create Date:     	23:40:00 10/28/2015
-- Design Name:
-- Project Name:    	red-diamond
-- Target Device:   	EP4CE22C8N
-- Tool Versions:   	15.0
-- Description:		    4K fft implementation for audio spectrum
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity fft is
    port (
        -- Synchronous reset
        reset   	: in std_logic;
        -- Master clock
        clk		: in std_logic;
        -- receiver has valid input data
        lock	 : out std_logic := '0'
        );
end fft;

architecture rtl of aes3rx is

begin


end rtl;
