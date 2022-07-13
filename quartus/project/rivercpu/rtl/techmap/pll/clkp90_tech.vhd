-----------------------------------------------------------------------------
--! @file
--! @copyright Copyright 2015 GNSS Sensor Ltd. All right reserved.
--! @author    Sergey Khabarov - sergeykhbr@gmail.com
--! @brief     Virtual clock phase offset generator (90 deg)
------------------------------------------------------------------------------

--! Standard library
library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;

entity clkp90_tech is
  generic (
    tech    : integer range 0 to NTECH := 0;
    --! clock frequency in KHz
    freq    : integer := 125000
  );
  port (
    --! Active High
    i_rst    : in  std_logic;
    i_clk    : in  std_logic;
    o_clk    : out std_logic;
    o_clkp90 : out std_logic;
    o_clk2x  : out std_logic;
    o_lock   : out std_logic
  );
end clkp90_tech;

architecture rtl of clkp90_tech is

begin

   inf : if tech = inferred generate
      o_clk    <= i_clk;
      o_clkp90 <= i_clk;
      o_clk2x  <= '0';
      o_lock  <= '0';
   end generate;

end;
