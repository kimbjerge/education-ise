///////////////////////////////////////////////////////////
//  Controller.cpp
//  Implementation of the Class Controller
//  Created on:      06-aug-2014 23:31:01
//  Original author: kbe
///////////////////////////////////////////////////////////

#include "Controller.h"


Controller::Controller(Dummy *algo)
{
	m_algo = algo;
}

Controller::~Controller()
{

}

void Controller::init()
{
	m_band = 0;
}

void Controller::updateUI()
{
	SetMaskLed(0x0f, m_band<<4);
}

void Controller::incBand()
{
	// Selects the next EQ band
	m_band++;
	if (m_band >= NUM_EQ_BANDS)
		m_band = 0;
	updateUI();
}

void Controller::incParam()
{
	// Insert code to select next parameter in algorithm

}

void Controller::incParamValue()
{
	// Increments parameter value
	float val = m_algo->incParameter(m_band, PM_GAIN, 0.1);

	// Update LED UI
	unsigned char tmp = (val > 2 ? val: val*10);
	SetMaskLed(0x38, tmp & 0x07);
}

void Controller::decParamValue()
{
	// Decrements parameter value
	float val = m_algo->decParameter(m_band, PM_GAIN, 0.1);

	// Update LED UI
	unsigned char tmp = (val > 2 ? val: val*10);
	SetMaskLed(0x38, tmp & 0x07);
}


