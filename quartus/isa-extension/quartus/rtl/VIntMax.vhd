library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

library work;
use work.CNNISATypes.all;

entity VIntMax is
    generic (
        element_length : positive  := ELEN
    );
    port (
        -- inputs
        operands        : in vreg;
        lo              : in signed(element_length-1 downto 0);
        -- outputs
        outputs         : out vreg
    );
end entity VIntMax;

architecture rtl of VIntMax is
    
begin

    GEN_LOGIC: for i in vreg'length/element_length downto 1 generate
        constant upper : integer := i * element_length;
        constant lower : integer := upper - element_length;
    begin
        UNIT: entity work.IntMax
            generic map (
                size => element_length
            )
            port map (
                -- inputs
                a => signed(operands(upper-1 downto lower)),
                b => lo,
                -- outputs
                std_logic_vector(output) => outputs(upper-1 downto lower)
            );
    end generate GEN_LOGIC;
    
end architecture rtl;