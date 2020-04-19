-------------------------------------------------------------------------------
-- Title : Creating a counter
-- Project : Chip Design
-------------------------------------------------------------------------------
-- Author : Aljosa Klajderic
-------------------------------------------------------------------------------
-- Description:
-- Testbenxh for Counter
-------------------------------------------------------------------------------

library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all; 
 use work.Global.all;
 
entity Counter is
 generic ( gBitWidth : natural := 10);
 port 
 ( 
 iClk : in std_ulogic;
 inResetAsync : in std_ulogic;
 oState : out unsigned(LogDualis(gBitWidth) downto 0)
 ); -- Strobe with the above given cycle time
end Counter;


architecture Bhv of Counter  is

signal State : unsigned(LogDualis(gBitWidth) downto 0) := (others =>'0');

begin

process (iClk, inResetAsync) is
begin
if inResetAsync = not('1') then
	State <= (others =>'0');
elsif rising_edge(iClk) then
	State <= (State+1) mod (2**gBitWidth);
end if;
end process;

oState <= State;


end architecture Bhv;