------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geissler
--
-- Create Date:      23:40:00 11/19/2016
-- Design Name:
-- Project Name:     red-diamond
-- Target Device:    EP4CE22C8N
-- Tool Versions:    16.0
-- Description:      This is the package for the AXI4 interfaces.
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

package axi4 is

------------------------------------------------------------------------------
-- Constants
------------------------------------------------------------------------------
  constant AWSIZE_1   : std_logic_vector(2 downto 0) := "000";
  constant AWSIZE_2   : std_logic_vector(2 downto 0) := "001";
  constant AWSIZE_4   : std_logic_vector(2 downto 0) := "010";
  constant AWSIZE_8   : std_logic_vector(2 downto 0) := "011";
  constant AWSIZE_16  : std_logic_vector(2 downto 0) := "100";
  constant AWSIZE_32  : std_logic_vector(2 downto 0) := "101";
  constant AWSIZE_64  : std_logic_vector(2 downto 0) := "110";
  constant AWSIZE_128 : std_logic_vector(2 downto 0) := "111";

  constant AWBURST_FIXED : std_logic_vector(1 downto 0) := "00";
  constant AWBURST_INCR  : std_logic_vector(1 downto 0) := "01";
  constant AWBURST_WRAP  : std_logic_vector(1 downto 0) := "10";

  constant AWLOCK_NORMAL    : std_logic_vector(1 downto 0) := "00";
  constant AWLOCK_EXCLUSIVE : std_logic_vector(1 downto 0) := "01";
  constant AWLOCK_LOCKED    : std_logic_vector(1 downto 0) := "10";
------------------------------------------------------------------------------
-- AXI-4 Global Signals
------------------------------------------------------------------------------
  type t_axi_write_address_channel_in is record
      awid    : std_logic_vector(3 downto 0);
      awaddr  : std_logic_vector(31 downto 0);
      awlen   : std_logic_vector(7 downto 0);
      awsize  : std_logic_vector(2 downto 0);
      awburst : std_logic_vector(1 downto 0);
      awlock  : std_logic_vector(1 downto 0);
      awcache : std_logic_Vector(3 downto 0);
      awprot  : std_logic_vector(2 downto 0);
      awvalid : std_ulogic;
  end record;

  type t_axi_write_data_channel_in is record
      wdata    : std_logic_vector(31 downto 0);
      wstrb    : std_logic_vector(3 downto 0);
      wlast    : std_ulogic;
      wvalid   : std_ulogic;
  end record;

  type t_axi_write_response_channel_in is record
      bready : std_ulogic;
  end record;

  type t_axi_write_response_channel_out is record
      bid    : std_ulogic;
      bresp  : std_ulogic;
      bvalid : std_ulogic;
  end record;

  type t_axi4_write_data_channel_in is record
      axi   : t_axi_write_data_channel_in;
      wuser : std_ulogic;
  end record;

  component axi4_slave
      port (
        aclk     : in std_ulogic;
        areset_n : in std_ulogic;
        -- write address channel
        axi4_in  : in t_axi_write_address_channel_in;
        awqos    : in std_logic_vector(3 downto 0);
        awregion : in std_logic_vector(3 downto 0);
        awuser   : in std_ulogic;
        -- write data channel
        wdc_in   : in t_axi4_write_data_channel_in;
        -- write response channel
        wrc_in   : in t_axi_write_response_channel_in;
        wrc_out  : out t_axi_write_response_channel_out
        -- read address channel
        -- read data channel
        -- low power interface
      );
  end component;

end axi4;
