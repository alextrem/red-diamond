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
        -- aes/ebu, spdif compatible stream
        aes3    : in std_logic;
        -- receiver has valid input data
        active  : out std_logic := 0;
         );
end aes3rx;

architecture rtl of aes3rx is
begin

end rtl;
