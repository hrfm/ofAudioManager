#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "AudioInputManager.h"

class testApp : public ofBaseApp{
    
	public:
		void setup();
		void update();
		void draw();
    
        void audioReceived 	(float * input, int bufferSize, int nChannels);
        
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    private :
        
        float _width;
        float _halfWidth;
        float _height;
        float _halfHeight;
        
        ofxOscSender _sender;
        
        bool _updated;

        int _mode;
        
        float _gain;
        float _gainNorm;
    
        int _averageNorm;
        int _averageDragStartX;
        int _averageStart;
        int _averageEnd;
        float _fftAverageRatio;
    
        int _b4x;
        int _b4y;
        
        int _displayText;
    
        float _biasList[256];
        float _powBiasList[256];
        
};
