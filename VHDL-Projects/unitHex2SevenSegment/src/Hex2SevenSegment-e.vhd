 ---------------------------------------------------------------------------
-- Title      : Hex-To-Seven-Segment-Decoder
-- Project    : Creting a seven segment project
-- Date		  : 30.4.2015
-- file		  : enitity or inteface of our Hex-To-Seven-Segment-Decoder enitiy 
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use work.Global.all;

entity Hex2SevenSegment is 
  port (iHexValue	: in  std_ulogic_vector(3 downto 0);
        o7SegCode	: out std_ulogic_vector(6 downto 0));
end Hex2SevenSegment;
