#include "ofxAudioUnitMatrixMixer.h"
#include "ofxAudioUnitUtils.h"

AudioComponentDescription matrixMixerDesc = {
	kAudioUnitType_Mixer,
	kAudioUnitSubType_MatrixMixer,
	kAudioUnitManufacturer_Apple
};

ofxAudioUnitMatrixMixer::ofxAudioUnitMatrixMixer(UInt32 inputBusCount, UInt32 outputBusCount, AudioStreamBasicDescription * inASBD, AudioStreamBasicDescription * outASBD)
{
	initWithLayout(inputBusCount, outputBusCount, inASBD, outASBD);
}

ofxAudioUnitMatrixMixer::ofxAudioUnitMatrixMixer(const ofxAudioUnitMatrixMixer &orig)
{	
	vector<UInt32> origBusCount = orig.getBusCount();
	vector<AudioStreamBasicDescription> origASBDs = orig.getASBDs();
	initWithLayout(origBusCount[0], origBusCount[1], &origASBDs[0], &origASBDs[1]);
}

ofxAudioUnitMatrixMixer& ofxAudioUnitMatrixMixer::operator=(const ofxAudioUnitMatrixMixer &orig)
{
	vector<UInt32> origBusCount = orig.getBusCount();
	vector<AudioStreamBasicDescription> origASBDs = orig.getASBDs();
	initWithLayout(origBusCount[0], origBusCount[1], &origASBDs[0], &origASBDs[1]);
}

void ofxAudioUnitMatrixMixer::initWithLayout(UInt32 inputBusCount, UInt32 outputBusCount, AudioStreamBasicDescription * inASBD, AudioStreamBasicDescription * outASBD)
{
	_desc = matrixMixerDesc;
	_unit = allocUnit(_desc);
	
	OFXAU_PRINT(AudioUnitSetProperty(*_unit,
									 kAudioUnitProperty_ElementCount,
									 kAudioUnitScope_Input,
									 0,
									 &inputBusCount,
									 sizeof(inputBusCount)),
				"setting number of input busses");
	
	OFXAU_PRINT(AudioUnitSetProperty(*_unit,
									 kAudioUnitProperty_ElementCount,
									 kAudioUnitScope_Output,
									 0,
									 &outputBusCount,
									 sizeof(outputBusCount)),
				"setting number of output busses");
	
	if(inASBD) {
         //cout<<"inASBD";
		for(int i = 0; i < inputBusCount; i++) {
			OFXAU_PRINT(AudioUnitSetProperty(*_unit,
											 kAudioUnitProperty_StreamFormat,
											 kAudioUnitScope_Input,
											 i,
											 inASBD,
											 sizeof(AudioStreamBasicDescription)),
						"setting matrix mixer input ASBD");
		}
	}
	
	if(outASBD) {
        //cout<<"outASBD";
		for(int i = 0; i < outputBusCount; i++) {
			OFXAU_PRINT(AudioUnitSetProperty(*_unit,
											 kAudioUnitProperty_StreamFormat,
											 kAudioUnitScope_Output,
											 i,
											 outASBD,
											 sizeof(AudioStreamBasicDescription)),
						"setting matrix mixer output ASBD");
		}
	}
	
	OFXAU_PRINT(AudioUnitInitialize(*_unit), "initializing matrix mixer unit");
}

ofxAudioUnitMatrixMixer::~ofxAudioUnitMatrixMixer()
{
	
}

void ofxAudioUnitMatrixMixer::setChannelLayout(UInt32 inputBusCount, UInt32 outputBusCount, AudioStreamBasicDescription * inASBD, AudioStreamBasicDescription * outASBD)
{
	initWithLayout(inputBusCount, outputBusCount, inASBD, outASBD);
}

#pragma mark - Levels

void ofxAudioUnitMatrixMixer::setLevels(const float * levels, size_t levelsCount)
{	
	OFXAU_PRINT(AudioUnitSetProperty(*_unit,
									 kAudioUnitProperty_MatrixLevels,
									 kAudioUnitScope_Global,
									 0,
									 levels,
									 levelsCount * sizeof(Float32)),
				"setting matrix volume levels");
}

void ofxAudioUnitMatrixMixer::setLevels(const std::vector<float> &levels)
{
	setLevels(&levels[0], levels.size());
}

#pragma mark - Util

std::vector<UInt32> ofxAudioUnitMatrixMixer::getBusCount() const
{
	std::vector<UInt32> busCount;
	UInt32 inputBusCount, outputBusCount, inputSize, outputSize;
	inputBusCount = outputBusCount = 0;
	inputSize = outputSize = sizeof(UInt32);
	
	OFXAU_PRINT(AudioUnitGetProperty(*_unit,
									 kAudioUnitProperty_ElementCount,
									 kAudioUnitScope_Input,
									 0,
									 &inputBusCount,
									 &inputSize),
				"getting matrix mixer input bus count");
	
	OFXAU_PRINT(AudioUnitGetProperty(*_unit,
									 kAudioUnitProperty_ElementCount,
									 kAudioUnitScope_Output,
									 0,
									 &outputBusCount,
									 &outputSize),
				"getting matrix mixer output bus count");
	
	busCount.push_back(inputBusCount);
	busCount.push_back(outputBusCount);
	return busCount;
}

std::vector<AudioStreamBasicDescription> ofxAudioUnitMatrixMixer::getASBDs() const
{
	std::vector<AudioStreamBasicDescription> ASBDs;
	AudioStreamBasicDescription inputASBD, outputASBD;
	UInt32 inputSize, outputSize;
	inputSize = outputSize = sizeof(UInt32);
	
	OFXAU_PRINT(AudioUnitGetProperty(*_unit,
									 kAudioUnitProperty_StreamFormat,
									 kAudioUnitScope_Input,
									 0,
									 &inputASBD,
									 &inputSize),
				"getting matrix mixer input ASBD");
	
	OFXAU_PRINT(AudioUnitGetProperty(*_unit,
									 kAudioUnitProperty_StreamFormat,
									 kAudioUnitScope_Output,
									 0,
									 &outputASBD,
									 &outputSize),
				"getting matrix mixer output ASBD");
	
	ASBDs.push_back(inputASBD);
	ASBDs.push_back(outputASBD);
	return ASBDs;
}

// ----------------------------------------------------------
float ofxAudioUnitMatrixMixer::getOutputLevel(int channel) const
// ----------------------------------------------------------
{
    /*
    for (int i=0; i<numOutChannels; ++i) {
		float amps[2];
		err = AudioUnitGetParameter(mixer, kMatrixMixerParam_PostAveragePower, kAudioUnitScope_Output, i, &amps[0]);
		err = AudioUnitGetParameter(mixer, kMatrixMixerParam_PostPeakHoldLevel, kAudioUnitScope_Output, i, &amps[1]);
		value = sqrt(dbamp(value));
		[meter[i] updateMeters: amps];
	}
    */
	AudioUnitParameterValue level;
	OFXAU_PRINT(AudioUnitGetParameter(*_unit,
									  kMatrixMixerParam_PostAveragePower,
									  kAudioUnitScope_Output,
									  channel,
									  &level),
				"getting mixer output level");
    
  //  double dbamp(double db) { return pow(10., 0.05 * db); }
    
 //   level = 255 + level;
    
   // float dbamp = pow(1.5, 0.07 * level);
	return level;
}

// ----------------------------------------------------------
void ofxAudioUnitMatrixMixer::enableOutputMetering()
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
void ofxAudioUnitMatrixMixer::disableOutputMetering()
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
 
