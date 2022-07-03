-- P4P 41 - CNN ISA Extension
-- 07/22 - cmcd407@auckland.ac.nz

entity Min is
    generic (
        type data_type
    );
    port (
        -- inputs
        a, b            : in data_type;
        -- outputs
        output          : out data_type
    );
end entity Min;

architecture rtl of Min is
    
begin
    
    output <= a when (a < b) else b;
    
end architecture rtl;