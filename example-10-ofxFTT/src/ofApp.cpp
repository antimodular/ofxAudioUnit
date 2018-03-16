#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	

    //---ofxFFT
    plotHeight = 128;
    bufferSize = 2048; //ofxAudioUnit is set for 2048 samplesToBuffer
    
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);
    // To use FFTW, try:
    //fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);
    
    drawBins.resize(fft->getBinSize());
    middleBins.resize(fft->getBinSize());
    audioBins.resize(fft->getBinSize());
    
    // the fft needs to be smoothed out, so we create an array of floats
    // for that purpose:
//    fftSmoothed = new float[bufferSize];
//    for (int i = 0; i < bufferSize; i++){
//        fftSmoothed[i] = 0;
//    }
    
    activeUnit = NULL;
    
    input.connectTo(tap).connectTo(output);
    input.start();
    activeUnit = &input;
    
    output.start();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //--ofxAudioUnit
    tap.getStereoWaveform(oscLineLeft, oscLineRight, ofGetWidth(), ofGetHeight()/4);
    
    
    vector<Float32> outData;
    tap.getLeftSamples(outData);
    
    //--ofxFFT
    
    if(outData.size() > 0){
        float maxValue = 0;
        for(int i = 0; i <bufferSize; i++) {
            if(abs(outData[i]) > maxValue) {
                maxValue = abs(outData[i]);
            }
        }
        for(int i = 0; i < bufferSize; i++) {
            outData[i] /= maxValue;
        }
        
        fft->setSignal(outData);
        
        float* curFft = fft->getAmplitude();
        memcpy(&audioBins[0], curFft, sizeof(float) * fft->getBinSize());
        
        maxValue = 0;
        for(int i = 0; i < fft->getBinSize(); i++) {
            if(abs(audioBins[i]) > maxValue) {
                maxValue = abs(audioBins[i]);
            }
        }
        for(int i = 0; i < fft->getBinSize(); i++) {
            audioBins[i] /= maxValue;
        }
        
        soundMutex.lock();
        middleBins = audioBins;
        soundMutex.unlock();
        
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //--draw regular ofxAudioUnit waves
	ofBackground(0,50,10);
	drawOscilloscopeGrid();
	drawOscilloscopeLines();
	drawOscilloscopeLabels();
	
   
    //--draw FFT
    ofSetColor(255);
    ofPushMatrix();
    ofTranslate(16, ofGetHeight()/2);
    
    soundMutex.lock();
    drawBins = middleBins;
    soundMutex.unlock();
    
    ofDrawBitmapString("Frequency Domain", 0, 0);
    plot(drawBins, -plotHeight, plotHeight / 2);
    ofPopMatrix();
    string msg = ofToString((int) ofGetFrameRate()) + " fps";
    ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);
    

    //--info
	ofSetColor(255);
	if(activeUnit){
		ofDrawBitmapString("Press space to show the Audio Unit's UI", ofPoint(25,25));
	}
	else {
		ofDrawBitmapString("You need to un-comment one of the blocks in\
						   setup() to generate some audio", ofPoint(25,25));
	}
	
}

void ofApp::plot(vector<float>& buffer, float scale, float offset) {
    ofNoFill();
    int n = buffer.size();
    ofDrawRectangle(0, 0, n, plotHeight);
    glPushMatrix();
    glTranslatef(0, plotHeight / 2 + offset, 0);
    ofBeginShape();
    for (int i = 0; i < n; i++) {
        ofVertex(i, sqrt(buffer[i]) * scale);
    }
    ofEndShape();
    glPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == ' ' && activeUnit)
	{
		activeUnit->showUI();
	}

}

//--------------------------------------------------------------
void ofApp::drawOscilloscopeGrid()
{
	ofSetColor(0, 40, 0);
	ofSetLineWidth(1);
	float step = float(ofGetHeight()) / 64;
	for(int x = 0; x < ofGetWidth(); x += step)
	{
		ofDrawLine(x, 0, x, ofGetHeight());
	}
	for(int y = 0; y < ofGetHeight(); y += step)
	{
		ofDrawLine(0, y, ofGetWidth(), y);
	}
}

//--------------------------------------------------------------
void ofApp::drawOscilloscopeLabels()
{
	ofSetColor(0, 200, 40);
	ofDrawBitmapString("Left Channel", 25, ofGetHeight() / 8 - 40);
	ofDrawBitmapString("Right Channel", 25, ofGetHeight() - (ofGetHeight() / 8 - 40));
	for(int y = 0; y < ofGetHeight(); y += ofGetHeight() / 2)
	{
		ofPushMatrix();
		{
			ofTranslate(0, y);
			
			ofTranslate(0, ofGetHeight() / 8);
			ofDrawBitmapString("+1", 0, 0);
			ofDrawBitmapString("+1", ofGetWidth() - 20, 0);
			
			ofTranslate(0, ofGetHeight() / 8);
			ofDrawBitmapString("0", 3, 0);
			ofDrawBitmapString("0", ofGetWidth() - 15, 0);
			
			ofTranslate(0, ofGetHeight() / 8);
			ofDrawBitmapString("-1", 0, 0);
			ofDrawBitmapString("-1", ofGetWidth() - 20, 0);
		}
		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void ofApp::drawOscilloscopeLines()
{
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(60, 150, 80, 175);
	for(int i = 1; i < 7; i++)
	{
		ofSetLineWidth(i);
		ofPushMatrix();
		{
			ofTranslate(0, ofGetHeight()/8 - 5);
			oscLineLeft.draw();
			ofTranslate(0, ofGetHeight()/2);
			oscLineRight.draw();
		}
		ofPopMatrix();
	}
	ofDisableBlendMode();
	
	ofSetColor(255);
	ofSetLineWidth(2);
	ofPushMatrix();
	{
		ofTranslate(0, ofGetHeight()/8 - 5);
		oscLineLeft.draw();
		ofTranslate(0, ofGetHeight()/2);
		oscLineRight.draw();
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
