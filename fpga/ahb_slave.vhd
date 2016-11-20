------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geißler
--
-- Create Date:      23:40:00 11/19/2016
-- Design Name:
-- Project Name:     red-diamond
-- Target Device:    EP4CE22C8N
-- Tool Versions:    16.0
-- Description:      AHB slave interface
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

use work.amba.all

entity ahb_slave is
  port(
    hclk     : in std_ulogic;
    hreset_n : in std_ulogic;
    ahb_in   : in t_ahb_slave_in;
    ahb_out  : out t_ahb_slave_out
  );
end entity

architecture rtl of ahb_slave is

  type t_ahb_state is (ADDRESS, DATA, FINISH);
  AHBSTATE : t_ahb_state;

  type t_register record is
    hwrite    : std_ulogic;
    haddr     : std_logic_vector(31 downto 0);
    hsize     : std_logic_vector(1 downto 0);
    hresp     : std_logic_vector(1 downto 0);
    hreadyout : std_ulogic;
    state     : t_ahb_state;    
  end record

  --
  r, r_next : t_register;

begin

  -- Cominatorical process
  comb_proc : process(ahb_in, r, hreset_n)
    v : t_register;
  begin
    v := r;

    case AHBSTATE is
      when ADDRESS => -- Address state
        if ahb_in.hready = '1' and ahb_in.hsel = '1' and ahb_in.htrans(1)='1' then
          v.haddr    := ahb_in.haddr;  -- store address
          v.hwrite   := ahb_in.hwrite; -- store write
          v.readyout := '0';
          v.state    := DATA;
        end if;
      when DATA => -- Data state
        if r.hwrite = '1' then
          ahb_write_data();
          
        else
          ahb_read_word();
          v.readyout := 1;
        end if;
      when FINISH =>
      when others =>
        null;
    end case;

  if (hreset_n = '0') then
    v.state := ADDRESS;
  end if;

  r <= v;
  ahb_out.readyout <= v.readyout;
 
  end process comb_proc;

end rtl;
