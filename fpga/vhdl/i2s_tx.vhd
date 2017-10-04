------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geissler
--
-- Create Date:      23:40:00 11/27/2016
-- Design Name:      i2s_tx.vhd
-- Project Name:     red-diamond
-- Target Device:    EP4CE22C8N
-- Tool Versions:    16.0
-- Description:      This is a i2s tx modul. Two 24 bit shift registers clock
--                   data to a D/A.
--                   Key Features:
--                   - configure receiver/transmitter, clock master/slave
--                     word select master/slave
--                   - ARM AMBA AXI4-Lite Bus (in future)
--                   - Justification modes: normal, left, right
--                   - Up to 8 I2S instances, configurable in different ways
--                   - Testmodes
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
------------------------------------------------------------------------------

--! standard library
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
--! i2s constants and definitions
use work.i2s_pkg.all;

entity i2s_tx is
   generic ( DATA_WIDTH : integer range 16 to 24 := 24
   );
   port (
      -- Synchronous reset
      reset_n : in std_logic;
      -- Master clock
      mclk    : in std_logic;

      -- I2S interface
      --! input
      r_i2s_in  : in t_i2s_in;
      --! output
      r_i2s_out : out t_i2s_out
   );
end entity;

architecture rtl of i2s_tx is

   constant c_cos_rom : mem_array := cos_lut;

   type t_reg_type is record
      sl_word_clock : std_logic;
      slv_temp_reg  : std_logic_vector(DATA_WIDTH-1 downto 0);
      counter    : integer range 0 to 31;
   end record;

   signal r, r_next : t_reg_type;
   signal r_i2s : t_i2s_in;

begin

  comb_proc : process(reset_n, r, r_i2s)
    variable v     : t_reg_type;
    variable v_i2s : t_i2s_in;
  begin
    v := r;

    v.counter := r.counter + 1;

    -- toggle word clock when 32 have been clocked in
    if r.counter = 31 then
      v.sl_word_clock := not r.sl_word_clock; -- toggle word clock
      v.counter := 0;                         -- reset counter

      -- latch data to temporariy register when 32 bit have been counted
      if r.sl_word_clock = '0' then
        v.slv_temp_reg := v_i2s.slv_l_channel;
      else
        v.slv_temp_reg := v_i2s.slv_r_channel;
      end if;
    end if;

    -- shift data to output
    v.slv_temp_reg(23 downto 1) := r.slv_temp_reg(22 downto 0);

    if reset_n = '0' then
      v.sl_word_clock := '0'; -- 0=left, 1=right
      v.counter := 0;
    end if;

    r_next <= v;

    r_i2s_out.sl_sdata <= r.slv_temp_reg(23);
    r_i2s_out.sl_wclk <= r.sl_word_clock;
  end process comb_proc;

  seq_proc : process(mclk)
  begin
    if rising_edge(mclk) then
      r <= r_next;
    end if;
  end process seq_proc;

  r_i2s_out.sl_bclk <= mclk;

end rtl;
