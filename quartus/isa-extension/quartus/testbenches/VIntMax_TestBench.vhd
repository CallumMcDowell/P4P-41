-- P4P 41 - CNN ISA Extension
-- 07/22 - cmcd407@auckland.ac.nz

library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

library work;
use work.CNNISATypes.all;
use work.CNNISATestbenchTypes.all;

entity VIntMax_TestBench is
    port (
        placeholder     : out std_logic
    );
end entity VIntMax_TestBench;


architecture test of VIntMax_TestBench is
    constant ELEMENT_MAX_SIGNED_INT    : integer := to_integer(ELEMENT_MAX_SIGNED);

    signal vreg_in, vreg_out    : vreg  := (others => '0');
    signal vreg_out_disp        : elements := (others => (others => '0'));
    signal lo                   : signed(ELEN-1 downto 0)  := (others => '0');
    
begin

    UNIT: entity work.VIntMax
        port map (
            -- inputs
            operands => vreg_in,
            lo => lo,
            -- outputs
            outputs => vreg_out
        );

    UNITTEST: process
        variable upper, lower : integer;

        variable a              : vreg;
        variable errors         : integer := 0;
    begin
        report "VIntMax: Test START";

        for i in vreg'length/ELEN downto 1 loop
            upper := i * ELEN;
            lower := upper - ELEN;
        
            a(upper-1 downto lower) := std_logic_vector(REAL_TO_WORD(real(i) - 0.5 * real(VLEN/ELEN)));
        end loop;
        
        vreg_in <= a;
        lo <= REAL_TO_WORD(0.0);

        wait for 10 ns; -- propagation delay

        for i in vreg_out'length/ELEN downto 1 loop
            upper := i * ELEN;
            lower := upper - ELEN;

            if signed(vreg_out(upper-1 downto lower)) < lo then
                report "VIntMax: Failed (asserted " & integer'image(to_integer(signed(vreg_out(upper-1 downto lower))))
                    & " > " & integer'image(to_integer(lo)) & ")"
                severity error;

                errors := errors + 1;
            end if;
        end loop;

        wait for 10 ns; -- propagation delay


        vreg_in <= (others => '0');
        lo <= to_signed(5, lo'length), to_signed(6, lo'length) after 10 ns;

        -- TODO: add error checking for above, and add edgecases


        if errors > 0 then
            report "VIntMax: Test FAILED (" & integer'image(errors) & " errors)";
        else
            report "VIntMax: Test COMPLETE";
        end if;
        wait;
    end process UNITTEST;

    vreg_out_disp <= VREG_TO_ELEMENTS(vreg_out);
    
end architecture test;