#include "ofxAudioUnit.h"

AudioComponentDescription mixerDesc = {
	kAudioUnitType_Mixer,
	kAudioUnitSubType_MultiChannelMixer,
	kAudioUnitManufacturer_Apple
};

// ----------------------------------------------------------
ofxAudioUnitMixer::ofxAudioUnitMixer()
// ----------------------------------------------------------
{
	_desc = mixerDesc;
	initUnit();
	
	// default volume is 0, which can make things seem like they aren't working
	
	int busses = getInputBusCount();
	for(int i = 0; i < busses; i++)
		setInputVolume(1, i);
	
    cout<<"in_busses "<<busses<<endl;
    
    int out_busses = getOutputBusCount();
    cout<<"out_busses "<<out_busses<<endl;
    for(int i = 0; i < out_busses; i++)
        setOutputVolume(1,i);
}

#pragma mark - Volume / Pan

// ----------------------------------------------------------
void ofxAudioUnitMixer::setInputVolume(float volume, int bus)
// ----------------------------------------------------------
{
	OFXAU_PRINT(AudioUnitSetParameter(*_unit,
									  kMultiChannelMixerParam_Volume,
									  kAudioUnitScope_Input,
									  bus,
									  volume,
									  0),
				"setting mixer input gain");
}

// ----------------------------------------------------------
void ofxAudioUnitMixer::setOutputVolume(float volume, int bus)
// ----------------------------------------------------------
{
	OFXAU_PRINT(AudioUnitSetParameter(*_unit,
									  kMultiChannelMixerParam_Volume,
									  kAudioUnitScope_Output,
									  bus,
									  volume,
									  0),
				"setting mixer output gain");
}

// ----------------------------------------------------------
void ofxAudioUnitMixer::setPan(float pan, int bus)
// ----------------------------------------------------------
{
#ifndef __MAC_10_7
	static bool printedSetPanOSVersionMessage = false;
	if(!printedSetPanOSVersionMessage)
	{
		cout << "ofxAudioUnitMixer's setPan() is only available"
		<< " on OS X 10.7 and up" << endl;
		printedSetPanOSVersionMessage = true;
	}
#else
	OFXAU_PRINT(AudioUnitSetParameter(*_unit,
									  kMultiChannelMixerParam_Pan,
									  kAudioUnitScope_Input,
									  bus,
									  pan,
									  0),
				"setting mixer pan");
#endif
}

#pragma mark - Busses

// ----------------------------------------------------------
bool ofxAudioUnitMixer::setInputBusCount(unsigned int numberOfInputBusses)
// ----------------------------------------------------------
{
    cout<<"setInputBusCount "<<numberOfInputBusses<<endl;
	UInt32 busCount = numberOfInputBusses;
	OFXAU_RET_BOOL(AudioUnitSetProperty(*_unit,
										kAudioUnitProperty_ElementCount,
										kAudioUnitScope_Input,
										0,
										&busCount,
										sizeof(busCount)),
				   "setting number of input busses");
}

// ----------------------------------------------------------
unsigned int ofxAudioUnitMixer::getInputBusCount() const
// ----------------------------------------------------------
{
	UInt32 busCount;
	UInt32 busCountSize = sizeof(busCount);
	OFXAU_PRINT(AudioUnitGetProperty(*_unit,
									 kAudioUnitProperty_ElementCount,
									 kAudioUnitScope_Input,
									 0,
									 &busCount,
									 &busCountSize),
				"getting input bus count");
	return busCount;
}
// ----------------------------------------------------------
bool ofxAudioUnitMixer::setOutputBusCount(unsigned int numberOfOutputBusses)
// ----------------------------------------------------------
{
    AudioStreamBasicDescription outputASBD;
    UInt32 theSize = sizeof(outputASBD);
    //http://lists.apple.com/archives/coreaudio-api/2005/Jun/msg00164.html
    theSize = sizeof(outputASBD);
    AudioUnitGetProperty(*_unit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 1, &outputASBD, &theSize);
    printf("device format:\n");
    //outputASBD.Print();
    
    cout<<"mSampleRate "<<outputASBD.mSampleRate<<endl;
    cout<<"mFormatID "<<outputASBD.mFormatID<<endl;
    cout<<"mFormatFlags "<<outputASBD.mFormatFlags<<endl;
    cout<<"mChannelsPerFrame "<<outputASBD.mChannelsPerFrame<<endl;
    cout<<"mFramesPerPacket "<<outputASBD.mFramesPerPacket<<endl;
    cout<<"mBitsPerChannel "<<outputASBD.mBitsPerChannel<<endl;
    cout<<"mBytesPerPacket "<<outputASBD.mBytesPerPacket<<endl;
    cout<<"mBytesPerFrame "<<outputASBD.mBytesPerFrame<<endl;
    // outputASBD.m
    
    
    
    Float64 rate=0;
    theSize = sizeof(Float64);
    //you could probably pull this rate value straight from asbdDevice.mSampleRate, but here's another way if you have your InputDeviceID handy.
    AudioDeviceGetProperty(107, 0, 1,kAudioDevicePropertyNominalSampleRate, &theSize, &rate);
    outputASBD.mSampleRate = rate;
    cout<<"rate "<<rate<<endl;
    
    AudioDeviceGetProperty(107, 0, 1,kAudioDevicePropertyNominalSampleRate, &theSize, &rate);
    //outputASBD.mSampleRate = rate;
    cout<<"rate "<<rate<<endl;
    
    //Set the stream format of AUHAL to match the sample rate of the input device
    theSize = sizeof(outputASBD);
    AudioUnitSetProperty(*_unit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &outputASBD, theSize);
    
    
    cout<<"setOutputBusCount "<<numberOfOutputBusses<<endl;
	UInt32 busCount = numberOfOutputBusses;
    /*
     OFXAU_RET_BOOL(AudioUnitSetProperty(*_unit,
     kAudioUnitProperty_ElementCount,
     kAudioUnitScope_Output,
     0,
     &busCount,
     sizeof(busCount)),
     "setting number of output busses");
     */
    /*
     AudioStreamBasicDescription outputASBD = {
     .mSampleRate       = 32000,
     .mFormatID         = kAudioFormatLinearPCM,
     .mFormatFlags      = kAudioFormatFlagsAudioUnitCanonical,
     .mChannelsPerFrame = busCount,
     .mFramesPerPacket  = 1,
     .mBitsPerChannel   = sizeof(Float32) * 8,
     .mBytesPerPacket   = sizeof(Float32),
     .mBytesPerFrame    = sizeof(Float32)
     };
     */
    //AudioStreamBasicDescription outputASBD
    AudioStreamBasicDescription outputASBD2 = {
		.mSampleRate       = 44100, //32000 //48000
		.mFormatID         = kAudioFormatLinearPCM,
		.mFormatFlags      = kAudioFormatFlagsAudioUnitCanonical, //kAudioFormatFlagIsSignedInteger, //kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsNonMixable //kAudioFormatFlagsAudioUnitCanonical,
		.mChannelsPerFrame = busCount,
		.mFramesPerPacket  = 1,
		.mBitsPerChannel   = sizeof(Float32) * 8,
		.mBytesPerPacket   = sizeof(Float32),
		.mBytesPerFrame    = sizeof(Float32)
	};
    
    OFXAU_RET_BOOL(AudioUnitSetProperty(*_unit,
										kAudioUnitProperty_StreamFormat,
										kAudioUnitScope_Output,
										1,
										&outputASBD2,
										sizeof(outputASBD2)),
				   "setting number of output busses");
}

// ----------------------------------------------------------
unsigned int ofxAudioUnitMixer::getOutputBusCount() const
// ----------------------------------------------------------
{
	UInt32 busCount;
	UInt32 busCountSize = sizeof(busCount);
	OFXAU_PRINT(AudioUnitGetProperty(*_unit,
									 kAudioUnitProperty_ElementCount,
									 kAudioUnitScope_Output,
									 1,
									 &busCount,
									 &busCountSize),
				"getting output bus count");
	return busCount;
}

#pragma mark - Metering

// ----------------------------------------------------------
float ofxAudioUnitMixer::getInputLevel(int bus)
// ----------------------------------------------------------
{
	AudioUnitParameterValue level;
	OFXAU_PRINT(AudioUnitGetParameter(*_unit,
									  kMultiChannelMixerParam_PreAveragePower,
									  kAudioUnitScope_Input,
									  bus,
									  &level),
				"getting mixer input level");
	return level;
}

// ----------------------------------------------------------
float ofxAudioUnitMixer::getOutputLevel() const
// ----------------------------------------------------------
{
	AudioUnitParameterValue level;
	OFXAU_PRINT(AudioUnitGetParameter(*_unit,
									  kMultiChannelMixerParam_PreAveragePower,
									  kAudioUnitScope_Output,
									  0,
									  &level),
				"getting mixer output level");
	return level;
}

// ----------------------------------------------------------
void ofxAudioUnitMixer::enableInputMetering(int bus)
// ----------------------------------------------------------
{
	UInt32 on = 1;
	AudioUnitSetProperty(*_unit,
						 kAudioUnitProperty_MeteringMode,
						 kAudioUnitScope_Input,
						 bus,
						 &on,
						 sizeof(on));
}

// ----------------------------------------------------------
void ofxAudioUnitMixer::enableOutputMetering()
// ----------------------------------------------------------
{
	UInt32 on = 1;
	AudioUnitSetProperty(*_unit,
						 kAudioUnitProperty_MeteringMode,
						 kAudioUnitScope_Output,
						 0,
						 &on,
						 sizeof(on));
}

// ----------------------------------------------------------
void ofxAudioUnitMixer::disableInputMetering(int bus)
// ----------------------------------------------------------
{
	UInt32 off = 0;
	AudioUnitSetProperty(*_unit,
						 kAudioUnitProperty_MeteringMode,
						 kAudioUnitScope_Input,
						 bus,
						 &off,
						 sizeof(off));
}

// ----------------------------------------------------------
void ofxAudioUnitMixer::disableOutputMetering()
// ----------------------------------------------------------
{
	UInt32 off = 0;
	AudioUnitSetProperty(*_unit,
						 kAudioUnitProperty_MeteringMode,
						 kAudioUnitScope_Output,
						 0,
						 &off,
						 sizeof(off));
}
