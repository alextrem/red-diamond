------------------------------------------------------------------------------
-- Company:          Red Diamond
-- Engineer:         Alexander Geissler
--
-- Create Date:      23:40:00 02/26/2015
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

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

package spdif_pkg is
  -- statemachine type
  type t_aes3_state is (UNLOCKED, CONFIRMING, LOCKED);

  type t_aes_in is record
    data       : std_logic;
    --rx_full    : std_ulogic;
    --rx_fifo_de : std_ulogic;
  end record;

  type t_aes_out is record
    lock         : std_logic;
    frame_clock  : std_logic;
    --i2s_out      : std_ulogic;
    --rx_de        : std_ulogic;
    --rx_fifo_full : std_ulogic;
    --rx_data      : std_logic_vector(31 downto 0);
  end record;

-------------------------------------------------------------------------------
-- Constants
-------------------------------------------------------------------------------

  constant X_PREAMBLE : std_logic_vector(7 downto 0) := "01000111";
  constant Y_PREAMBLE : std_logic_vector(7 downto 0) := "00100111";
  constant Z_PREAMBLE : std_logic_vector(7 downto 0) := "00010111";

  -- AES3/EBU or SPDIF stream constants
  constant FRAMES : integer := 192;
  subtype FRAME_WIDTH is integer range FRAMES-1 downto 0;
  constant CHANNELS : integer := 2;
  constant SUBFRAME : integer := 32;

  -- channel status description
  type t_byte_0 is record
    channel_use : std_logic;                         -- professional or consumer
    pcm         : std_logic;                         -- PCM indication
    preemphasis : std_logic_vector(2 downto 0);      -- signal pre-emphasis
    lock        : std_logic;                         -- lock indication
    sample_frequency : std_logic_vector(1 downto 0); -- encoded sampling freq.
  end record t_byte_0;

  type t_byte_1 is record
    channel_mode : std_logic_vector(3 downto 0);
    user_bit     : std_logic_vector(3 downto 0);
  end record t_byte_1;

  type t_byte_2 is record
    aux_sample_bits    : std_logic_vector(2 downto 0);
    sample_word_length : std_logic_vector(2 downto 0);
    alignment_level    : std_logic_vector(1 downto 0);
  end record t_byte_2;

  type t_byte_3 is record
    multichannel_mode   : std_logic_vector(6 downto 0);
    channel_number      : std_logic;
  end record t_byte_3;

  type t_byte_4 is record
    reference_signal    : std_logic_vector(1 downto 0);
    reserved            : std_logic;
    sampling_frequency  : std_logic_vector(3 downto 0);
    frequency_scaling   : std_logic;
  end record t_byte_4;

  type t_byte_5 is record
    reserved : std_logic_vector(7 downto 0);
  end record;

  type t_byte_6_to_9 is record
    channel_origin_1 : std_logic_vector(7 downto 0);
    channel_origin_2 : std_logic_vector(7 downto 0);
    channel_origin_3 : std_logic_vector(7 downto 0);
  end record t_byte_6_to_9;

  type t_byte_10_to_13 is record
    channel_destination_1 : std_logic_vector(7 downto 0);
    channel_destination_2 : std_logic_vector(7 downto 0);
    channel_destination_3 : std_logic_vector(7 downto 0);
  end record t_byte_10_to_13;

  type t_byte_14_to_17 is record
    lsac_0 : std_logic_vector(7 downto 0); -- local sample address code
    lsac_1 : std_logic_vector(7 downto 0); -- local sample address code
    lsac_2 : std_logic_vector(7 downto 0); -- local sample address code
    lsac_3 : std_logic_vector(7 downto 0); -- local sample address code
  end record t_byte_14_to_17;

  type t_byte_18_to_21 is record
    tsc_0 : std_logic_vector(7 downto 0); -- time of day sample code
    tsc_1 : std_logic_vector(7 downto 0); -- time of day sample code
    tsc_2 : std_logic_vector(7 downto 0); -- time of day sample code
    tsc_3 : std_logic_vector(7 downto 0); -- time of day sample code
  end record t_byte_18_to_21;

  type channel_status is record
    reg_0          : t_byte_0;
    reg_1          : t_byte_1;
    reg_2          : t_byte_2;
    reg_3          : t_byte_3;
    reg_4          : t_byte_4;
    reg_5          : t_byte_5;
    origin         : t_byte_6_to_9;
    destination    : t_byte_10_to_13;
    sample_address : t_byte_14_to_17;
    time_of_day    : t_byte_18_to_21;
    reliability    : std_logic_vector(7 downto 0);
    crc            : std_logic_vector(7 downto 0);
  end record channel_status;

    -- RX data will be 8 times oversampled
    -- 192000 Hz : 24,576  MHz
    -- 176400 Hz : 22,5792 MHz
    --  96000 Hz : 12,288  MHz
    --  88200 Hz : 11,2896 MHz
    --  48000 Hz :  6,144  MHz
    --  44100 Hz :  5,6448 MHz
    --  32000 Hz :  4,096  MHz
-------------------------------------------------------------------------------
-- Functions
-------------------------------------------------------------------------------

  function preamble_detection (
    data  : std_logic_vector(7 downto 0);
    const : std_logic_vector(7 downto 0) )
  return std_logic;

  function crcc (
    data : std_logic_vector(7 downto 0);
    crc  : std_logic_vector(7 downto 0) )
  return std_logic_vector;

-------------------------------------------------------------------------------
-- Components
-------------------------------------------------------------------------------

  component aes3rx
  port (
    clk     : in std_ulogic;
    reset   : in std_ulogic;

    -- aes/ebu interface definition
    aes_in  : in t_aes_in;
    aes_out : out t_aes_out
  );
  end component;

end spdif_pkg;

package body spdif_pkg is

  function preamble_detection (
    data  : std_logic_vector(7 downto 0);
    const : std_logic_vector(7 downto 0))
  return std_logic is
    variable v : std_logic := '0';
  begin
    if data = const or data = not const then
      v := '1';
    else
      v := '0';
    end if;
    return v;
  end preamble_detection;

  function crcc (
    data : std_logic_vector(7 downto 0);
    crc  : std_logic_vector(7 downto 0))
  return std_logic_vector is
    variable d : std_logic_vector(7 downto 0);
    variable c : std_logic_vector(7 downto 0);
    variable n : std_logic_Vector(7 downto 0);
  begin
    d := data;
    c := crc;

    n(0) := d(0) xor d(4) xor d(5) xor d(6) xor c(0) xor c(4) xor c(5) xor c(6);
    n(1) := d(1) xor d(5) xor d(6) xor d(7) xor c(1) xor c(5) xor c(6) xor c(7);
    n(2) := d(0) xor d(2) xor d(4) xor d(5) xor d(7) xor c(0) xor c(2) xor c(4) xor c(5) xor c(7);
    n(3) := d(0) xor d(1) xor d(3) xor d(4) xor c(0) xor c(1) xor c(3) xor c(4);
    n(4) := d(0) xor d(1) xor d(2) xor d(6) xor c(0) xor c(1) xor c(2) xor c(6);
    n(5) := d(1) xor d(2) xor d(3) xor d(7) xor c(1) xor c(2) xor c(3) xor c(7);
    n(6) := d(2) xor d(3) xor d(4) xor c(2) xor c(3) xor c(4);
    n(7) := d(3) xor d(4) xor d(5) xor c(3) xor c(4) xor c(5);

  return n;
  end crcc;

end;
