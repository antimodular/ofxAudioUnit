#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	ofxAudioUnit compressor;
	ofxAudioUnit delay1,delay2;
	ofxAudioUnit distortion;
	ofxAudioUnit filter;
	ofxAudioUnit normalize;
    
	ofxAudioUnitFilePlayer source1, source2, source3;
	ofxAudioUnitMixer mixer;
	ofxAudioUnitOutput output;
	
	ofxAudioUnitTap tap1, tap2, tap3;
	ofPolyline wave1, wave2, wave3;
};
