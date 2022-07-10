library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

package CNNISATypes is

    constant    ELEN                : positive      := 8;
        -- Number of bits in an element.
        -- Must be a power of 2 or multiple of 8. VLEN must be a multiple of ELEN.
        -- To deprecate when support for dynamic word length is added.
    constant    VLEN                : positive      := ELEN * 8;
        -- Number of bits in a vector register.
        -- Must be a power of 2 or multiple of 8.

    subtype        vreg             is std_logic_vector(VLEN-1 downto 0);
        -- Vector register.



    type        element_width is    (EIGHT, SIXTEEN, THIRTY_TWO);

end package CNNISATypes;


package body CNNISATypes is
        
end package body CNNISATypes;