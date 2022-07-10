-- P4P 41 - CNN ISA Extension
-- 07/22 - cmcd407@auckland.ac.nz
--
-- [Name]
-- [Why this block is needed, and how to use it.]

library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;

library work;
use work.CNNISATypes.all;

entity VMUL is
    generic (
        element_length : positive  := ELEN
    );
    port (
        -- inputs
        a, b        : in vreg;
        -- outputs
        output      : out vreg
    );
end entity VMUL;

architecture rtl of VMUL is
    
begin

    GEN_LOGIC: for i in vreg'length/element_length downto 1 generate
        constant upper : integer := i * element_length;
        constant lower : integer := upper - element_length;

        signal temp : signed(2*element_length -1 downto 0);
    begin
        -- truncate to 16 bits
        temp <= signed(a(upper-1 downto lower)) * signed(b(upper-1 downto lower));
        output(upper-1 downto lower) <= std_logic_vector(temp(element_length-1 downto 0));
    end generate GEN_LOGIC;
    
end architecture rtl;