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
        -- Data output for left and right channel
        sd   : inout std_logic;
        -- Bit clock
        sck    : inout std_logic;
        -- Audio master clock output
        ws    : inout std_logic;
		  -- left channel data
		  left_channel : inout std_logic_vector(31 downto 0);
		  -- right channel data
		  right_channel : inout std_logic_vector(31 downto 0)
         );
end entity;

architecture rtl of i2s is

signal sv_sr	: std_logic_vector(31 downto 0);

begin
	SHIFT_REGISTER: process(reset, sck)
	begin
	if rising_edge(sck) then
		if reset = '1' then
			left_channel <= (others=>'0');
			right_channel <= (others=>'0');
			sck <= '0';
			sd <= '0';
		else
			sv_sr <= sd & sv_sr(31 downto 1);
		end if;
	end if;
	end process;
end rtl;
