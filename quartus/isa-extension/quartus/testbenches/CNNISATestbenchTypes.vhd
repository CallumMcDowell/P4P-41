library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
use ieee.math_real.all;

library work;
use work.CNNISATypes.all;

package CNNISATestbenchTypes is
    -- Constants
    constant ELEMENT_MAX_SIGNED     : signed(ELEN-1 downto 0)   := (signed(ELEN-1 downto 0)'LEFT => '0', others => '1');
    constant ELEMENT_MAX_UNSIGNED   : signed(ELEN-1 downto 0)   := (others => '1');

    type elements is array (VLEN/ELEN -1 downto 0) of signed(ELEN-1 downto 0);

    -- Functions
    function REAL_TO_INT(x : real) return integer;
    function REAL_TO_WORD(x : real) return signed;
    function INT_TO_WORD(x : integer) return signed;

    function VREG_TO_ELEMENTS(x : vreg) return elements;

    -- Procedures
    procedure REPORT_TEST_START(entity_name : string);
    procedure REPORT_TEST_END(entity_name : string; errors : integer);
    
end package CNNISATestbenchTypes;

package body CNNISATestbenchTypes is

    -- Functions
    function REAL_TO_INT(x : real) return integer is
    begin
        return integer(round(x));
    end REAL_TO_INT;

    function REAL_TO_WORD(x : real) return signed is
    begin
        return to_signed(integer(round(x)), ELEN);
        -- WARNING: ASSUMES WORD IS SIGNED! Add precompiler case statement?
    end REAL_TO_WORD;

    function INT_TO_WORD(x : integer) return signed is
    begin
        return to_signed(x, ELEN);
    end INT_TO_WORD;

    function VREG_TO_ELEMENTS(x : vreg) return elements is
        variable elements_array : elements;
        variable upper, lower : integer;
    begin
        for i in vreg'length/ELEN downto 1 loop
            upper := i * ELEN;
            lower := upper - ELEN;

            elements_array(i-1) := signed(x(upper-1 downto lower));
        end loop;

        return elements_array;
    end VREG_TO_ELEMENTS;


    -- Procedures
    procedure REPORT_TEST_START(entity_name : string) is
    begin
        report entity_name & ": Test START";
    end REPORT_TEST_START;

    procedure REPORT_TEST_END(entity_name : string; errors : integer) is
    begin
        if errors > 0 then
            report entity_name & ": Test FAILED (" & integer'image(errors) & " errors)";
        else
            report entity_name & ": Test COMPLETE";
        end if;
    end REPORT_TEST_END;
        
end package body CNNISATestbenchTypes;