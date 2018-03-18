#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"

#ifdef __OBJC__
#import <AVFoundation/AVFoundation.h>
#import <webrtcvad/webrtcvad.h>
//#import </Applications/openFrameworks-master/addons/ofxAudioUnit/example-12-recordToFile_playthrough_VAD/src/webrtcvad/webrtcvad/webrtcvad.h>
class ofApp;
@interface RecordDelegate : NSObject<AVAudioRecorderDelegate>
{
    ofApp * _app;
}
@property (nonatomic, assign) ofApp * app;
@end
#else
typedef void AVCaptureAudioFileOutput;
typedef void RecordDelegate;
typedef void AVAudioRecorder;
#endif

class ofApp : public ofBaseApp {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void startRecording(string filePath);
	void stopRecording();
	void finishedRecording(string filePath);
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
    bool checkForVoice(string filePath);
    
	//ofxAudioUnitMatrixMixer matrixMixer;
    //ofxAudioUnitMixer mixer;
    
	AVAudioRecorder * recorder;
	RecordDelegate * recordDelegate;
    
    bool isRecording;
    string newFileName;
    ofxAudioUnitOutput output;
    	ofxAudioUnitInput input;
    ofxAudioUnitFilePlayer filePlayer;
};
