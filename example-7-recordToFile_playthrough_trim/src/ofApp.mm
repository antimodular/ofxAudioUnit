#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0);
    

	
	isRecording = false;
    
   // mixer.setInputBusCount(2);
    input.listInputDevices();
 	input.start();
    input.setDevice("Built-in Microphone");
    input.connectTo(output);

    output.start();
    
    recordDelegate = [[RecordDelegate alloc] init];
	recordDelegate.app = this;
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);
    ofDrawBitmapString("press r to record, s to stop, p to replay", 10,10);
    
    if(isRecording){
        ofSetColor(255, 0, 0);
        ofDrawBitmapString("is recording", 10,30);
    }else{
        ofSetColor(0, 0, 255);
        ofDrawBitmapString("is NOT recording", 10,30);
    }
}

void ofApp::exit(){

}

void ofApp::startRecording(string filePath) {
	
	// converting file path from C++ string to NSString
	NSString * fileString = [NSString stringWithUTF8String:filePath.c_str()];
	
	// declaring settings for recorded file
	NSDictionary * fileSettings = @{
		AVFormatIDKey : @(kAudioFormatMPEG4AAC),
		AVSampleRateKey : @(44100),
		AVNumberOfChannelsKey : @(2)
	};
	
	if(recorder)
	{
		[recorder release];
	}
	
	NSError * err;
	recorder = [[AVAudioRecorder alloc] initWithURL:[NSURL fileURLWithPath:fileString]
										   settings:fileSettings
											  error:&err];
	
	if(err){
		NSLog(@"Error when setting up recorder : %@", err);
	} else {
		[recorder setDelegate:recordDelegate];
        //[recorder recordForDuration:(10)];
		[recorder record];
	}
}

void ofApp::stopRecording() {
	[recorder stop];
}

void ofApp::finishedRecording(string fileName) {
	cout << "Recorded to " << fileName << endl;
}

void ofApp::keyPressed(int key) {
    if(key == 'r'){
        isRecording = true;
        newFileName = ofGetTimestampString()+".m4a";
     startRecording(ofToDataPath(newFileName));
    }
	if(key == 's'){
        isRecording = false;
    stopRecording();
    }
    if(key == 'p'){
       
        
        filePlayer.setFile(ofToDataPath(newFileName));
         UInt32 framesToPlay = filePlayer.getLength();
        cout<<"file frame length "<<framesToPlay<<endl;
        framesToPlay = framesToPlay*2; ///2;// replace with however many frame you want to trim
         filePlayer.setLength(framesToPlay);
        filePlayer.loop();
        filePlayer.connectTo(output);
    }
}

void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}

@implementation RecordDelegate

@synthesize app = _app;

- (void)audioRecorderDidFinishRecording:(AVAudioRecorder *)recorder successfully:(BOOL)flag
{
	// notifying the oF app that the audio file is ready
	_app->finishedRecording([[[recorder url] path] UTF8String]);
}

@end
