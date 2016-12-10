------------------------------------------------------------------------------
-- Company:             Red Diamond
-- Engineer:            Alexander Geissler
--
-- Create Date:         23:40:00 02/26/2015
-- Design Name:
-- Project Name:        red-diamond
-- Target Device:       EP4CE22C8N
-- Tool Versions:       16.0
-- Description:	        This AES3/EBU and SPDIF receiver is compliant with
--                      IEC61937, IEC60958-3 and IEC60958-4
--                      The input is sampled in by either 
--                      49.152 MHz for 48kHz, 96kHz and 192kHz samplerates
--                      45.1584 MHz for 44.1kHz, 88.2kHz or 176.4 kHz
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
-- Revision 0.2 - Changed indentation
--              - rewrite of the state machine
------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use work.spdif_pkg.ALL;

entity aes3rx is
  port (
    -- Synchronous reset
    reset   : in std_logic;
    -- Master clock
    clk     : in std_logic;
    
    aes_in  : in t_aes_in;
    aes_out : out t_aes_out
  );
end aes3rx;

architecture rtl of aes3rx is

  type t_reg_type is record
    slv_aes3  : std_logic_vector(3 downto 0);
    sl_change : std_logic;
    slv_clk_counter : std_logic_vector(7 downto 0);
    slv_decoder_shift : std_logic_vector(7 downto 0);
    sl_x_detected : std_logic; -- Asserted when x preamble has been detected
    sl_y_detected : std_logic; -- Asserted when y preamble has been detected
    sl_z_detected : std_logic; -- Asserted when z preamble has been detected
    sl_preamble_detected : std_logic; -- Asserted when all preambles
    sl_lock : std_logic;
    state : t_aes3_state;
  end record;

  signal r, r_next : t_reg_type;

begin

  input_shift_proc: process(aes_in, reset, r)
    variable v : t_reg_type;
  begin
    v := r;

    -- getting asynchronous input
    v.slv_aes3 := aes_in.data & r.slv_aes3(3 downto 1);
    -- detecting signal change
    v.sl_change := r.slv_aes3(2) xor r.slv_aes3(1);

    -- counting
    if r.slv_clk_counter = b"0000_0000" then
      if r.sl_change = '1' then
        v.slv_clk_counter := (others => '1');
      else
        v.slv_clk_counter := r.slv_clk_counter - 1;
      end if;
    end if;

  -- decoder shift register
  v.slv_decoder_shift := r.slv_aes3(0) & r.slv_decoder_shift(7 downto 1);

  -- preamble detection
  v.sl_x_detected := preamble_detection(r.slv_decoder_shift, X_PREAMBLE);
  v.sl_y_detected := preamble_detection(r.slv_decoder_shift, Y_PREAMBLE);
  v.sl_z_detected := preamble_detection(r.slv_decoder_shift, Z_PREAMBLE);

  v.sl_preamble_detected := r.sl_x_detected and r.sl_y_detected and r.sl_z_detected;
  -- Locking state machine for AES3/EBU data stream.
  -- The locking for 192kHz, 96kHz and 48kHz will be done in parallel.
  -- The clock will be set to 122 MHz
  case r.state is
    when UNLOCKED =>
      if r.sl_preamble_detected = '1' then
        v.state := CONFIRMING;
        v.sl_lock  := '0';
      end if;
    when CONFIRMING =>
      if r.sl_preamble_detected = '1' then
        v.state := LOCKED;
      end if;
    when LOCKED =>
      if r.sl_preamble_detected = '0' then
        v.state := UNLOCKED;
        v.sl_lock := '1';
      end if;
  end case;

  if (reset = '0') then
    v.state := UNLOCKED;
  end if;

  r_next <= v;
  end process input_shift_proc;

  proc : process (clk)
  begin
    if rising_edge(clk) then
      r <= r_next;
    end if;
  end process proc;

  aes_out.lock <= r.sl_lock;  

end rtl;
