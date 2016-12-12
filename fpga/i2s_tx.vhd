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

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.i2s_pkg.all;

entity i2s_tx is
--  generic ( DATA_WIDTH : integer range 16 to 32
--  );
  port (
    -- Synchronous reset
    reset_n : in std_ulogic;
    -- Master clock
    mclk    : in std_ulogic;
		
    -- I2S interface
    -- input
    i2s_in  : in t_i2s_in;
    -- output
    i2s_out : out t_i2s_out
  );
end entity;

architecture rtl of i2s_tx is

  type t_reg_type is record
    word_clock : std_logic;
    temp_reg   : std_logic_vector(23 downto 0);
    counter    : std_logic_vector(4 downto 0);
  end record;

  signal r, r_next : t_reg_type;

begin

  comb_proc : process(reset_n, r)
    variable v : t_reg_type;
  begin
    v := r;

    -- 
    v.counter := std_logic_vector(unsigned(v.counter) + 1);

    -- toggle word clock when 32bit have been clocked in 
    if r.counter = b"10000" then -- 32
      v.word_clock := not v.word_clock;
      v.counter := b"00000";
    end if;

    -- shift data to output
    if r.word_clock = '0' then
      v.temp_reg(23  downto 1) := i2s_in.l_channel(22 downto 0);
    else
      v.temp_reg(23 downto 1) := i2s_in.r_channel(22 downto 0);
    end if;

    if reset_n = '0' then
      v.word_clock := '0'; -- 0=left, 1=right
      v.counter := b"00000";
    end if;

    r_next <= v;

    i2s_out.sdata <= r.temp_reg(23);
    i2s_out.wclk <= r.word_clock;
  end process comb_proc;

  seq_proc : process(mclk)
  begin
    if rising_edge(mclk) then
      r <= r_next;
    end if;
  end process seq_proc;

end rtl;
