 ---------------------------------------------------------------------------
-- Title      : Hex-To-Seven-Segment-Decoder
-- Project    : Creting a seven segment project
-- Date		  : 30.4.2015
-- file		  : architecture of the Hex-To-Seven-Segment-Decoder enitity
-------------------------------------------------------------------------------


use work.Global.all;

architecture Rtl of Hex2SevenSegment is
 
-- ---------------------------------------------------------------------------
-- Conversion HEX-Value to low active 7-segment value
-- ---------------------------------------------------------------------------
  
    function ToSevSeg(cValue : std_ulogic_vector(3 downto 0))
      return std_ulogic_vector is
    begin
      
      case cValue(3 downto 0) is
        when "0000" => return "0111111";
        when "0001" => return "0000110";
        when "0010" => return "1011011";
        when "0011" => return "1001111";
        when "0100" => return "1100110";
        when "0101" => return "1101101";
        when "0110" => return "1111101";
        when "0111" => return "0000111";
        when "1000" => return "1111111";
        when "1001" => return "1101111";
        when "1010" => return "1110111";
        when "1011" => return "1111100";
        when "1100" => return "0111001";
        when "1101" => return "1011110";
        when "1110" => return "1111001";
        when "1111" => return "1110001";
        when others => return "XXXXXXX";
      end case;
    end ToSevSeg;
    
	begin
	
-- ---------------------------------------------------------------------------
-- Decode input value to 7-Segment code
-- ---------------------------------------------------------------------------
		o7SegCode  <= ToSevSeg(iHexValue);
 
end Rtl;
