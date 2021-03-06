-------------------------------------------------------------------------------
-- Title      : Testbench for functions in global project package
-- Project    : Audio Signal Processing
-------------------------------------------------------------------------------
-- $Id: tbGlobal-Bhv-ea.vhd,v 1.1 2003/10/13 14:44:15 pfaff Exp $
-------------------------------------------------------------------------------
-- Author     : Copyright 2003: Markus Pfaff
-- Standard   : Using VHDL'93
-- Simulation : Model Technology Modelsim
-- Synthesis  : Exemplar Leonardo
-------------------------------------------------------------------------------
-- Description:
--
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

use work.Global.all;

entity tbGlobal is

end entity tbGlobal;

architecture Bhv of tbGlobal is

begin  -- architecture Bhv

  TickleLogDualis : process is
    type     aIntArray is array (natural range <>) of integer;
    variable LogDualisOf : aIntArray(0 to 100) := (others => 0);
  begin
    for idx in LogDualisOf'range loop
      LogDualisOf(idx) := LogDualis(idx);
    end loop;  -- idx
    wait;
  end process TickleLogDualis;

end architecture Bhv;

