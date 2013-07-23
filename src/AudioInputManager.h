/*
 *  AudioInputManager.h
 *  vj
 *
 *  Created by KAWAKITA Hirofumi on 10/12/14.
 *  Copyright 2010 sgmnt. All rights reserved.
 *
 */

#ifndef _AUDIO_INPUT_MANAGER_
#define _AUDIO_INPUT_MANAGER_

#include "fft.h"
//#include "OSCInputManager.h"

class AudioInputManager{

public:
    
    double maxSignalValue;
    double maxFFTValue;
    
    double *inputSignal;
	double *audioInputFFT;
    
    double fftTotal;
    
    double volume;
    
	void setup(int size);
	void update(float * input, int bufferSize, int nChannels);
	double getSum(int startIndex, int endIndex);
	double getFFTMaxOfRange(int startIndex, int endIndex);
	double getFFTAverageOfRange(int startIndex, int endIndex);
    
private:
	
	complex *pSignal;
	int _signalLength;
	int _fftLength;
    
};

#endif