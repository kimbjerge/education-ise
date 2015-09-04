///////////////////////////////////////////////////////////
//  Dummy.cpp
//  Implementation of the Class Dummy
//  Created on:      06-aug-2014 09:31:01
//  Original author: kbe
///////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include "Dummy.h"


Dummy::Dummy(int sampleRate){
	m_sampleRate = sampleRate;
}

Dummy::~Dummy(){

}

void Dummy::process(short* input, short* output, short len){

	// Talk through, just copy input block to output                    
	memcpy(output, input, len*sizeof(short));
}

void Dummy::create(void) {

}

float Dummy::incParameter(short band, PARAMETER param, float delta) {
    float value = 0;

	return value;
}

float Dummy::decParameter(short band, PARAMETER param, float delta) {
    float value = 0;

	return value;
}

void Dummy::setParameter(short band, PARAMETER param, float value) {

}
