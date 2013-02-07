/*
 *  ofxAudioDeviceInfo.h

this is butchered code from multiple sources
some things are commented out because they are remainders of other functionalities

 */

#include <AudioToolbox/AudioToolbox.h>



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
    OSStatus fillDeviceList();
   // AudioDeviceInfo * GetAudioDevice();
     void GetAudioDevice();
    OSStatus GetAudioDevices( Ptr * devices, UInt16 * devicesAvailable );
private:
    int * deviceIDNumbers;
};

