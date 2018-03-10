#pragma once

#include "ofxAudioUnitBase.h"

#include "ofUtils.h"
#include <iostream>

// ofxAudioUnitOutput wraps the AUHAL output unit on OSX
// and the RemoteIO unit on iOS

// This unit drives the "pull" model of Core Audio and
// sends audio to the actual hardware (ie. speakers / headphones)

class ofxAudioUnitOutput : public ofxAudioUnit
{
public:
	ofxAudioUnitOutput();
	~ofxAudioUnitOutput(){stop();}
	
	bool start();
	bool stop();
	
#if !TARGET_OS_IPHONE
	bool setDevice(AudioDeviceID deviceID);
	bool setDevice(const std::string &deviceName);

	 void listOutputDevices();
    
    //stephan schulz edits
    bool setDevice(AudioDeviceID deviceID, std::vector<int> _channelInputMap);
    vector<int> deviceIDArray;
    vector<string> deviceNameArray;
    int outputDeviceAmount;
    
    int getOutChannelAmount();
#endif
};
