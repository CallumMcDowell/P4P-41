library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

library work;
use work.CNNISATypes.all;

entity IntMax is
    generic (
        size        : positive  := 8
    );
    port (
        -- inputs
        a           : in signed(size-1 downto 0);
        b           : in signed(size-1 downto 0);
        -- outputs
        output      : out signed(size-1 downto 0)
    );
end entity IntMax;

architecture rtl of IntMax is
    
begin
    
    output <= a when (a > b) else b;
    
end architecture rtl;