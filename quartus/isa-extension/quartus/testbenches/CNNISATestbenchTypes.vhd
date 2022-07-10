library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

library work;
use work.CNNISATypes.all;

package CNNISATestbenchTypes is
    -- Constants
    constant WORD_MAX_SIGNED    : word := (word'LEFT => '0', others => '1');
    constant WORD_MAX_UNSIGNED  : word := (others => '1');

    -- Functions
    function REAL_TO_WORD(x : real) return word;
    
end package CNNISATestbenchTypes;

package body CNNISATestbenchTypes is

    function REAL_TO_WORD(x : real) return word is
    begin
        return to_signed(integer(round(x)), word'length);
        -- WARNING: ASSUMES WORD IS SIGNED! Add precompiler case statement?
    end REAL_TO_WORD;
        
end package body CNNISATestbenchTypes;