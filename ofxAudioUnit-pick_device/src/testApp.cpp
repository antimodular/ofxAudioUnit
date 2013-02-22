#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
    mixer.AudioUnitGetDeviceList();
    
    int deviceID = -1;
	//for(int i=0; i<20; i++){
    bool done = false;
    int i = 0;
    while(done == false){
        int temp_id = mixer.deviceIDArray[i];
        string temp_name = mixer.deviceNameArray[i];
        
        cout<<i<<" , ID = "<<temp_id<<" , name = "<<temp_name<<endl;
        if(temp_name == "Built-in Output") deviceID = temp_id;
        if(temp_id == -1) done = true;
        i++;
    }
    
    sound.setFile(ofFilePath::getAbsolutePath("sounds/test.wav"));
    sound.connectTo(tap);
    tap.connectTo(mixer, 0);
    mixer.connectTo(output);
    
    mixer.setInputVolume(1, 0);
    mixer.setPan(0.5,0);
    
    bool b_start = output.start(deviceID);
    
    sound.loop();
    
    ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::update(){
    
    //	The ofxAudioUnitTap has a function for getting an ofPolyline
    //	representing the waveform of the samples currently passing
    //	through it
    
	tap.getLeftWaveform(waveform, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(170);
	ofSetColor(20);
	waveform.draw();
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}
