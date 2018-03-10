#include "ofxAudioUnitOutput.h"
#include "ofxAudioUnitUtils.h"
#include "ofxAudioUnitHardwareUtils.h"
#include <CoreFoundation/CoreFoundation.h>



AudioComponentDescription outputDesc = {
	kAudioUnitType_Output,
#if TARGET_OS_IPHONE
	kAudioUnitSubType_RemoteIO,
#else
	kAudioUnitSubType_HALOutput,
#endif
	kAudioUnitManufacturer_Apple
};


// ----------------------------------------------------------
ofxAudioUnitOutput::ofxAudioUnitOutput()
// ----------------------------------------------------------
{
	_desc = outputDesc;
	initUnit();
}

// ----------------------------------------------------------
bool ofxAudioUnitOutput::start()
// ----------------------------------------------------------
{
	OFXAU_RET_BOOL(AudioOutputUnitStart(*_unit), "starting output unit");
}

// ----------------------------------------------------------
bool ofxAudioUnitOutput::stop()
// ----------------------------------------------------------
{
	OFXAU_RET_BOOL(AudioOutputUnitStop(*_unit), "stopping output unit");
}

#pragma mark - Hardware

#if !TARGET_OS_IPHONE

// ----------------------------------------------------------
bool ofxAudioUnitOutput::setDevice(AudioDeviceID deviceID)
// ----------------------------------------------------------
{
	UInt32 deviceIDSize = sizeof(deviceID);
	OFXAU_RET_BOOL(AudioUnitSetProperty(*_unit,
										kAudioOutputUnitProperty_CurrentDevice,
										kAudioUnitScope_Global,
										0,
										&deviceID,
										deviceIDSize),
				   "setting output unit's device ID");
}

// ----------------------------------------------------------
bool ofxAudioUnitOutput::setDevice(const std::string &deviceName)
// ----------------------------------------------------------
{
    vector<AudioDeviceID> outputDevices = AudioOutputDeviceList();
	AudioDeviceID deviceID;
	bool found = false;
	for(int i = 0; i < outputDevices.size(); i++) {
		int diff = AudioDeviceName(outputDevices[i]).compare(deviceName);
		if(!diff) {
			deviceID = outputDevices[i];
			found = true;
			break;
		}
	}
	
	if(found) {
		return setDevice(deviceID);
	} else {
		return false;
	}
}

//stephan schulz edits
int ofxAudioUnitOutput::getOutChannelAmount(){
    UInt32 propertySize;
    Boolean writable = false;
    OSStatus status = AudioUnitGetPropertyInfo(*_unit,
                                               kAudioOutputUnitProperty_ChannelMap,
                                               kAudioUnitScope_Output,
                                               0,
                                               &propertySize, &writable);
    //SignalIf_(writable == false);
    //cout<<"writable "<<writable<<endl;
    
    
    long nChannels = propertySize / sizeof(SInt32);
    
    return nChannels;
}
// ----------------------------------------------------------
bool ofxAudioUnitOutput::setDevice(AudioDeviceID deviceID, std::vector<int> _channelInputMap)
// ----------------------------------------------------------
{
    
    
    
    //I do this (error-handling removed for clarity):
    //http://lists.apple.com/archives/coreaudio-api/2004/Feb/msg00314.html
    //http://lists.apple.com/archives/coreaudio-api/2006/Sep/msg00153.html
    
    UInt32 propertySize;
    Boolean writable = false;
    OSStatus status = AudioUnitGetPropertyInfo(*_unit,
                                               kAudioOutputUnitProperty_ChannelMap,
                                               kAudioUnitScope_Output,
                                               0,
                                               &propertySize, &writable);
    //SignalIf_(writable == false);
    //cout<<"writable "<<writable<<endl;
    
    
    long nChannels = propertySize / sizeof(SInt32);
    long* channelMapPtr = (long*)malloc(propertySize);
    
    cout<<"output device has/found "<<nChannels<<" channels \n";
//    ofLog()<<"output device has/found "<<nChannels<<" channels ";
    UInt32 scratch = propertySize;
    status = AudioUnitGetProperty(*_unit,
                                  kAudioOutputUnitProperty_ChannelMap,
                                  kAudioUnitScope_Output,
                                  0,
                                  channelMapPtr,
                                  &scratch);
    
    //  channelMapPtr[0] = 0;
    for (long i = 0; i < nChannels; i++)
    {
        //tell the output channels which input channels to play
        // channelMapPtr[i] = 0; // //
        channelMapPtr[i] = _channelInputMap[i];
        // cout<<"out "<<i<<" gets in "<<_channelInputMap[i]<<endl;
    }
    
    //   channelMapPtr[3] = 0;
    //   channelMapPtr[5] = 1;
    
    OFXAU_RET_BOOL(AudioUnitSetProperty(*_unit,
                                        kAudioOutputUnitProperty_ChannelMap,
                                        kAudioUnitScope_Output,
                                        0,
                                        channelMapPtr,
                                        scratch),"setting output unit's device ID");
    
    free((void *)channelMapPtr);
    
    
}


// ----------------------------------------------------------
void ofxAudioUnitOutput::listOutputDevices()
// ----------------------------------------------------------
{
	vector<AudioDeviceID> deviceList = AudioOutputDeviceList();
     outputDeviceAmount = deviceList.size();
	for(int i = 0; i < deviceList.size(); i++) {
        
        deviceIDArray.push_back(deviceList[i]) ;
        
        deviceNameArray.push_back(AudioDeviceName(deviceList[i]));
        
        cout << "ID[" << deviceList[i] << "]  \t" << "Name[" << AudioDeviceName(deviceList[i]) << "]\n";
//        ofLog()<< "ID[" << deviceList[i] << "]  \t" << "Name[" << AudioDeviceName(deviceList[i]) << "]\n";
	}
    
//    return deviceList.size();
}

#endif // !TARGET_OS_IPHONE
