#pragma once

#include "ofxAudioUnitBase.h"

class ofxAudioUnitMatrixMixer : public ofxAudioUnit {

public:
	ofxAudioUnitMatrixMixer(UInt32 inputBusCount = 8, UInt32 outputBusCount = 8, AudioStreamBasicDescription * inASBD = NULL, AudioStreamBasicDescription * outASBD = NULL);
	~ofxAudioUnitMatrixMixer();
	ofxAudioUnitMatrixMixer(const ofxAudioUnitMatrixMixer &orig);
	ofxAudioUnitMatrixMixer& operator=(const ofxAudioUnitMatrixMixer &orig);
	
	// this will remove any active connections, effectively resetting the unit
	void setChannelLayout(UInt32 inputBusCount, UInt32 outputBusCount, AudioStreamBasicDescription * inASBD = NULL, AudioStreamBasicDescription * outASBD = NULL);
	
	void setLevels(const std::vector<float> &levels);
	void setLevels(const float * levels, size_t levelsCount);
	
    float getOutputLevel(int channel) const;
    void  enableOutputMetering();
	void  disableOutputMetering();
    
private:
	void initWithLayout(UInt32 inputBusCount, UInt32 outputBusCount, AudioStreamBasicDescription * inASBD = NULL, AudioStreamBasicDescription * outASBD = NULL);
	std::vector<UInt32> getBusCount() const;
	std::vector<AudioStreamBasicDescription> getASBDs() const;
};
