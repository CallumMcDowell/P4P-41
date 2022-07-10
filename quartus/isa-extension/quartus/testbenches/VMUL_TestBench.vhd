-- P4P 41 - CNN ISA Extension
-- 07/22 - cmcd407@auckland.ac.nz

library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

library work;
use work.CNNISATypes.all;
use work.CNNISATestbenchTypes.all;

entity VMUL_TestBench is
    port (
        placeholder     : out std_logic        
    );
end entity VMUL_TestBench;

architecture test of VMUL_TestBench is
    signal vreg_a, vreg_b, vreg_out : vreg      := (others => '0');
    signal vreg_out_disp            : elements  := (others => (others => '0'));
begin

    UNIT: entity work.VMUL
        port map (
            a => vreg_a,
            b => vreg_b,
            output => vreg_out
        );

    UNITTEST: process
        constant TEST_NAME      : string := "VMUL";
        variable errors         : integer := 0;

        variable upper, lower   : integer;
        variable a, b           : vreg;

        type answer is record
            a : elements;
            b : elements;
            ans : elements;
        end record;
        variable expected       : answer;

        variable counter        : integer := 0;
        variable step           : integer := 1;

        procedure VALIDATE_ANSWERS(
            signal calculated : in vreg;
            variable expected : in answer;
            variable errors   : inout integer;
            constant TEST_NAME : in string
            ) is
        begin
            for i in VLEN/ELEN downto 1 loop
                upper := i * ELEN;
                lower := upper - ELEN;

                if signed(vreg_out(upper-1 downto lower)) /= expected.ans(i-1) then
                    report TEST_NAME & ": Failed (asserted "
                        & integer'image(to_integer(expected.a(i-1))) 
                        & " x " & integer'image(to_integer(expected.b(i-1))) & 
                        " = " & integer'image(to_integer(signed(vreg_out(upper-1 downto lower))))
                        & ", not " & integer'image(to_integer(expected.ans(i-1))) & ")"
                    severity error;

                    errors := errors + 1;
                end if;
            end loop;
        end VALIDATE_ANSWERS;

    begin
        -- START --
        REPORT_TEST_START(TEST_NAME);

        -- Test Positive-Positive Squares --
        -- Populate
        counter := 0;
        for i in VLEN/ELEN downto 1 loop
            upper := i * ELEN;
            lower := upper - ELEN;

            a(upper-1 downto lower) := std_logic_vector(to_signed(counter, ELEN));
            b(upper-1 downto lower) := std_logic_vector(to_signed(counter, ELEN));

            expected.a(i-1) := to_signed(counter, ELEN);
            expected.b(i-1) := to_signed(counter, ELEN);
            expected.ans(i-1) := INT_TO_WORD(REAL_TO_INT(real(counter)**2.0) rem REAL_TO_INT(2.0**real(ELEN)));
            
            counter := counter + step;
        end loop;

        vreg_a <= a;
        vreg_b <= b;

        wait for 10 ns; -- propagation delay

        -- Check output
        VALIDATE_ANSWERS(vreg_out, expected, errors, TEST_NAME);


        -- Test Positive-Negative Squares --
        -- Populate
        counter := 0;
        for i in VLEN/ELEN downto 1 loop
            upper := i * ELEN;
            lower := upper - ELEN;

            a(upper-1 downto lower) := std_logic_vector(to_signed(counter, ELEN));
            b(upper-1 downto lower) := std_logic_vector(to_signed(-1 * counter, ELEN));

            expected.a(i-1) := to_signed(counter, ELEN);
            expected.b(i-1) := to_signed(-1 * counter, ELEN);
            expected.ans(i-1) := INT_TO_WORD(REAL_TO_INT(real(counter) * real(-1 * counter)) rem REAL_TO_INT(2.0**real(ELEN)));
            
            counter := counter + step;
        end loop;

        vreg_a <= a;
        vreg_b <= b;

        wait for 10 ns; -- propagation delay

        -- Check output
        VALIDATE_ANSWERS(vreg_out, expected, errors, TEST_NAME);


        -- Test Negative-Negative Squares and Overflow --
        -- Populate
        counter := 6;
        for i in VLEN/ELEN downto 1 loop
            upper := i * ELEN;
            lower := upper - ELEN;

            a(upper-1 downto lower) := std_logic_vector(to_signed(-1 * counter, ELEN));
            b(upper-1 downto lower) := std_logic_vector(to_signed(-1 * counter, ELEN));

            expected.a(i-1) := to_signed(-1 * counter, ELEN);
            expected.b(i-1) := to_signed(-1 * counter, ELEN);
            expected.ans(i-1) := INT_TO_WORD(REAL_TO_INT(real(-1 * counter) * real(-1 * counter)) rem REAL_TO_INT(2.0**real(ELEN)));
            
            counter := counter + step;
        end loop;

        vreg_a <= a;
        vreg_b <= b;

        wait for 10 ns; -- propagation delay

        -- Check output
        VALIDATE_ANSWERS(vreg_out, expected, errors, TEST_NAME);
 

        -- FINISH --
        REPORT_TEST_END(TEST_NAME, errors);
        wait;
    end process UNITTEST;

    vreg_out_disp <= VREG_TO_ELEMENTS(vreg_out);
    
    
    
end architecture test;