 ---------------------------------------------------------------------------------------
-- Title      : Hex-To-Seven-Segment-Decoder
-- Project    : Creting a seven segment project
-- Date		  : 30.4.2015
-- file		  : testbench to check the proper working of the Hex2SevenSegment-e project
----------------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use work.global.all;

entity tbHex2SevenSegment is
end tbHex2SevenSegment;

architecture Bhv of tbHex2SevenSegment is

  signal nibble				: std_ulogic_vector(3 downto 0);
  signal sevenSegCode		: std_ulogic_vector(6 downto 0);

begin

  DUT: entity work.Hex2SevenSegment(Rtl)
    port map (iHexValue	=> nibble,
		      o7SegCode	=> sevenSegCode);

  Stimul : process is
  begin
		-- 8
		nibble <= ('1','0','0','0');
		wait for 100 ns;

		-- 0
		nibble <= "0000";
		wait for 100 ns;
		
		-- F
		nibble <= "1111";
		wait for 100 ns;
		
		-- 1
		nibble <= "0001";
		wait for 100 ns;
  
    wait;
  end process Stimul;
  
end Bhv;

