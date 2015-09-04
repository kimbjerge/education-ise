///////////////////////////////////////////////////////////
//  Controller.h
//  Implementation of the Class Controller
//  Created on:      06-aug-2014 23:31:01
//  Original author: kbe
///////////////////////////////////////////////////////////

#if !defined(CONTROLLER__INCLUDED_)
#define CONTROLLER__INCLUDED_

#include "HAL.h"
#include "Dummy.h"

class Controller
{

public:

	Controller(Dummy *algo);
	virtual ~Controller();

	void init();
	void incBand();
	void incParam();
	void incParamValue();
	void decParamValue();

protected:
    void updateUI();
	Dummy *m_algo;
	short m_band;
};

#endif // !defined(CONTROLLER__INCLUDED_)
