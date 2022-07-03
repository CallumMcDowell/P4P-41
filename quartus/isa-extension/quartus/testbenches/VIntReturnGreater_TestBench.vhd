-- P4P 41 - CNN ISA Extension
-- 07/22 - cmcd407@auckland.ac.nz

library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

library work;
use work.CNNISATypes.all;

entity VIntReturnGreater_TestBench is
    port (
        placeholder     : out std_logic
    );
end entity VIntReturnGreater_TestBench;


architecture test of VIntReturnGreater_TestBench is
    constant WORD_MAX_SIGNED_INT    : integer := to_integer(WORD_MAX_SIGNED);

    signal vreg_in, vreg_out    : vreg  := (others => (others => '0'));
    signal lo                   : word  := (others => '0');
    
begin
    
    UNIT    : entity work.VIntReturnGreater
        port map (
            -- inputs
            operands => vreg_in,
            lo => lo,
            -- outputs
            outputs => vreg_out
        );

    UNITTEST: process
        variable a              : vreg;
    begin
        report "VIntReturnGreater: Test START";

        for i in 0 to a'length-1 loop
            a(i) := REAL_TO_WORD(real(i) - 0.5 * real(a'length));
        end loop;
        
        vreg_in <= a;
        lo <= REAL_TO_WORD(0.0);

        wait for 10 ns; -- propagation delay

        for i in 0 to vreg_out'length-1 loop
            if vreg_out(i) > lo then
                report "VIntReturnGreater: Failed (asserted " & integer'image(to_integer(vreg_out(i)))
                    & " > " & integer'image(to_integer(lo)) & ")"
                severity error;
            end if;
        end loop;

        wait for 10 ns; -- propagation delay

        vreg_in <= (others => to_signed(-1, word'length));
        lo <= to_signed(5, word'length), to_signed(6, word'length) after 10 ns;

        wait;
    end process UNITTEST;
    
end architecture test;