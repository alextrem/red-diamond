---------------------------------------
-- Company:
-- Engineer:        Alexander Geißler
--
-- Create Date:     23:40:00 02/26/2015
-- Design Name:
-- Project Name:    red-diamond
-- Target Device:   EP4CE22C8N
-- Tool Versions:   14.0
-- Description:
--
-- Dependencies:
--
-- Revision:
-- Revision 0.1 - File created
--------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

package spdif_pkg is
    -- statemachine type
    type aes3_statemachine_type is (unlocked, confirming, locked);

    constant X_PREAMBLE : std_logic_vector(7 downto 0) := "01000111";
    constant Y_PREAMBLE : std_logic_vector(7 downto 0) := "00100111";
    constant Z_PREAMBLE : std_logic_vector(7 downto 0) := "00010111";

    -- AES3/EBU or SPDIF stream constants
    constant FRAMES : integer := 192;
    subtype FRAME_WIDTH is integer range FRAMES-1 downto 0;
    constant CHANNELS : integer := 2;
    constant SUBFRAME : integer := 32;

    -- channel status description
    type byte_0 is record
        channel_use : std_logic;
        pcm         : std_logic;
        preemphasis : std_logic_vector(2 downto 0);
        --
        sample_frequency : std_logic_vector(1 downto 0);
    end record byte_0;

    type byte_1 is record
        channel_mode    : std_logic_vector(3 downto 0);
        user_bit        : std_logic_vector(3 downto 0);
    end record byte_1;

    type byte_2 is record
        aux_sample_bits     : std_logic_vector(2 downto 0);
        sample_word_length  : std_logic_vector(2 downto 0);
        alignment_level     : std_logic_vector(1 downto 0);
    end record byte_2;

    type byte_3 is record
        multichannel_mode   : std_logic_vector(6 downto 0);
        channel_number      : std_logic;
    end record byte_3;

    type byte_4 is record
        reference_signal    : std_logic_vector(1 downto 0);
        reserved            : std_logic;
        sampling_frequency  : std_logic_vector(3 downto 0);
        frequency_scaling   : std_logic;
    end record byte_4;

    --type byte_6 is record
        --channel_origin : string (0 to 3);
    --end record byte_6;

    --type byte_10 is record
        --channel_destination : string (0 to 3);
    --end record byte_10;

    type channel_status is record
        reg_0 : byte_0;
        reg_1 : byte_1;
        reg_2 : byte_2;
        reg_3 : byte_3;
        reg_4 : byte_4;
        --origin : byte_6;
        --destination : byte_10;
        crc : std_logic_vector(7 downto 0);
    end record channel_status;

    -- RX data will be 5 times oversampled
    -- 192000 Hz : 24,576  MHz
    -- 176400 Hz : 22,5792 MHz
    --  96000 Hz : 12,288  MHz
    --  88200 Hz : 11,2896 MHz
    --  48000 Hz :  6,144  MHz
    --  44100 Hz :  5,6448 MHz
    --  32000 Hz :  4,096  MHz

end spdif_pkg;
