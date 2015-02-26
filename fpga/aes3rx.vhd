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

    signal sl_preamble_detected : std_logic := '0'; -- Asserted when preamble has been detected
    signal sl_x_detected        : std_logic := '0'; -- Asserted when x preamble has been detected
    signal sl_y_detected        : std_logic := '0'; -- Asserted when y preamble has been detected
    signal sl_z_detected        : std_logic := '0'; -- Asserted when z preamble has been detected

begin

end rtl;
