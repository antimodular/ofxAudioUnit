#include "ofxAudioUnit.h"
#include "ofxAudioUnitUtils.h"
#import <mach/mach_time.h>

AudioComponentDescription filePlayerDesc =
{
	kAudioUnitType_Generator,
	kAudioUnitSubType_AudioFilePlayer,
	kAudioUnitManufacturer_Apple
};

// ----------------------------------------------------------
ofxAudioUnitFilePlayer::ofxAudioUnitFilePlayer()
// ----------------------------------------------------------
{
	_desc = filePlayerDesc;
	initUnit();
}

// ----------------------------------------------------------
ofxAudioUnitFilePlayer::~ofxAudioUnitFilePlayer()
// ----------------------------------------------------------
{
	stop();
	AudioFileClose(_fileID[0]);
}

#pragma mark - Properties

// ----------------------------------------------------------
bool ofxAudioUnitFilePlayer::setFile(const std::string &filePath)
// ----------------------------------------------------------
{
	CFURLRef fileURL;
	fileURL = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault,
	                                                  (const UInt8 *)filePath.c_str(),
	                                                  filePath.length(),
	                                                  NULL);
	
	if(_fileID[0]) AudioFileClose(_fileID[0]);
	
	OSStatus s = AudioFileOpenURL(fileURL, kAudioFileReadPermission, 0, _fileID);
	
	CFRelease(fileURL);
	
	if(s != noErr)
	{
		cout << "Error " << s << " while opening file at " << filePath << endl;
		return false;
	}
	
	UInt64 numPackets = 0;
	UInt32 dataSize   = sizeof(numPackets);
	
	AudioFileGetProperty(_fileID[0],
	                     kAudioFilePropertyAudioDataPacketCount,
	                     &dataSize,
	                     &numPackets);
	
	AudioStreamBasicDescription asbd = {0};
	dataSize = sizeof(asbd);
	
	AudioFileGetProperty(_fileID[0], kAudioFilePropertyDataFormat, &dataSize, &asbd);
	
	// defining a region which basically says "play the whole file"
	memset(&_region, 0, sizeof(_region));
	_region.mTimeStamp.mFlags       = kAudioTimeStampHostTimeValid;
	_region.mTimeStamp.mSampleTime  = 0;
	_region.mCompletionProc         = NULL;
	_region.mCompletionProcUserData = NULL;
	_region.mAudioFile              = _fileID[0];
	_region.mLoopCount              = 0;
	_region.mStartFrame             = 0;
	_region.mFramesToPlay           = numPackets * asbd.mFramesPerPacket;
	
	// setting the file ID now since it seems to have some overhead.
	// Doing it now ensures you'll get sound pretty much instantly after
	// calling play() (subsequent calls don't have the overhead)
	OFXAU_RET_BOOL(AudioUnitSetProperty(*_unit,
	                                    kAudioUnitProperty_ScheduledFileIDs,
	                                    kAudioUnitScope_Global,
	                                    0,
	                                    _fileID,
	                                    sizeof(_fileID)),
	               "setting file player's file ID");
}

// ----------------------------------------------------------
void ofxAudioUnitFilePlayer::setLength(UInt32 length)
// ----------------------------------------------------------
{
	_region.mFramesToPlay = length;
}

// ----------------------------------------------------------
UInt32 ofxAudioUnitFilePlayer::getLength()
// ----------------------------------------------------------
{
	return _region.mFramesToPlay;
}

#pragma mark - Playback

// ----------------------------------------------------------
void ofxAudioUnitFilePlayer::play(uint64_t startTime)
// ----------------------------------------------------------
{
    
    /*1. Open the file using AudioFileOpen(), which gives you an AudioFileID reference to it.
     2. Set up an ScheduledAudioFileRegion (this is a C structure) to specify which part of the file to play. Set this as the kAudioUnitProperty_ScheduledFileRegion property on your file player unit.
     3. Prime frames on the unit using the kAudioUnitProperty_ScheduledFilePrime property. This preloads frames from disk so playback is able to start immediately when you tell it to.
     4. Set a start time on the unit by assigning an AudioTimeStamp structure to the kAudioUnitProperty_ScheduleStartTimeStamp property. This will start the unit producing audio (converted from MP3 into the 32-bit float linear PCM format that CoreAudio prefers to deal with internally).
     */
    
	if(!(_region.mTimeStamp.mFlags & kAudioTimeStampHostTimeValid))
	{
		cout << "ofxAudioUnitFilePlayer has no file to play" << endl;
		return;
	}
	
	OFXAU_RETURN(AudioUnitSetProperty(*_unit,
	                                  kAudioUnitProperty_ScheduledFileIDs,
	                                  kAudioUnitScope_Global,
	                                  0,
	                                  _fileID,
	                                  sizeof(_fileID)),
	             "setting file player's file ID");
	
	OFXAU_RETURN(AudioUnitSetProperty(*_unit,
	                                  kAudioUnitProperty_ScheduledFileRegion,
	                                  kAudioUnitScope_Global,
	                                  0,
	                                  &_region,
	                                  sizeof(_region)),
	             "setting file player region");
//	UInt32 defaultVal = 0;
//    AudioUnitSetProperty(*_unit, kAudioUnitProperty_ScheduledFilePrime,
//                         kAudioUnitScope_Global, 0, &defaultVal, sizeof(defaultVal));
    
	if(startTime == 0)
	{
		startTime = mach_absolute_time();
	}
	AudioTimeStamp startTimeStamp = {0};
	FillOutAudioTimeStampWithHostTime(startTimeStamp, startTime);
	
	OFXAU_RETURN(AudioUnitSetProperty(*_unit,
	                                  kAudioUnitProperty_ScheduleStartTimeStamp,
	                                  kAudioUnitScope_Global,
	                                  0,
	                                  &startTimeStamp,
	                                  sizeof(startTimeStamp)),
	             "setting file player start time");
}

// ----------------------------------------------------------
void ofxAudioUnitFilePlayer::loop(unsigned int timesToLoop, uint64_t startTime)
// ----------------------------------------------------------
{
	_region.mLoopCount = timesToLoop;
	play(startTime);
}

// ----------------------------------------------------------
void ofxAudioUnitFilePlayer::stop()
// ----------------------------------------------------------
{
	reset();
}
// ----------------------------------------------------------
int ofxAudioUnitFilePlayer::getPlayheadPos()
// ----------------------------------------------------------
{
	AudioTimeStamp timestamp;
    UInt32 size = sizeof(timestamp);
    AudioUnitGetProperty(*_unit, kAudioUnitProperty_CurrentPlayTime, kAudioUnitScope_Global, 0, &timestamp, &size);
    
    return timestamp.mSampleTime;
}


