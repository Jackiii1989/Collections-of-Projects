-------------------------------------------------------------------------------
-- Title : D-FlipFlop with Asnyc reset 
-- Project : Chip Design
-- Created : 2012-12-05
-------------------------------------------------------------------------------
-- Date:
-- creating the D-FlipFlop. Diving the entity and architecture separetly makes no
-- sence for this kind of example.
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

entity DFFAsnycResetBE is
  port (
    iD           : in  std_ulogic;              -- Data Input
    iClk         : in  std_ulogic;              -- Clock Input
    inResetAsync : in  std_ulogic;              -- Asnyc reset 
    oQ           : out std_ulogic;              -- Data Output 
    oQ_n         : out std_ulogic              -- Data Output inverted
  );
end DFFAsnycResetBE;

architecture Bhv of DFFAsnycResetBE is
signal Q: std_ulogic;
begin
    process(iClk, inResetAsync)
    begin
      if inResetAsync = not('1') then -- async reset mit negative Logik
        Q <= '0';
      elsif iClk'event and iClk='1' then
        Q <= iD;
	  elsif iClk'event and iClk='0' then
		Q <= iD;
      end if;
    end process;
    oQ_n <= not Q;
    oQ   <= Q;
end architecture Bhv;