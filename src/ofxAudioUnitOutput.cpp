#include "ofxAudioUnit.h"

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
: _isReady(false)
// ----------------------------------------------------------
{
	_desc = outputDesc;
	initUnit();
}

// ----------------------------------------------------------
bool ofxAudioUnitOutput::start(int deviceID)
// ----------------------------------------------------------
{
    cout<<"ofxAudioUnitOutput::start() "<<deviceID<<endl;
    if(!_isReady) _isReady = configureOutputDevice(deviceID);
    if(!_isReady) return false;
    
	OFXAU_RET_BOOL(AudioOutputUnitStart(*_unit), "starting output unit");
}

// ----------------------------------------------------------
bool ofxAudioUnitOutput::stop()
// ----------------------------------------------------------
{
	OFXAU_RET_BOOL(AudioOutputUnitStop(*_unit), "stopping output unit");
}

#pragma mark - Configuration

// ----------------------------------------------------------
bool ofxAudioUnitOutput::configureOutputDevice(int deviceID)
// ----------------------------------------------------------
{
    
    OSStatus    err = noErr;
    UInt32      outSize;
    Boolean     outWritable;
    
    err = AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDevices, &outSize, &outWritable);
    if ( err != noErr ) cout<<"err 1"<<endl;
    UInt16 devicesAvailable = outSize / sizeof(AudioDeviceID);
    if ( devicesAvailable < 1 )
    {
        fprintf(stderr, "No devices\n" );
        //return err;
    }else{
        
        cout<<"*devicesAvailable "<<devicesAvailable<<endl;
    }
    
	UInt32 on  = 1;
    UInt32 off = 0;
	OFXAU_RET_FALSE(AudioUnitSetProperty(*_unit,
										 kAudioOutputUnitProperty_EnableIO,
										 kAudioUnitScope_Input,
										 1,
										 &off,
										 sizeof(off)),
					"disabling input on HAL unit");
	
	
	OFXAU_RET_FALSE(AudioUnitSetProperty(*_unit,
										 kAudioOutputUnitProperty_EnableIO,
										 kAudioUnitScope_Output,
										 0,
										 &on,
										 sizeof(on)),
					"enabling output on HAL unit");
	
	AudioDeviceID outputDeviceID = kAudioObjectUnknown;
    
	UInt32 deviceIDSize = sizeof( AudioDeviceID );
	AudioObjectPropertyAddress prop_addr = {
		kAudioHardwarePropertyDefaultOutputDevice,
		kAudioObjectPropertyScopeGlobal,
		kAudioObjectPropertyElementMaster
	};
	
    
	OFXAU_RET_FALSE(AudioObjectGetPropertyData(kAudioObjectSystemObject,
											   &prop_addr,
											   0,
											   NULL,
											   &deviceIDSize,
											   &outputDeviceID),
					"getting device ID for default input");
	
    if(deviceID != -1) outputDeviceID = deviceID;
    
	OFXAU_RET_FALSE(AudioUnitSetProperty(*_unit,
										 kAudioOutputUnitProperty_CurrentDevice,
										 kAudioUnitScope_Global,
										 0,
										 &outputDeviceID,
										 deviceIDSize),
					"setting HAL unit's device ID");
	cout<<"outputDeviceID "<<outputDeviceID<<endl;
	
	OFXAU_RET_BOOL(AudioUnitInitialize(*_unit),
				   "initializing hardware input unit after setting it to input mode");
    
    
}

