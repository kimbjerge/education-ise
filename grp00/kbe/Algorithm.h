///////////////////////////////////////////////////////////
//  Algorithm.h
//  Implementation of the Class Algorithm
//  Created on:      06-aug-2014 09:30:58
//  Original author: kbe
///////////////////////////////////////////////////////////

#if !defined(ALGORITHM_INCLUDED_)
#define ALGORITHM_INCLUDED_

class Algorithm
{

public:
	Algorithm();
	virtual ~Algorithm();

	virtual void process(short* input, short* output, short len) = 0;
	virtual void create(void) = 0;

};
#endif // !defined(ALGORITHM_INCLUDED_)
