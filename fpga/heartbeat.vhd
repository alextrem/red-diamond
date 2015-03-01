---------------------------------------
-- Company:
-- Engineer:        Alexander Geißler
--
-- Create Date:     23:40:00 02/26/2015
-- Design Name:
-- Project Name:    red-diamond
-- Target Device:   EP4CE22C8N
-- Tool Versions:   14.0
-- Description:
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
--------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity heartbeat is
	port (
		clk	: in std_logic;
		counter_out	: out std_logic
	);
end entity;

architecture rtl of heartbeat is

signal counter_data : std_logic_vector(31 downto 0) := (others=>'0');

begin

process(clk)
begin
	if rising_edge(clk) then
		counter_data <= std_logic_vector(unsigned(counter_data) + 1);
	end if;
end process;

counter_out <= counter_data(21);

end rtl;