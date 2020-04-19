-------------------------------------------------------------------------------
-- Title : Creating a strobe-Generator 
-- Project : Chip Design
-------------------------------------------------------------------------------
-- Author : Aljosa Klajderic
-------------------------------------------------------------------------------
-- Description:
-- Strobe generator
-------------------------------------------------------------------------------

library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all; 
 use work.Global.all;
 
entity StrobeGen is
 generic ( gNrClkCycles : natural := 10);--50E6);
 port -- Sequential logic inside this unit
 ( 
 iClk : in std_ulogic;
 inResetAsync : in std_ulogic;
 oStrobe : out std_ulogic;
 oCounter : out unsigned(LogDualis(gNrClkCycles) downto 0)
 ); -- Strobe with the above given cycle time
end StrobeGen;


architecture Bhv of StrobeGen is

signal Counter : unsigned(LogDualis(gNrClkCycles) downto 0) := (others =>'0');

begin

process (iClk, inResetAsync) is
begin
if inResetAsync = not('1') then
	Counter <= (others =>'0');
elsif rising_edge(iClk) then
	Counter <= (Counter+1) mod (gNrClkCycles+1);
end if;
end process;

oCounter <= Counter;

process(Counter) is
begin
	if to_integer(Counter) = gNrClkCycles then
	 oStrobe <= cActivated;
	else
	 oStrobe <= cInactivated;
	end if;
end process;
end architecture Bhv;