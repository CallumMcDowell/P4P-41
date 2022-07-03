library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

package CNNISATypes is
    constant    BITS        : positive      := 8;
        -- placeholder, use bytes if supporting variable types (int8-32, fxp, fp)
    constant    WORDS       : positive      := 8;
        -- placeholder, use vector width if supporting variable types

    subtype     word is     signed(BITS-1 downto 0);
    type        vreg is     array (WORDS-1 downto 0) of word;


    -- Constants
    constant WORD_MAX_SIGNED    : word := (word'LEFT => '0', others => '1');
    constant WORD_MAX_UNSIGNED  : word := (others => '1');

    -- Functions
    function REAL_TO_WORD(x : real) return word;
    
end package CNNISATypes;

package body CNNISATypes is

    function REAL_TO_WORD(x : real) return word is
    begin
        return to_signed(integer(round(x)), word'length);
        -- WARNING: ASSUMES WORD IS SIGNED! Add precompiler case statement?
    end REAL_TO_WORD;
        
end package body CNNISATypes;