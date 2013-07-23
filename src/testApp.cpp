#include "testApp.h"

AudioInputManager aim;

double p0;
double p1;
double p2;
double p3;
double p4;
double p5;

string boin;

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetFrameRate(30);
    
    ofBackground(28, 28, 28);
    ofSetBackgroundAuto(true);
    
    // --- Store Window Size. ---
    _width  = ofGetWidth();
    _height = ofGetHeight();
    _halfWidth  = _width / 2;
    _halfHeight = _height / 2;
    
    ofSoundStreamListDevices();
    
    // --- Sound Setup. ---
    ofSoundStreamSetup( 0, 2, this, 44100, 512, 4 );
    aim.setup(512);
    
    _sender.setup("127.0.0.1", 10000);
    
    _displayText = 0;
    
    _mode        = 0;
    
    _gainNorm = 64.0;
    
    _averageStart = 0;
    _averageEnd   = 256.0;
    _averageNorm  = 256.0;
    
    for( int i = 0; i < 256; i++ ){
        _biasList[i] = 6.0;
        _powBiasList[i] = 6.0;
    }
    
    return;
    
    /*
    p0 = 0;
    p1 = 0;
    p2 = 0;
    p3 = 0;
    p4 = 0;
    //*/
    
}

//--------------------------------------------------------------
void testApp::update(){
	
    if( _updated == true ){
        
        ofxOscMessage m;
        
        // ----- GAIN -----
        
        _gain = ( aim.maxSignalValue * 30 ) / _gainNorm;
        
        m.setAddress("/audio/gain");
        m.addFloatArg( _gain );
        _sender.sendMessage(m);
        
        m.clear();
        
        // ----- WAVE -----
        
        m.setAddress("/audio/wave");
        m.addIntArg( 128 );
        for( int i = 0; i < 128; i++ ){
            m.addFloatArg( aim.inputSignal[i] );
        }
        _sender.sendMessage(m);
        
        m.clear();
        
        // ----- FFT -----
        
        m.setAddress("/audio/fft");
        m.addIntArg( 128 );
        for( int i = 0; i < 128; i++ ){
            m.addFloatArg( ( aim.audioInputFFT[i] * _powBiasList[i] ) / 256.0 );
        }
        _sender.sendMessage(m);
        
        m.clear();
        
        // ----- FFT AVERAGE -----
        
        _fftAverageRatio = aim.getFFTAverageOfRange(_averageStart,_averageEnd ) / _averageNorm * 6.0;
        
        m.setAddress("/audio/fft/average");
        m.addFloatArg( _fftAverageRatio );
        _sender.sendMessage(m);
        
        m.clear();
        
        /*
        // ----- BOIN ------
        m.setAddress("/audio/boin");
        m.addStringArg(boin);
        _sender.sendMessage(m);
        m.clear();
        //*/
        
        _updated = false;
        
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    // --- draw FFT Average.
    
    ofSetColor(64 + 32 * _fftAverageRatio, 0, 0);
    ofRect(_averageStart, _height - _height * _fftAverageRatio, _averageEnd - _averageStart, _height * _fftAverageRatio );
    
    ofSetColor( 128, 0, 0);
    ofRect(_averageStart, _height - _averageNorm, _averageEnd - _averageStart, 1 );
    ofRect(_averageStart, 0, 1, _height );
    ofRect(_averageEnd, 0, 1, _height );
    
    // --- draw gain.
    
    ofRect(288, _halfHeight - _gainNorm, 256, 1 );
    ofRect(288, _halfHeight + _gainNorm, 256, 1 );
    
    // --- draw Equalizer.
    
    ofSetColor(255, 255, 255);
    for( int i =0; i < 256; i++ ){
        ofRect(i, _height, 1, - ( aim.audioInputFFT[i] * _biasList[i] ) );
        ofRect(288 + i, _halfHeight + aim.inputSignal[i] * 30, 1, 1 );
    }
    
    // --- draw volume
    
    ofSetColor( 0, 128, 0);
    ofRect( 257, _height - _height * aim.volume / 100.0, 30, 1 );
    ofSetColor( 0, 64, 0);
    ofRect(257, 0, 1, _height );
    ofRect(287, 0, 1, _height );
    
    // --- split line
    
    ofSetColor(0, 0, 0);
    //ofRect(0, _halfHeight, _width, 1 );
    ofRect(256, 0, 1, _height );
    ofRect(288, 0, 1, _height );
    
    return;
    
    // --- draw biasList.
    
    /*
    
    ofSetColor(0, 0, 255);
    for( int i =0; i < 256; i++ ){
        ofRect( i, _height - 128.0 * _biasList[i], 1, 1 );
    }
    
    if( _displayText == 1 ){
        
        if( _mode == 1 ){
            ofSetColor(230);
            ofDrawBitmapString("[B]bias", 5, 13 );
        }else{
            ofSetColor(128);
            ofDrawBitmapString("[B]bias", 5, 13 );
        }
        
        if( _mode == 2 ){
            ofSetColor(230);
            ofDrawBitmapString("[G]gain", 70, 13 );
        }else{
            ofSetColor(128);
            ofDrawBitmapString("[G]gain", 70, 13 );
        }
        
    }
    
    return;
    
    /*
    
    // --- 母音認識 --------------------------------
    
    //if( _gain < 0.2 ) return;
    
    ofSetColor(255,255,255);
    
    p0 *= 0.2;
    p1 *= 0.2;
    p2 *= 0.2;
    p3 *= 0.2;
    p4 *= 0.2;
    p5 *= 0.2;
    
    double _p0 = aim.getSum(0, 9) / aim.fftTotal * 100;
    double _p1 = aim.getSum(6, 19) / aim.fftTotal * 100;

    double _p2 = aim.getSum(20, 29) / aim.fftTotal * 100;
    
    double _p3 = aim.getSum(40, 49) / aim.fftTotal * 100;
    
    double _p4 = aim.getSum(0, 4) / aim.fftTotal * 100;
    double _p5 = aim.getSum(5, 9) / aim.fftTotal * 100;
    
    if( p0 < _p0 || p0 == INFINITY ) p0 = _p0;
    if( p1 < _p1 || p1 == INFINITY ) p1 = _p1;
    if( p2 < _p2 || p2 == INFINITY ) p2 = _p2;
    if( p3 < _p3 || p3 == INFINITY ) p3 = _p3;
    if( p4 < _p4 || p4 == INFINITY ) p4 = _p4;
    if( p5 < _p5 || p5 == INFINITY ) p5 = _p5;
    
    if( _p1 < _p3 * 1.1 && 10.0 < _p3 * 1.1 && _p2 < 10.0 ){
        boin = "i";
    }else if( 28.0 < (_p2+_p3) ){
        boin = "e";
    }else if( _p0 <= _p1 ){
        boin = "a";
    }else{
        if( _p5 * 2.0 <= _p4 ){
            boin = "u";
        }else{
            boin = "o";
        }
    }
    
    ofDrawBitmapString( boin, _width - 12, _height - 4 );
    
    /*
    ofDrawBitmapString( ofToString( p0 ), 100,70 );
    ofDrawBitmapString( ofToString( p1 ), 100,80 );
    ofDrawBitmapString( ofToString( p2 ), 100,90 );
    ofDrawBitmapString( ofToString( p3 ), 100,100 );
    ofDrawBitmapString( ofToString( p4 ), 100,110 );
    ofDrawBitmapString( ofToString( p5 ), 100,120 );
    //*/
    
}

//--------------------------------------------------------------
void testApp::audioReceived(float * input, int bufferSize, int nChannels){
    if( _updated == false ){
        aim.update( input, bufferSize, nChannels );
        _updated = true;
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key==357){
        if( aim.volume < 100.0 ){
            aim.volume += 1.0;
        }
    }else if(key==359){
        if( 0.0 < aim.volume ){
            aim.volume -= 1.0;
        }
    }
    /*
    if( key == 32 ){
        if( _displayText == 0 ){
            _displayText = 1;
        }else{
            _displayText = 0;
        }
    }
    /*
    if( key == 97 ){ // A
        _mode = 2;
    }else if( key == 98 ){
        _mode = 1;
    }else if( key == 103 ){ // G
        _mode = 3;
    }
    */
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    //_mode = 0;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
    if( y < 0 ) y = 0;
    if( _height <= y ) x = _height - 1;
    
    if( _mode == 0 ){
        
        aim.volume = 100.0 - 100.0 * y / _height;
        
    }else if( _mode == 1 ){
        
        // 範囲内におさめる.
        if( x < 0 ) x = 0;
        if( 256 <= x ) x = 256 - 1;
        
        if( x < _averageDragStartX ){
            _averageStart = x;
            _averageEnd   = _averageDragStartX;
        }else{
            _averageStart = _averageDragStartX;
            _averageEnd   = x;
        }
        
        if( _averageStart == _averageEnd ){
            if( _averageStart == 0 ){
                _averageEnd++;
            }else{
                _averageStart--;
            }
        }
        
        _averageNorm = _height - y;
        
        if( _averageNorm < 1.0 ){
            _averageNorm = 1.0;
        }
        
    }else{
        
        _gainNorm = abs( _halfHeight - y );
        
        if( _gainNorm < 1.0 ){
            _gainNorm = 1.0;
        }
        
    }
    
    /*基本 bias はもう使わないでいく.
    {
        
        _gainNorm = abs( _halfHeight - y );
        
        if( _b4x <= x ){
            for( int i = x; _b4x < i; i-- ){
                _biasList[i] = ( 256.0 - y ) / 128.0;
                _powBiasList[i] = pow(_biasList[i],2);
            }
        }else{
            for( int i = x; i < _b4x; i++ ){
                _biasList[i] = ( 256.0 - y ) / 128.0;
                _powBiasList[i] = pow(_biasList[i],2);
            }
        }
        
        _b4x = x;
        _b4y = y;
        
    }
    */
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    _b4x = x;
    _b4y = y;
    if( x < 256 ){
        _averageDragStartX = x;
        _mode = 1;
    }else if( 288 < x ){
        _mode = 2;
    }else{
        _mode = 0;
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    // --- Store Window Size. ---
    _width  = ofGetWidth();
    _height = ofGetHeight();
    _halfWidth  = _width / 2;
    _halfHeight = _height / 2;
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}