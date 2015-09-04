///////////////////////////////////////////////////////////////////////////////
//
// 	
//  Experiment 7.4 talk-through program using BF533 EZ-KIT
//  FILE name: HAL.h
//
//  Description: Talk-through program with different CODEC settings
//		 C header file containing prototypes and macros
//  
//  For the book "Embedded Signal Processing with the Micro Signal Architecture"
//		  By Woon-Seng Gan and Sen M. Kuo
//		  Publisher: John Wiley and Sons, Inc.
//
//  Tools used: VisualDSP++ v4.0 (running on BF533 EZ-KIT)
//  Modified: 6/8-2014 - KBE
//
///////////////////////////////////////////////////////////////////////////////


//Necessary variable declaration
#ifndef __HAL_DEFINED
	#define __HAL_DEFINED
	
//#define _USE_LEGACY_CDEF_BEHAVIOUR
#include <cdefBF533.h>
#include <sys\exception.h>

#define pFlashA_PortA_Dir  (volatile unsigned char *)0x20270006
#define pFlashA_PortA_Data (volatile unsigned char *)0x20270004

#define pFlashA_PortB_Dir  (volatile unsigned char *)0x20270007
#define pFlashA_PortB_Data (volatile unsigned char *)0x20270005

//Address of AD1836 control register
//(refer to page 15 of AD1836 Datasheet)
#define DAC_CONTROL_1 0x0000
#define DAC_CONTROL_2 0x1000
#define DAC_VOLUME_0  0x2000
#define DAC_VOLUME_1  0x3000
#define DAC_VOLUME_2  0x4000
#define DAC_VOLUME_3  0x5000
#define DAC_VOLUME_4  0x6000
#define DAC_VOLUME_5  0x7000
#define ADC_0_PEAK_LEVEL  0x8000
#define ADC_1_PEAK_LEVEL  0x9000
#define ADC_2_PEAK_LEVEL  0xA000
#define ADC_3_PEAK_LEVEL  0xB000
#define ADC_CONTROL_1 0xC000
#define ADC_CONTROL_2 0xD000
#define ADC_CONTROL_3 0xE000

#define SAMPLE_FS				48000	// Sample rate in Hz
#define INPUT_SIZE				128
#define TOTAL_FRAME				2

// size of array sCodec1836TxRegs
#define CODEC_1836_REGS_LENGTH	11

// DMA flow mode (Autobuffer)
#define FLOW_1					0x1000

// SPORT0 word length
#define SLEN_16					0x000F

// names for slots in ad1836 audio frame
//AD1836 Datasheet pg 10-12
#define INTERNAL_ADC_L0			0
#define INTERNAL_ADC_L1			1
#define INTERNAL_ADC_R0			2
#define INTERNAL_ADC_R1			3

#define INTERNAL_DAC_L0			0
#define INTERNAL_DAC_L1			1
#define INTERNAL_DAC_R0			2
#define INTERNAL_DAC_R1			3

extern "C" {
	
	//Global variables
	extern short sCh0LeftIn[];
	extern short sCh0LeftOut[];
	extern short sCh0RightIn[];
	extern short sCh0RightOut[];
	extern short sCh1LeftIn[];
	extern short sCh1LeftOut[];
	extern short sCh1RightIn[];
	extern short sCh1RightOut[];
	//Variable to configure Codec1836
	extern volatile unsigned short sCodec1836TxRegs[];
	//Buffer from/to Codec1836
	extern volatile short sDataBufferTX[];
	extern volatile short sDataBufferRX[];
	
	//LED state
	extern volatile unsigned char ucLED;
	inline void SetMaskLed(unsigned char mask, unsigned char val)
	                  { ucLED = (ucLED & mask) | val;};
	inline void SetLed(unsigned char val)
	                  { ucLED = val;};

	// Function prototypes
	void Init_EBIU(void);
	void Init_LED(void);
	void Init_1836(void);
	void Init_Sport(void);
	void Init_DMA(void);
	void Init_PF(void);
	void Init_Timer(void);
	void Init_Interrupt(void);
	void Enable_DMA(void);

	void PressedSwitch(short sw);
	void Process(void);

	// Interrupt handler for SPORT0 RX
	EX_INTERRUPT_HANDLER(Sport0_RX_ISR);

	// Interrupt handler for Core Timer
	EX_INTERRUPT_HANDLER(Timer_ISR);

	// Interrupt handler for programmable flags
	EX_INTERRUPT_HANDLER(Switch_ISR);

};

#endif
