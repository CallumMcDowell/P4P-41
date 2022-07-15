----------------------------------------------------------------------------
--! @file
--! @copyright  Copyright 2015 GNSS Sensor Ltd. All right reserved.
--! @author     Sergey Khabarov
--! @brief      Virtual simple input buffer.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;

entity ibuf_tech is
  generic
  (
    generic_tech : integer := 0
  );
  port (
    o  : out std_logic;
    i  : in std_logic
  );
end; 
 
architecture rtl of ibuf_tech is

component ibuf_inferred is
  port (
    o  : out std_logic;
    i  : in std_logic
  );
end component; 

begin

  inf0 : if generic_tech /= mikron180 generate 
    bufinf : ibuf_inferred port map
    (
      o => o,
      i => i
    );
  end generate;

end;
