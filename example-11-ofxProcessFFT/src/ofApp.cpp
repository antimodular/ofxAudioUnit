#include "ofApp.h"

//this example uses custom ofxFft with changes to ofxProcessFFT
//https://github.com/antimodular/ofxFft/tree/dev_noMic_easyFft

//--------------------------------------------------------------
void ofApp::setup(){
	
	activeUnit = NULL;
	
    input.connectTo(tap).connectTo(output);
	input.start();
	activeUnit = &input;

	output.start();
	ofSetVerticalSync(true);
	ofEnableSmoothing();
    
    
    //---ofxFFT
    bufferSize = 2048;
    
    fft.setup(false,bufferSize);
    fft.setNumFFTBins(100);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //--ofxAudioUnit
    tap.getStereoWaveform(oscLineLeft, oscLineRight, ofGetWidth(), ofGetHeight()/4);
    
    
    
    vector<Float32> outData;
    tap.getLeftSamples(outData);
    
    if(outData.size() > 0){
        
        float * curBuffer = (float*)malloc(bufferSize * sizeof(float));
        for(int i=0; i<outData.size();i++){
            curBuffer[i] = outData[i];
        }
//        float* curFft = fft->getAmplitude();
//        memcpy(&audioBins[0], curFft, sizeof(float) * fft->getBinSize());
        
        fft.fft.audioReceived(curBuffer, bufferSize, 1);
      
        fft.update();
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

//--process fft
    fft.drawBars();
    fft.drawDebug();
    
    ofNoFill();
    ofDrawRectangle(824, 0, 200, 200);
    ofDrawRectangle(824, 200, 200, 200);
    ofDrawRectangle(824, 400, 200, 200);
    ofDrawRectangle(824, 600, 200, 200);
    
    fft.drawHistoryGraph(ofPoint(824,0), LOW);
    fft.drawHistoryGraph(ofPoint(824,200),MID );
    fft.drawHistoryGraph(ofPoint(824,400),HIGH );
    fft.drawHistoryGraph(ofPoint(824,600),MAXSOUND );
    ofDrawBitmapString("LOW",850,20);
    ofDrawBitmapString("HIGH",850,420);
    ofDrawBitmapString("MID",850,220);
    ofDrawBitmapString("MAX VOLUME",850,620);
    
    ofSetColor(0);
    ofDrawBitmapString("Press 'r' or 'q' to toggle normalization of values", 20,320);
    
    //--info
	ofSetColor(255);
	if(activeUnit){
		ofDrawBitmapString("Press space to show the Audio Unit's UI", ofPoint(25,25));
	}
    
    
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == ' ' && activeUnit)
	{
		activeUnit->showUI();
	}

    if(key=='q'){
        fft.setVolumeRange(100);
        fft.setNormalize(false);
    }
    if(key=='r'){
        fft.setNormalize(true);
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
