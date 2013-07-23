/*
 *  AudioInputManager.cpp
 *  vj
 *  
 *  Created by KAWAKITA Hirofumi on 10/12/14.
 *  Copyright 2012 sgmnt.org All rights reserved.
 *  
 */

#include "AudioInputManager.h"

void AudioInputManager::setup(int size){
    
	maxSignalValue = 0;
    maxFFTValue    = 0;
    
	_signalLength = size;
	_fftLength    = size / 2;
	
    volume = 16.0;
    
	pSignal       = new complex[_signalLength];
	inputSignal   = new double[_signalLength];
	audioInputFFT = new double[_fftLength];
    
}

void AudioInputManager::update(float * input, int bufferSize, int nChannels){
	
    maxSignalValue *= 0.3;
    
	for ( int i = 0; i < bufferSize; i++ ){
        double value   = input[i*nChannels] + input[i*nChannels+1];
        pSignal[i]     = value * volume;
		inputSignal[i] = value * volume;
        if( maxSignalValue < value * volume ){
            maxSignalValue = value * volume;
        }
	}
	
	CFFT::Forward( pSignal, _signalLength);
	
    maxFFTValue = 0;
    fftTotal = 0;
    
	for( int i = 0; i < _fftLength; i++ ){
        if( 0 == pSignal[i] ){
            audioInputFFT[i] = 0;
        }else{
            audioInputFFT[i] = sqrt(pSignal[i].norm());
            //audioInputFFT[i] = pSignal[i].norm();
        }
        if( maxFFTValue < audioInputFFT[i] ){
            maxFFTValue = audioInputFFT[i];
        }
        if(i<100){
            fftTotal += audioInputFFT[i];
        }
	}
	
    if( fftTotal == 0 ){
        fftTotal = 1;
    }
    
}

double AudioInputManager::getSum(int startIndex, int endIndex){
    double val = 0.0;
	for( int i = startIndex; i < endIndex; i++ ){
		val += audioInputFFT[i];
	}
	return val;
}

double AudioInputManager::getFFTMaxOfRange(int startIndex, int endIndex ){
    double max = 0.0;
	for( int i = startIndex; i < endIndex; i++ ){
        if( max < audioInputFFT[i] ) max = audioInputFFT[i];
	}
	return max;
}

double AudioInputManager::getFFTAverageOfRange(int startIndex, int endIndex ){
	double val = 0.0;
	for( int i = startIndex; i < endIndex; i++ ){
		val += audioInputFFT[i];
	}
	return val / ( endIndex - startIndex );
}