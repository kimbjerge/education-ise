///////////////////////////////////////////////////////////////////////////////
//
// 	DSP Design Framework (C++) for stereo sample block processing
//  
//  Main program and initialization
//
//  6/8-2014 KBE
///////////////////////////////////////////////////////////////////////////////
#include "HAL.h"
#include "sysreg.h"  //definition of reg_SYSCFG
#include "ccblkfn.h" //definition of built-in function
					 //  in this program:sysreg_write()
#include "Dummy.h"
#include "Controller.h"

// Initialization of sample block inputs
void InitProcess(Algorithm *left, Algorithm *right);

// Initialization of control for switch inputs
void InitSwitch(Controller *left, Controller *right);

// Instance of dummy algorithm for left and right stereo channels
Dummy AlgoLeft(SAMPLE_FS);
Dummy AlgoRight(SAMPLE_FS);

// Instance of controller left and right
Controller CtrlLeft(&AlgoLeft);
Controller CtrlRight(&AlgoRight);

void InitAlgoProcess(void)
{
	// Initialization of boundary interfaces (interrupts)
	AlgoLeft.create();
	AlgoLeft.create();
	InitProcess(&AlgoLeft, &AlgoLeft);

	CtrlLeft.init();
	CtrlRight.init();
	InitSwitch(&CtrlLeft, &CtrlRight);
}

void InitSystemHardware(void)
{
	sysreg_write(reg_SYSCFG, 0x32); //Enable 64-bit,free-running cycle-counter
							   		//BF533 Hardware Reference pg 4-6
	ucLED = 0x3F; // Turn all LEDs on

	Init_EBIU();
	Init_LED();
	Init_1836();
	Init_Sport();
	Init_DMA();
	Init_PF();
	Init_Timer();
}

void main(void)
{

	InitSystemHardware(); // Initialization of BF533
	InitAlgoProcess(); // Initialization of processing algorithm
	Init_Interrupt(); // Enable interrupts
	Enable_DMA();

	while(1)
	{
	}; // wait forever
}
