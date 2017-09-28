------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geißler
--
-- Create Date:      23:40:00 11/19/2016
-- Design Name:
-- Project Name:     red-diamond
-- Target Device:    EP4CE22C8N
-- Tool Versions:    16.0
-- Description:      This AES3/EBU and SPDIF receiver is compliant with
--                   IEC61937, IEC60958-3 and IEC60958-4
--                   This package contains constants and type definitions
--                   for the receiver
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
-- Revision 0.2 - Changed indentation
------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package amba is

------------------------------------------------------------------------------
-- Constants
------------------------------------------------------------------------------

  constant AHBDW         : integer := 32;

  constant HTRANS_IDLE   : std_logic_vector(1 downto 0) := "00";
  constant HTRANS_BUSY   : std_logic_vector(1 downto 0) := "01";
  constant HTRANS_NONSEQ : std_logic_vector(1 downto 0) := "10";
  constant HTRANS_SEQ    : std_logic_vector(1 downto 0) := "11";

  constant HBURST_SINGLE : std_logic_vector(2 downto 0) := "000";
  constant HBURST_INCR   : std_logic_vector(2 downto 0) := "001";
  constant HBURST_WRAP4  : std_logic_vector(2 downto 0) := "010";
  constant HBURST_INCR4  : std_logic_vector(2 downto 0) := "011";
  constant HBURST_WRAP8  : std_logic_vector(2 downto 0) := "100";
  constant HBURST_INCR8  : std_logic_vector(2 downto 0) := "101";
  constant HBURST_WRAP16 : std_logic_vector(2 downto 0) := "110";
  constant HBURST_INCR16 : std_logic_vector(2 downto 0) := "111";

  constant HSIZE_BYTE    : std_logic_vector(2 downto 0) := "000";
  constant HSIZE_HWORD   : std_logic_vector(2 downto 0) := "001";
  constant HSIZE_WORD    : std_logic_vector(2 downto 0) := "010";
  constant HSIZE_DWORD   : std_logic_vector(2 downto 0) := "011";
  constant HSIZE_4WORD   : std_logic_vector(2 downto 0) := "100";
  constant HSIZE_8WORD   : std_logic_vector(2 downto 0) := "101";
  constant HSIZE_16WORD  : std_logic_vector(2 downto 0) := "110";
  constant HSIZE_32WORD  : std_logic_vector(2 downto 0) := "111";

  constant HRESP_OKAY    : std_logic_vector(1 downto 0) := "00";
  constant HRESP_ERROR   : std_logic_vector(1 downto 0) := "01";
  constant HRESP_RETRY   : std_logic_vector(1 downto 0) := "10";
  constant HRESP_SPLIT   : std_logic_vector(1 downto 0) := "11";

------------------------------------------------------------------------------
-- AHB slave inputs
------------------------------------------------------------------------------

  type t_ahb_slave_in is record
    hsel      : std_ulogic;
    haddr     : std_logic_vector(31 downto 0);
    hwrite    : std_ulogic;
    htrans    : std_logic_vector(1 downto 0);
    hsize     : std_logic_vector(2 downto 0);
    hburst    : std_logic_vector(3 downto 0);
    hwdata    : std_logic_vector(31 downto 0);
    hprot     : std_logic_vector(3 downto 0);
    hready    : std_ulogic;
    hmastlock : std_ulogic;
  end record;

  -- AHB slave outputs
  type t_ahb_slave_out is record
    hreadyout : std_ulogic;
    hresp     : std_logic_vector(1 downto 0);
    hrdata    : std_logic_vector(31 downto 0);
  end record;

  type t_apb3_slave_in is record
    psel  : std_ulogic;
    paddr : std_logic_vector(31 downto 0);
  end record;

------------------------------------------------------------------------------
-- functions and procedures
------------------------------------------------------------------------------

--  function ahb_write_word (
--    hdata : std_logic_vector(AHBDW-1 downto 0);
--    haddr : std_logic_vector(4 downto 2))
--    return std_logic_vector;

  procedure ahb_write_data (
    haddr : std_logic_vector(4 downto 2);
    hdata : std_logic_vector(AHBDW-1 downto 0));

  function ahb_read_word (
    hdata : std_logic_vector(AHBDW-1 downto 0);
    haddr : std_logic_vector(4 downto 2))
    return std_logic_vector;

  procedure ahb_read_word (
    hdata : in std_logic_vector(AHBDW-1 downto 0);
    haddr : in std_logic_vector(4 downto 2);
    data  : out std_logic_vector(AHBDW-1 downto 0));

------------------------------------------------------------------------------
-- Components
------------------------------------------------------------------------------

  component ahb_slave
    port (
      hclk     : in std_ulogic;
      hreset_n : in std_ulogic;
      ahb_in   : in t_ahb_slave_in;
      ahb_out  : out t_ahb_slave_out
    );
  end component;

end amba;

package body amba is

  procedure ahb_write_data(
    haddr : in std_logic_vector(4 downto 2);
    hdata : in std_logic_vector(AHBDW-1 downto 0)) is
  begin
  end ahb_write_data;

  function ahb_read_word (
    hdata : std_logic_vector(AHBDW-1 downto 0);
    haddr : std_logic_vector(4 downto 2))
    return std_logic_vector is
    variable data : std_logic_vector(AHBDW-1 downto 0);
  begin
    data := hdata;
    return data;
  end ahb_read_word;

  procedure ahb_read_word (
    hdata : in std_logic_vector(AHBDW-1 downto 0);
    haddr : in std_logic_vector(4 downto 2);
    data  : out std_logic_vector(AHBDW-1 downto 0)) is
  begin
    data := ahb_read_word(hdata, haddr);
  end ahb_read_word;

end;
