library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

library work;
use work.CNNISATypes.all;

entity Clip is
    generic (
        size        : positive  := 8
    );
    port (
        -- inputs
        operand     : in signed(size-1 downto 0);
        lo          : in signed(size-1 downto 0);
        -- outputs
        output      : out signed(size-1 downto 0)
    );
end entity Clip;

architecture rtl of clip is
    
begin
    
    output <= operand when (operand > lo) else lo;
    
end architecture rtl;