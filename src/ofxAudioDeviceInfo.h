/*
 *  ofxAudioDeviceInfo.h
 */

#include <AudioToolbox/AudioToolbox.h>
#include <string>
//#include <iostream>

/*
 struct  AudioDeviceInfo{
 char            name[256];
 AudioDeviceID   inputDevice;
 AudioDeviceID   outputDevice;
 };
 */

class ofxAudioDeviceInfo
{
public:
    // OSStatus fillDeviceList();
    // AudioDeviceInfo * GetAudioDevice();
    void GetAudioDevice(int * t_deviceIDArray, std::string * deviceNameArray);
    OSStatus GetAudioDevices( Ptr * devices, UInt16 * devicesAvailable );
private:
    int * deviceIDNumbers;
};

