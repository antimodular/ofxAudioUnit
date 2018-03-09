#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"

#ifdef __OBJC__
#import <AVFoundation/AVFoundation.h>
class testApp;
@interface RecordDelegate : NSObject<AVAudioRecorderDelegate>
{
	testApp * _app;
}
@property (nonatomic, assign) testApp * app;
@end
#else
typedef void AVCaptureAudioFileOutput;
typedef void RecordDelegate;
#endif

class testApp : public ofBaseApp {
	
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
