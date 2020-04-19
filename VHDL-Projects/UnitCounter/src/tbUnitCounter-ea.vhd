-------------------------------------------------------------------------------
-- Title : Creating a strobe-Generator 
-- Project : Chip Design
-------------------------------------------------------------------------------
-- Author : Aljosa Klajderic
-------------------------------------------------------------------------------
-- Description:
-- Testbench for Counter
-------------------------------------------------------------------------------

library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all; 
 use work.Global.all;
 
entity tbCounter is
end tbCounter;


architecture Bhv of tbCounter is

   --CLk Gen	
    signal Clk          	 : std_ulogic:= '1' ;
    signal finisihed   		 : std_ulogic:= '0'  ;
    constant cHalf_period: time  :=  1 ns;
    constant cCount:integer 	 :=  5;
    

    signal Rst     	  : std_ulogic := not('0');
    signal Strobe		 : std_ulogic;
    signal ClkCount 		 : unsigned(LogDualis(cCount) downto 0);
    signal StrobeCnt 		 : unsigned(LogDualis(cCount) downto 0):= (others=> '0');
begin

--best lines ever
Clk <= not Clk after cHalf_period when finisihed/='1' else '0';
Rst <= not('1') after 49 ns;
finisihed <= '1' after 60 ns;


-- instantinting the architectures
DUTCounnter: entity work.Counter(Bhv)
  generic map(gBitWidth=> (cCount))
  port map (Strobe,Rst,StrobeCnt);
  
  
process(finisihed) is
begin
	if finisihed = '1' then
		report "the meausrment hast finished";	
	end if;
end process;




end architecture Bhv;