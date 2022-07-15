----------------------------------------------------------------------------
--! @file
--! @copyright  Copyright 2015 GNSS Sensor Ltd. All right reserved.
--! @author     Sergey Khabarov
--! @brief      Virtual input buffer with the differential signals.
----------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;

entity idsbuf_tech is
  generic (
    generic_tech : integer := 0
  );
  port (
    clk_p : in std_logic;
    clk_n : in std_logic;
    o_clk  : out std_logic
  );
end; 
 
architecture rtl of idsbuf_tech is

begin

  infer : if generic_tech = inferred generate 
      o_clk <= clk_p;
  end generate;

end;
