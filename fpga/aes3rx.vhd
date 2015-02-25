library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity aes3rx is
    port (
        -- Synchronous reset
        reset   : in std_logic;
        -- Master clock
        clk     : in std_logic;
        aes3    : in std_logic;
        active  : out std_logic;
         );
end aes3rx;
