///////////////////////////////////////////////////////////
//  Process.cpp
//  Implementation of Process
//  Created on:      06-aug-2014 09:30:58
//  Original author: kbe
///////////////////////////////////////////////////////////
#include "HAL.h"
#include "Algorithm.h"

Algorithm *pAlgoLeft;
Algorithm *pAlgoRight;

void InitProcess(Algorithm *left, Algorithm *right)
{
	pAlgoLeft = left;
	pAlgoRight = right;
}

extern "C" {

	// Interface to SPORT interrupt
	void Process(void)
	{
		//int i;
	
		pAlgoLeft->process(sCh0LeftIn, sCh0LeftOut, INPUT_SIZE);
		pAlgoRight->process(sCh0RightIn, sCh0RightOut, INPUT_SIZE);
	
		/* Not used - extra channels
		for (i=0; i<INPUT_SIZE; i++)
		{
			//sCh0LeftOut[i] = sCh0LeftIn[i];
			//sCh0RightOut[i] = sCh0RightIn[i];
			sCh1LeftOut[i] = sCh1LeftIn[i];
			sCh1RightOut[i] = sCh1RightIn[i];
		}
		*/
	
	}

}
