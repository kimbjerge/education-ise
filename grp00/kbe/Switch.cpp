///////////////////////////////////////////////////////////
//  Switch.cpp
//  Implementation of Switch
//  Created on:      06-aug-2014 09:30:58
//  Original author: kbe
///////////////////////////////////////////////////////////
#include "Controller.h"

// Controller reference
Controller *pCtrlRight;
Controller *pCtrlLeft;

void InitSwitch(Controller *left, Controller *right)
{
	pCtrlLeft = left;
	pCtrlRight = right;
}

extern "C" {

    void PressedSwitch(short sw)
    {
    	switch (sw) {
			case 4:
				pCtrlLeft->incParamValue();
				pCtrlRight->incParamValue();
				break;
			case 5:
				pCtrlLeft->decParamValue();
				pCtrlRight->decParamValue();
				break;
			case 6:
				pCtrlLeft->incParam();
				pCtrlRight->incParam();
				break;
			case 7:
				pCtrlLeft->incBand();
				pCtrlRight->incBand();
				break;

    	}
    }

}
