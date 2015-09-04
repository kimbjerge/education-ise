///////////////////////////////////////////////////////////////////////////////
//
// 	
//  Experiment 7.4 talk-through program using BF533 EZ-KIT
//  FILE name: Init.c
//
//  Description: C file containing all initialization routines
//		 
//  
//  For the book "Embedded Signal Processing with the Micro Signal Architecture"
//		  By Woon-Seng Gan and Sen M. Kuo
//		  Publisher: John Wiley and Sons, Inc.
//
//  Tools used: VisualDSP++ v4.0 (running on BF533 EZ-KIT)
//  Modified: 6/8-2014 - KBE
//
///////////////////////////////////////////////////////////////////////////////
#include "HAL.h"

volatile unsigned short sCodec1836TxRegs
	[CODEC_1836_REGS_LENGTH] = {
		DAC_CONTROL_1	| 0x010,	// 16 bit data width
		DAC_CONTROL_2	| 0x000,
		DAC_VOLUME_0	| 0x3FF,
		DAC_VOLUME_1	| 0x3FF,
		DAC_VOLUME_2	| 0x3FF,
		DAC_VOLUME_3	| 0x3FF,
		DAC_VOLUME_4	| 0x000,
		DAC_VOLUME_5	| 0x000,
		ADC_CONTROL_1	| 0x024,	// 12dB gain for left and right
		ADC_CONTROL_2	| 0x020,	// 16 bit data width
		ADC_CONTROL_3	| 0x00A
};

// Prepare buffer for DMA transfer
section("L1_data_b") volatile short sDataBufferRX[4 * INPUT_SIZE * TOTAL_FRAME];
section("L1_data_b") volatile short sDataBufferTX[4 * INPUT_SIZE * TOTAL_FRAME];

// Prepare the temp audio data for processing
section("L1_data_b") short sCh0LeftIn[INPUT_SIZE];
section("L1_data_b") short sCh0RightIn[INPUT_SIZE];
section("L1_data_b") short sCh0LeftOut[INPUT_SIZE];
section("L1_data_b") short sCh0RightOut[INPUT_SIZE];
section("L1_data_b") short sCh1LeftIn[INPUT_SIZE];
section("L1_data_b") short sCh1RightIn[INPUT_SIZE];
section("L1_data_b") short sCh1LeftOut[INPUT_SIZE];
section("L1_data_b") short sCh1RightOut[INPUT_SIZE];

// variable to store the state of LED
volatile unsigned char ucLED;

//Enable and Configure the Flash
//(EZKIT Manual pg2-6)
void Init_EBIU(void)
{
	*pEBIU_AMBCTL0 = 0x7BB07BB0;
	*pEBIU_AMBCTL1 = 0x7BB07BB0;
	*pEBIU_AMGCTL  = 0x000F;
	
}

//Flash A port B direction is set
//to initialize LED
//(EZKIT Manual pg 2-9)
void Init_LED(void)
{
	*pFlashA_PortB_Dir = 0x3F; 	
	
	// The initial condition for the LEDs
	*pFlashA_PortB_Data = ucLED;
}

//Initialize  and configure AD1836
void Init_1836(void)
{
	volatile int i, j;
	
	//EZKIT Manual pg 2-8
	//last bit of Flash A port A is connected
	//to the reset pin of AD1836
	*pFlashA_PortA_Dir = 0x01;
	*pFlashA_PortA_Data = 0x00;
	
	//(pg 3-3 EZKIT Manual)
	// The clock for ADSP BF-533 is 27MHz x 10 = 270 MHz
	// Minimum time required for resetting AD1836 is 5ns
	for (i = 0; i < 20; i++);
	
	// write to Port A to enable AD1836
	*pFlashA_PortA_Data = 0x01;
	
	// Recover from reset, takes 4500 MClks
	//(where MClk = 12.88MHz)
	//AD1836 Datasheet pg 3
	for (i = 0; i < 0xF000; i++);
	
	// Configure SPI
	// DMA write transfer, 16bits, SCK toggles at beginning
	// of data bit, Active low SCK, As master
	*pSPI_CTL = TIMOD | SIZE | MSTR;
	*pSPI_BAUD = 16;
	*pSPI_FLG = FLS4;
	
	// Setup DMA5 transfer for control regs to AD1836
	// DMA channel 5 is by default mapped to SPI
	*pDMA5_PERIPHERAL_MAP = 0x5000;
	*pDMA5_START_ADDR = (void *)sCodec1836TxRegs;
	*pDMA5_CONFIG = WDSIZE_16;
	*pDMA5_X_COUNT = CODEC_1836_REGS_LENGTH;
	*pDMA5_X_MODIFY = 2;
	
	// Start DMA by first enabling DMA xfer, followed by SPI
	*pDMA5_CONFIG = *pDMA5_CONFIG | DMAEN;
	*pSPI_CTL = *pSPI_CTL | SPE;
	
	// Wait until DMA xfer finished
	for (j = 0; j < 0x0AFF; j++);
	
	// Disable SPI
	*pSPI_CTL = 0x0000;
	
}

// Configure Sport0 for I2S mode,
//to transmit/receive data to/from the AD1836.
//Configure Sport for external clocks and frame syncs.												//
void Init_Sport(void)
{
	// Sport0 receive configuration
	// External CLK, External Frame sync, MSB first, Active Low
	// 16-bit data, Stereo frame sync enable
	*pSPORT0_TCR1 = TCKFE | LTFS | TFSR;
	*pSPORT0_TCR2 = TSFSE | TXSE | SLEN_16;
	
	// Sport0 transmit configuration
	// External CLK, External Frame sync, MSB first, Active Low
	// 16-bit data, Secondary side enable, Stereo frame sync enable
	*pSPORT0_RCR1 = RFSR | LRFS | RCKFE;
	*pSPORT0_RCR2 = RSFSE | RXSE | SLEN_16;
}


//Initialize DMA1 (by default mapped to Sport0 RX)
//and DMA2 (mapped to Sport0 TX)
void Init_DMA(void)
{
	// Set DMA1 to SPORT0 RX
	*pDMA1_PERIPHERAL_MAP = 0x1000;

	// Configure DMA1
	// 16-bit transfers, Interrupt on completion, Autobuffer mode
	//2D DMA, Interrupt after each row
	*pDMA1_CONFIG = FLOW_1 | DI_EN | WNR | WDSIZE_16 | DMA2D | DI_SEL;
									
	 // Start address of data buffer
	*pDMA1_START_ADDR = (void *)sDataBufferRX;
	// DMA inner loop count
	*pDMA1_X_COUNT = 4*INPUT_SIZE;
	// Inner loop address increment
	*pDMA1_X_MODIFY = 2;
	*pDMA1_Y_COUNT = TOTAL_FRAME;
	*pDMA1_Y_MODIFY = 2;
	
	// Set up DMA2 to transmit
	// Map DMA2 to Sport0 TX
	*pDMA2_PERIPHERAL_MAP = 0x2000;
	
	// Configure DMA2
	// 16-bit transfers, Autobuffer mode, 2D DMA
	*pDMA2_CONFIG = WDSIZE_16 | FLOW_1 | DMA2D;
	// Start address of data buffer
	*pDMA2_START_ADDR = (void *)sDataBufferTX;
	// DMA inner loop count
	*pDMA2_X_COUNT = 4*INPUT_SIZE;
	// Inner loop address increment
	*pDMA2_X_MODIFY = 2;
	*pDMA2_Y_COUNT = TOTAL_FRAME;
	*pDMA2_Y_MODIFY = 2;
	

}

// Initialize the programmable flags as input
void Init_PF(void)
{
	// Enable PF10 and PF11 as input, interrupt source,
	// edge sensitive
	*pFIO_DIR = *pFIO_DIR & 0xF0FF;
	*pFIO_INEN = *pFIO_INEN | 0x0F00;
	*pFIO_EDGE = *pFIO_EDGE | 0x0F00;
	*pFIO_MASKA_D = *pFIO_MASKA_D | 0x0F00;
}

// Initialize the timer used to change the LEDs state
void Init_Timer(void)
{
	// BF533 Hardware Reference pg 15-46 (Core Timer)
	/*
	// Assign TPERIOD with 100,000
	*pTCOUNT = *pTPERIOD = 0x000186A0;
	
	// Assign TSCALE with 26, so that we get a 100Hz timer
	// as CCLK is 10 x 27MHz on the EZ-KIT LITE
	*pTSCALE = 26;	

	// Enable the timer, auto reload
	*pTCNTL = *pTCNTL | 0x00000007;*/
	
	//Using Timer0 instead core timer
	*pTIMER0_CONFIG		= 0x0019;
	*pTIMER0_PERIOD		= 0x00800000;
	*pTIMER0_WIDTH		= 0x00400000;
	*pTIMER_ENABLE		= 0x0001;
}

void Init_Interrupt(void)
{
	unsigned int uMask;
	
	// Interrupt when DMA1 transfer is finished
	*pSIC_IAR0 = 0xFFFFFFFF;
	*pSIC_IAR1 = 0xFFFFFF2F;
	*pSIC_IAR2 = 0xFFFF5FF4;
	
	register_handler(ik_ivg9, Sport0_RX_ISR);
	register_handler(ik_ivg12, Switch_ISR);
	register_handler(ik_ivg11, Timer_ISR);
	
	// Enable mask for SPORT0 RX and PF A
	*pSIC_IMASK = 0x00090200;
	
	/* Enable core timer interrupt
	uMask = cli();	
	uMask = uMask | 0x00000040; // set the IVTMR interrupt on
	sti(uMask);*/
}

void Enable_DMA(void)
{
	// Enable DMA1 and DMA2
	*pDMA1_CONFIG = *pDMA1_CONFIG | DMAEN;
	*pDMA2_CONFIG = *pDMA2_CONFIG | DMAEN;
	
	// Enable SPORT0 RX and SPORT0 TX
	*pSPORT0_RCR1 = *pSPORT0_RCR1 | RSPEN;
	*pSPORT0_TCR1 = *pSPORT0_TCR1 | TSPEN;	
}

