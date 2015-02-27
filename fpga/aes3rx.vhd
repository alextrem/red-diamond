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
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use work.spdif_pkg.ALL;

entity aes3rx is
    port (
        -- Synchronous reset
        reset   : in std_logic;
        -- Master clock
        clk     : in std_logic;
        -- aes/ebu, spdif compatible stream
        aes3    : in std_logic;
		  -- left audio data
		  audio_left	: out std_logic_vector(23 downto 0);
		  -- right audio data
		  audio_right	: out std_logic_vector(23 downto 0);
		  -- validity bit
		  validity_bit	: out std_logic := '0';
		  -- paritiy bit
		  parity_bit	: out std_logic := '0';
        -- receiver has valid input data 
        lock	 : out std_logic := '0'
         );
end aes3rx;

architecture rtl of aes3rx is

	signal sv_aes3	: std_logic_vector(3 downto 0);
	signal sl_change : std_logic;
   signal sl_preamble_detected : std_logic := '0'; -- Asserted when preamble has been detected
   signal sl_x_detected        : std_logic := '0'; -- Asserted when x preamble has been detected
   signal sl_y_detected        : std_logic := '0'; -- Asserted when y preamble has been detected
   signal sl_z_detected        : std_logic := '0'; -- Asserted when z preamble has been detected

begin

input_shift_reg_proc: process(clk)
begin
	if clk'event and clk = '1' then
		if reset = '1' then
			sv_aes3 <= (others => '0');
		else
			sv_aes3 <= aes3 & sv_aes3(3 downto 1);
		end if;
	end if;
end process;

change_detect_proc: process(clk)
begin
	if rising_edge(clk) then
		if reset = '1' then
			sl_change <= '0';
		else
			sl_change <= sv_aes3(2) xor sv_aes3(1);
		end if;
	end if;
end process;

end rtl;
