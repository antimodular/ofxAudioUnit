#include "ofxAudioUnitOutput.h"
#include "ofxAudioUnitUtils.h"
#include "ofxAudioUnitHardwareUtils.h"
#include <CoreFoundation/CoreFoundation.h>

AudioComponentDescription outputDesc = {
	kAudioUnitType_Output,
#if TARGET_OS_IPHONE
	kAudioUnitSubType_RemoteIO,
#else
	kAudioUnitSubType_HALOutput, //kAudioUnitSubType_DefaultOutput, //
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
    /*
    AudioStreamBasicDescription        formatDesc;
    UInt32 dataSize = sizeof(formatDesc);
    int err = AudioDeviceGetProperty(34, 0, false, kAudioDevicePropertyStreamFormat, &dataSize, &formatDesc);

    cout<<"mSampleRate "<<formatDesc.mSampleRate<<endl;
    cout<<"mFormatID "<<formatDesc.mFormatID<<endl;
    cout<<"mFormatFlags "<<formatDesc.mFormatFlags<<endl;
    cout<<"mChannelsPerFrame "<<formatDesc.mChannelsPerFrame<<endl;
    cout<<"mFramesPerPacket "<<formatDesc.mFramesPerPacket<<endl;
    cout<<"mBitsPerChannel "<<formatDesc.mBitsPerChannel<<endl;
    cout<<"mBytesPerPacket "<<formatDesc.mBytesPerPacket<<endl;
    cout<<"mBytesPerFrame "<<formatDesc.mBytesPerFrame<<endl;
*/
    /*
    AudioStreamBasicDescription ASBD = {
        .mSampleRate       = 44100,
        .mFormatID         = kAudioFormatLinearPCM,
        .mFormatFlags      = kAudioFormatFlagsNativeFloatPacked,
        .mChannelsPerFrame = 1,
        .mFramesPerPacket  = 1,
        .mBitsPerChannel   = sizeof(Float32) * 8,
        .mBytesPerPacket   = sizeof(Float32),
        .mBytesPerFrame    = sizeof(Float32)
    };
   
    AudioUnitSetProperty(*_unit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output,0,&ASBD,sizeof(ASBD));

     */
    cout<<"starting up"<<endl;
   	OFXAU_RET_BOOL(AudioOutputUnitStart(*_unit), "starting output unit");
    cout<<"done starting up"<<endl;
    

    
}

// ----------------------------------------------------------
bool ofxAudioUnitOutput::stop()
// ----------------------------------------------------------
{
	OFXAU_RET_BOOL(AudioOutputUnitStop(*_unit), "stopping output unit");
}

#pragma mark - Hardware

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
        
        cout<<"output device has/found "<<nChannels<<" channels "<<endl;
        
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
bool ofxAudioUnitOutput::setDevice(const string &deviceName)
// ----------------------------------------------------------
{
	std::vector<AudioDeviceID>  outputDevices = AudioOutputDeviceList();
    
   
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
	}
}
