///////////////////////////////////////////////////////////
//  Dummy.h
//  Implementation of the Class Dummy
//  Created on:      06-aug-2014 09:31:01
//  Original author: kbe
///////////////////////////////////////////////////////////

#if !defined(DUMMY_INCLUDED_)
#define DUMMY_INCLUDED_

#include "HAL.h"
#include "Algorithm.h"

#define NUM_EQ_BANDS 4 

enum PARAMETER {
	PM_GAIN = 0,
	PM_FREQ = 1,
	PM_Q = 2
};

class Dummy : public Algorithm
{

public:

	Dummy(int sampleRate);
	virtual ~Dummy();

	virtual void process(short* input, short* output, short len);
	virtual void create(void);
	void setParameter(short band, PARAMETER param, float value);
	float incParameter(short band, PARAMETER param, float delta);
	float decParameter(short band, PARAMETER param, float delta);

protected:

	int m_sampleRate;
};

#endif // !defined(DUMMY_INCLUDED_)
