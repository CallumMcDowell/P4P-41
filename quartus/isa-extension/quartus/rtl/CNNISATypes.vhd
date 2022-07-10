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
 
end package CNNISATypes;

package body CNNISATypes is
        
end package body CNNISATypes;