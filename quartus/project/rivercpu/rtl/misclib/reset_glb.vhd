--!
--! Copyright 2018 Sergey Khabarov, sergeykhbr@gmail.com
--!
--! Licensed under the Apache License, Version 2.0 (the "License");
--! you may not use this file except in compliance with the License.
--! You may obtain a copy of the License at
--!
--!     http://www.apache.org/licenses/LICENSE-2.0
--!
--! Unless required by applicable law or agreed to in writing, software
--! distributed under the License is distributed on an "AS IS" BASIS,
--! WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
--! See the License for the specific language governing permissions and
--! limitations under the License.
--!

library ieee;
use ieee.std_logic_1164.all;
library commonlib;
use commonlib.types_common.all;

--! @brief NoC global reset former.
--! @details This module produces output reset signal in a case if
--!          button 'Reset' was pushed or PLL isn't a 'lock' state.
--! param[in]  inSysReset Button generated signal
--! param[in]  inSysClk Clock from the PLL. Bus clock.
--! param[in]  inPllLock PLL status.
--! param[out] outReset Output reset signal with active 'High' (1 = reset).
entity reset_global is
  port (
    inSysReset  : in std_ulogic;
    inSysClk    : in std_ulogic;
    outReset    : out std_ulogic
    );
end;

architecture arch_reset_global of reset_global is

  type reg_type is record
    delay_cnt : std_logic_vector(7 downto 0);
  end record;

  signal r : reg_type;
begin

  proc_rst : process (inSysClk, inSysReset, r) 
    variable wb_delay_cnt : std_logic_vector(7 downto 0);
    variable sys_reset : std_logic;
  begin

    sys_reset := inSysReset;
    
    wb_delay_cnt := r.delay_cnt;
    if r.delay_cnt(7) = '0' then
      wb_delay_cnt := r.delay_cnt + 1;
    end if;
    
    if sys_reset = '1' then
      r.delay_cnt <= (others => '0');
    elsif rising_edge(inSysClk) then 
      r.delay_cnt  <= wb_delay_cnt;
    end if;
  end process;

  outReset <= not r.delay_cnt(7);
  
end;  
