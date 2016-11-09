------------------------------------------------------------------------------
-- Company:             Red Diamond
-- Engineer:        	Alexander Geißler
--
-- Create Date:     	23:40:00 02/26/2015
-- Design Name:
-- Project Name:    	red-diamond
-- Target Device:   	EP4CE22C8N
-- Tool Versions:   	14.0
-- Description:		This AES3/EBU and SPDIF receiver is compliant with
--							IEC61937, IEC60958-3 and IEC60958-4
--                   The input is sampled in by either 
--                   49.152 MHz for 48kHz, 96kHz and 192kHz samplerates
--                   45.1584 MHz for 44.1kHz, 88.2kHz or 176.4 kHz
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
use work.spdif_pkg.ALL;

entity aes3rx is
    port (
        -- Synchronous reset
        reset   	: in std_logic;
        -- Master clock
        clk		: in std_logic;
        -- aes/ebu, spdif compatible stream
        aes3    : in std_logic;
		  -- left audio data
		  audio_left	: out std_logic_vector(23 downto 0);
		  -- right audio data
		  audio_right	: out std_logic_vector(23 downto 0);
		  -- status bits
		  --status			: out channel_status;
		  -- validity bit
		  validity_bit	: out std_logic := '0';
		  -- paritiy bit
		  parity_bit	: out std_logic := '0';
        -- receiver has valid input data 
        lock	 : out std_logic := '0'
         );
end aes3rx;

architecture rtl of aes3rx is

	signal sv_aes3					 : std_logic_vector(3 downto 0);
	signal sl_change 				 : std_logic;
	signal sl_aes3_clk          : std_logic;
	signal sv_clk_counter 		 : std_logic_vector(4 downto 0);
	signal current_state, next_state	: aes3_state_type := unlocked;
	signal sv_decoder_shift		 : std_logic_vector(7 downto 0);
   signal sl_preamble_detected : std_logic := '0'; -- Asserted when preamble has been detected
   signal sl_x_detected        : std_logic := '0'; -- Asserted when x preamble has been detected
   signal sl_y_detected        : std_logic := '0'; -- Asserted when y preamble has been detected
   signal sl_z_detected        : std_logic := '0'; -- Asserted when z preamble has been detected

begin

input_shift_reg_proc: process(clk, reset)
begin
	if rising_edge(clk) then
		if reset = '1' then
			sv_aes3 <= (others => '0');
		else
			sv_aes3 <= aes3 & sv_aes3(3 downto 1);
		end if;
	end if;
end process;

-- detects when change on input occurs
change_detect_proc: process(clk, reset)
begin
	if rising_edge(clk) then
		if reset = '1' then
			sl_change <= '0';
		else
			sl_change <= sv_aes3(2) xor sv_aes3(1);
		end if;
	end if;
end process;

aes3_clock_count_proc: process(clk)
begin
	if rising_edge(clk) then
		if reset = '1' then
			sv_clk_counter <= (others => '1');
		elsif sv_clk_counter = b"0_0000" then
			if sl_change = '1' then
				sv_clk_counter <= (others => '1');
			else
				sv_clk_counter <= sv_clk_counter - 1;
			end if;
		end if;
	end if;
end process;

--
decoder_shift_reg_proc: process(clk)
begin
	if rising_edge(clk) then
		if reset = '1' then
			sv_decoder_shift <= (others => '0');
		else
			sv_decoder_shift <= sv_aes3(0) & sv_decoder_shift(7 downto 1);
		end if;
	end if;
end process;

-- Locking state machine for AES3/EBU data stream.
-- The locking for 192kHz, 96kHz and 48kHz will be done in parallel.
-- The clock will be set to 122 MHz
lock_state_proc : process(current_state, sl_preamble_detected)
begin
	case current_state is
		when unlocked =>
			if sl_preamble_detected = '1' then
				next_state <= confirming;
			else
				next_state <= unlocked;
			end if;
		when confirming =>
			if sl_preamble_detected = '1' then
				next_state <= locked;
			else
				next_state <= unlocked;
			end if;
		when locked =>
			if sl_preamble_detected = '0' then
				next_state <= unlocked;
			else
				next_state <= locked;
			end if;
	end case;
end process;

-- Synchronization process for state machine
lock_state_machine_sync_proc: process(clk)
begin
	if rising_edge(clk) then
		if reset = '1' then
			current_state <= unlocked;
		elsif sl_change = '1' then
			current_state <= next_state;
		end if;
	end if;
end process;

-- Preamble detection processes
x_preamble_detector : process(sv_decoder_shift)
begin
	if sv_decoder_shift = X_PREAMBLE or sv_decoder_shift = not X_PREAMBLE then
		sl_x_detected <= '1';
	else
		sl_x_detected <= '0';
	end if;
end process;

y_preamble_detector : process(sv_decoder_shift)
begin
	if sv_decoder_shift = Y_PREAMBLE or sv_decoder_shift = not Y_PREAMBLE then
		sl_y_detected <= '1';
	else
		sl_y_detected <= '0';
	end if;
end process;

z_preamble_detector : process(sv_decoder_shift)
begin
	if sv_decoder_shift = Z_PREAMBLE or sv_decoder_shift = not Z_PREAMBLE then
		sl_z_detected <= '1';
	else
		sl_z_detected <= '0';
	end if;
end process;

sl_preamble_detected <= sl_x_detected or sl_y_detected or sl_z_detected;

end rtl;
