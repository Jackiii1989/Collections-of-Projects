-------------------------------------------------------------------------------
-- Title   : D-Latch and D-FlipFlop Inference Examples
-- Project : Chip Design
-- Date: 2017-11-27
-------------------------------------------------------------------------------
-- Description:
-- Different D-Latch and D-FlipFlop Inference Examples. Can be used to find out
-- which synthesizer results you will get independent of slightly different 
-- VHDL descriptions for latches and flip-flops.
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity TBFlipFlopBS is
  port (
    KEY0_Clk   	: in  std_ulogic;              -- Clock Input
	 --SW0_Clk   		: in  std_ulogic;              -- Clock Input
	 SW0_iD   	   : in  std_ulogic;              -- Clock Input
	 SW3_iRst   	: in  std_ulogic;              -- Clock Input
   LED0 			: out std_ulogic;              -- Data Output A
   LED1 			: out std_ulogic              -- Data Output B
);
end entity TBFlipFlopBS;


architecture Rtl of TBFlipFlopBS is


  signal KEY0_ClkInter: std_ulogic;
  
  signal  SW0_iD_inv   	: std_ulogic;              -- Clock Input
  signal SW1_iEn_inv   	: std_ulogic;              -- Clock Input
  signal SW2_iSet_inv  	: std_ulogic;              -- Clock Input
  signal SW3_iRst_inv   : std_ulogic;              -- Clock Input
 
begin  -- architecture Rtl

-- instantinting the architectures
FFAsnycRst: entity work.DFFAsnycResetBE(Bhv)
  port map (SW0_iD,KEY0_ClkInter,SW3_iRst,Led0,Led1);



  KEY0_ClkInter <= '1' when KEY0_Clk='0' else '0';
  
  --inverting the signals
  SW0_iD_inv<=not SW0_iD;


end architecture Rtl;
