library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

library work;
use work.CNNISATypes.all;

entity VClip is
    port (
        -- inputs
        operands        : in vreg;
        lo              : in word;
        -- outputs
        outputs         : out vreg
    );
end entity VClip;

architecture rtl of VClip is
    
begin

    GEN_LOGIC: for w in 0 to operands'length-1 generate

    begin
        clip: entity work.Clip
            generic map (
                size => word'length
            )
            port map (
                -- inputs
                operand => operands(w),
                lo => lo,
                -- outputs
                output => outputs(w)
            );
    end generate GEN_LOGIC;
    
end architecture rtl;