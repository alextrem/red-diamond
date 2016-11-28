------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geißler
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
use work.i2s_pkg.all;

entity i2s_tx is
  generic ( DATA_WIDTH : integer range 16 to 32
  );
  port (
    -- Synchronous reset
    reset   : in std_ulogic;
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
  counter : integer;
end record;

signal r, r_next : t_reg_type;

begin
  comb_proc : process(r)
  begin
    
  end process comb_proc;
end rtl; 
