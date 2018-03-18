#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
    output.listOutputDevices();
  //  output.device
    int deviceID = -1;
	//for(int i=0; i<20; i++){
    bool done = false;
    int i = 0;
    
//    vector<AudioDeviceID> deviceList = unitUtil.AudioOutputDeviceList();
//	
//	for(int i = 0; i < deviceList.size(); i++) {
//		cout << "ID[" << deviceList[i] << "]  \t" << "Name[" << AudioDeviceName(deviceList[i]) << "]\n";
//	}
//    
   // while(done == false){
    cout<<"output.outputDeviceAmount "<<output.outputDeviceAmount<<endl;
    
    for(int i=0; i<output.outputDeviceAmount; i++){
        int temp_id = output.deviceIDArray[i];
        string temp_name = output.deviceNameArray[i];
        
        cout<<i<<" , ID = "<<temp_id<<" , name = "<<temp_name<<endl;
        if(temp_name == "Built-in Output") deviceID = temp_id;
        //if(temp_name == "Aggregate Device") deviceID = temp_id;
        if(temp_id == -1 || temp_id > 1000) done = true;
        //i++;
    }
    
    sound.setFile(ofFilePath::getAbsolutePath("sounds/test.wav"));
    sound.connectTo(tap);
    tap.connectTo(mixer, 0);
    mixer.connectTo(output);
    
    mixer.setInputVolume(1, 0);
    mixer.setPan(0.5,0);
    
    output.setDevice(deviceID);
    bool b_start = output.start();
    
    sound.loop();
    
    ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //	The ofxAudioUnitTap has a function for getting an ofPolyline
    //	representing the waveform of the samples currently passing
    //	through it
    
	tap.getLeftWaveform(waveform, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(170);
	ofSetColor(20);
	waveform.draw();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
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
