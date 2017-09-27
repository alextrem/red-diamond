------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geissler
--
-- Create Date:      23:40:00 11/19/2016
-- Design Name:
-- Project Name:     red-diamond
-- Target Device:    EP4CE22C8N
-- Tool Versions:    16.0
-- Description:      This is the package for the AXI interfaces.
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
-- Revision 0.2 - Changed indentation
-- Revision 0.3 - Updated package name
--                Added constants, subprograms and interface definition
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package axi is

------------------------------------------------------------------------------
-- Constants
------------------------------------------------------------------------------
  constant cslv_axsize_1   : std_logic_vector(2 downto 0) := "000";
  constant cslv_axsize_2   : std_logic_vector(2 downto 0) := "001";
  constant cslv_axsize_4   : std_logic_vector(2 downto 0) := "010";
  constant cslv_axsize_8   : std_logic_vector(2 downto 0) := "011";
  constant cslv_axsize_16  : std_logic_vector(2 downto 0) := "100";
  constant cslv_axsize_32  : std_logic_vector(2 downto 0) := "101";
  constant cslv_axsize_64  : std_logic_vector(2 downto 0) := "110";
  constant cslv_axsize_128 : std_logic_vector(2 downto 0) := "111";

  constant cslv_axburst_fixed : std_logic_vector(1 downto 0) := "00";
  constant cslv_axburst_incr  : std_logic_vector(1 downto 0) := "01";
  constant cslv_axburst_wrap  : std_logic_vector(1 downto 0) := "10";

  constant cslv_awlock_normal    : std_logic_vector(1 downto 0) := "00";
  constant cslv_awlock_exclusive : std_logic_vector(1 downto 0) := "01";
  constant cslv_awlock_locked    : std_logic_vector(1 downto 0) := "10";

  constant cslv_axcache_dev_non_buf : std_logic_vector(3 downto 0) := "0000";
  constant cslv_axcache_dev_buf     : std_logic_vector(3 downto 0) := "0001";
  constant cslv_axcache_norm_nc_nb  : std_logic_vector(3 downto 0) := "0010";
  constant cslv_axcache_norm_nc_b   : std_logic_vector(3 downto 0) := "0010";
  constant cslv_arcache_wt_no_alloc : std_logic_vector(3 downto 0) := "0011";
  constant cslv_awcache_wt_no_alloc : std_logic_vector(3 downto 0) := "0110";
  constant cslv_arcache_wt_r_alloc  : std_logic_vector(3 downto 0) := "1110";
  constant cslv_awcache_wt_r_alloc  : std_logic_vector(3 downto 0) := "0110";
  constant cslv_arcache_wt_w_alloc  : std_logic_vector(3 downto 0) := "1010";
  constant cslv_awcache_wt_w_alloc  : std_logic_vector(3 downto 0) := "1110";
  constant cslv_axcache_wtr_w_alloc : std_logic_vector(3 downto 0) := "1110";
  constant cslv_arcache_wb_no_alloc : std_logic_vector(3 downto 0) := "1011";
  constant cslv_awcache_wb_no_alloc : std_logic_vector(3 downto 0) := "0111";
  constant cslv_arcache_wb_r_alloc  : std_logic_vector(3 downto 0) := "1111";
  constant cslv_awcache_wb_r_alloc  : std_logic_vector(3 downto 0) := "0111";

  constant cslv_axprot_u_access  : std_logic_vector(2 downto 0) := "000";
  constant cslv_axprot_p_access  : std_logic_vector(2 downto 0) := "001";
  constant cslv_axprot_s_access  : std_logic_vector(2 downto 0) := "010";
  constant cslv_axprot_ns_access : std_logic_vector(2 downto 0) := "011";
  constant cslv_axprot_d_access  : std_logic_vector(2 downto 0) := "100";
  constant cslv_axprot_i_access  : std_logic_vector(2 downto 0) := "100";

  constant cslv_xresp_okay   : std_logic_vector(1 downto 0) := "00";
  constant cslv_xresp_exokay : std_logic_vector(1 downto 0) := "01";
  constant cslv_xresp_slverr : std_logic_vector(1 downto 0) := "10";
  constant cslv_xresp_decerr : std_logic_vector(1 downto 0) := "11";


------------------------------------------------------------------------------
-- AXI-4 Global Signals
------------------------------------------------------------------------------

   type t_axi_wa_slv_in is record
      slv_awid    : std_logic_vector(3 downto 0);
      slv_awaddr  : std_logic_vector(31 downto 0);
      slv_awlen   : std_logic_vector(7 downto 0);
      slv_awsize  : std_logic_vector(2 downto 0);
      slv_awburst : std_logic_vector(1 downto 0);
      slv_awlock  : std_logic_vector(1 downto 0);
      slv_awcache : std_logic_Vector(3 downto 0);
      slv_awprot  : std_logic_vector(2 downto 0);
      sl_awvalid : std_ulogic;
   end record;

   type t_axi_wa_slv_out is record
      sl_awready : std_ulogic;
   end record;

   type t_axi_wd_slv_in is record
      slv_wdata : std_logic_vector(31 downto 0);
      slv_wstrb : std_logic_vector(3 downto 0);
      sl_wlast  : std_ulogic;
      sl_wvalid : std_ulogic;
   end record;

   type t_axi_wd_slv_out is record
      sl_wready : std_ulogic;
   end record;

   type t_axi_wr_slv_in is record
      sl_bready : std_ulogic;
   end record;

   type t_axi_wr_slv_out is record
      sl_bid    : std_ulogic;
      sl_bresp  : std_ulogic;
      sl_bvalid : std_ulogic;
   end record;

   type t_axi4_wd_slv_in is record
      r_axi    : t_axi_wd_slv_in;
      sl_wuser : std_ulogic;
   end record;

   type t_axi_ra_slv_in is record
      slv_arid     : std_logic_vector(1 downto 0);
      slv_araddr   : std_logic_vector(31 downto 0);
      slv_arlen    : std_logic_vector(7 downto 0);
      slv_arsize   : std_logic_vector(2 downto 0);
      slv_arburst  : std_logic_vector(1 downto 0);
      sl_arlock    : std_logic;
      slv_arcache  : std_logic_vector(3 downto 0);
      slv_arprot   : std_logic_vector(2 downto 0);
      slv_arqos    : std_logic_vector(3 downto 0);
      slv_arregion : std_logic_vector(3 downto 0);
      sl_aruser    : std_logic;
      sl_arvalid   : std_logic;
   end record;

   type t_axi_ra_slv_out is record
      sl_arready : std_ulogic;
   end record;

   type t_axi_rd_slv_in is record
      sl_rready : std_logic;
   end record;

   type t_axi_rd_slv_out is record
      sl_rid    : std_logic;
      slv_rdata : std_logic_vector(31 downto 0);
      slv_rresp : std_logic_vector(1 downto 0);
      sl_rlast  : std_logic;
      sl_ruser  : std_logic;
      sl_rvalid : std_logic;
   end record;

   type t_axi_lpi_slv_in is record
      sl_csysreq : std_ulogic;
   end record;

   type t_axi_lpi_slv_out is record
      sl_csysack : std_ulogic;
      sl_cactive : std_ulogic;
   end record;

------------------------------------------------------------------------------
-- Sub-programs
------------------------------------------------------------------------------

   --function axi_device_reg()
   --return std_logic_vector;
   function axireadword (
      rdata : std_logic_vector(31 downto 0);
      raddr : std_logic_vector(4 downto 0))
   return std_logic_vector;

   procedure axireadword (
      rdata : in std_logic_vector(31 downto 0);
      raddr : in std_logic_vector(4 downto 0);
      data  : out std_logic_vector(31 downto 0));

------------------------------------------------------------------------------
-- Components
------------------------------------------------------------------------------

   component axi3_slave
      port (
         -- global signals
         sl_aclk    : in std_ulogic;
         sl_aresetn : in std_ulogic;
         -- write address channel
         -- write data channel
         -- write response channel
         -- read data channel
         -- low power interface
         r_lpi_in  : in t_axi_lpi_slv_in;
         r_lpi_out : out t_axi_lpi_slv_out
      );
   end component;

   component axi4_slave
      port (
         -- global signals
         sl_aclk     : in std_ulogic;
         sl_areset_n : in std_ulogic;
         -- write address channel
         r_wac_in   : in t_axi_wa_slv_in;
         awqos     : in std_logic_vector(3 downto 0);
         awregion  : in std_logic_vector(3 downto 0);
         awuser    : in std_ulogic;
         r_wac_out : out t_axi_wa_slv_out;
         -- write data channel
         r_wdc_in   : in t_axi4_wd_slv_in;
         r_wdc_out  : out t_axi_wd_slv_out;
         -- write response channel
         r_wrc_in   : in t_axi_wr_slv_in;
         r_wrc_out  : out t_axi_wr_slv_out;
         -- read address channel
         r_rac_in   : in t_axi_ra_slv_in;
         r_rac_out  : out t_axi_ra_slv_out;
         -- read data channel
         r_rdc_in   : in t_axi_rd_slv_in;
         r_rdc_out  : out t_axi_rd_slv_out;
         -- low power interface
         r_lpi_in   : in t_axi_lpi_slv_in;
         r_lpi_out  : out t_axi_lpi_slv_out
      );
   end component;

   component axi4_light_slave
      port (
         -- global signals
         sl_aclk     : in std_ulogic;
         sl_areset_n : in std_ulogic
      );
   end component;

end axi;

package body axi is

   function axireadword (
      rdata : std_logic_vector(31 downto 0);
      raddr : std_logic_vector(4 downto 0))
      return std_logic_vector is
   begin
   end axireadword;

   procedure axireadword (
      rdata : in std_logic_vector(31 downto 0);
      raddr : in std_logic_vector(4 downto 0);
      data  : out std_logic_vector(31 downto 0)) is
   begin
   end axireadword;

end;
