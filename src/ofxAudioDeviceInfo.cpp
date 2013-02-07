
#include "ofxAudioDeviceInfo.h"
#include <AudioToolbox/AudioToolbox.h>

/*
struct DeviceListEntry {
    bool              assigned;
    AudioDeviceInfo   device;
    int               priority;
};

typedef struct DeviceListEntry DeviceListEntry;


DeviceListEntry * Devices = NULL;
int DeviceCount = 0;
*/


OSStatus ofxAudioDeviceInfo::fillDeviceList()
{
    OSStatus        err = noErr;
    AudioDeviceID   *devices = NULL;
    UInt16          devicesAvailable = 0;
    
    //if (Devices != NULL) return -1;
    
    // fetch a pointer to the list of available devices
    if((err = GetAudioDevices((Ptr*)&devices, &devicesAvailable)) != noErr) {
        return err;
    }
    
    //fprintf(stderr, "devicesAvailable = %d\n", (int) devicesAvailable);
    
    /*
    Devices = (DeviceListEntry *) calloc(sizeof(DeviceListEntry), devicesAvailable);
    for (int i=0; i<devicesAvailable; i++) {
        Devices[i].assigned = false;
        Devices[i].device.name[0] = '\0';
        Devices[i].device.inputDevice = -1;
        Devices[i].device.outputDevice = -1;
        Devices[i].priority = -1;
    }
     */
    
    
    for (int i=0 ; i<devicesAvailable; i++) {
        fprintf(stderr, "\n");
        UInt16      deviceID = devices[i];
        UInt32      outSize = 0;
        char        deviceName[1024];
        UInt32      theNumberInputChannels  = 0;
        UInt32      theNumberOutputChannels  = 0;
        AudioBufferList *theBufferList = NULL;
        int         device_priority = -1;
        char device_name_selected[1024];
        strcpy(device_name_selected, "Unknown");
        
        // get device name
        outSize = sizeof(deviceName);
        err = AudioDeviceGetProperty(deviceID, 0, 0, kAudioDevicePropertyDeviceName, &outSize, deviceName);
        if(err != noErr) {
            fprintf(stderr, "failed to get device name\n");
            continue;
        }
        fprintf(stderr, "Checking device %s\n", deviceName);
        
        
        outSize = 0;
        theNumberInputChannels = 0;
        err = AudioDeviceGetPropertyInfo( deviceID, 0, 1, kAudioDevicePropertyStreamConfiguration, &outSize, NULL);
        if((err == noErr) && (outSize != 0)) {
            theBufferList = (AudioBufferList*)malloc(outSize);
            if(theBufferList != NULL) {
                // get the input stream configuration
                err = AudioDeviceGetProperty( deviceID, 0, 1, kAudioDevicePropertyStreamConfiguration, &outSize, theBufferList);
                if(err == noErr) {
                    // count the total number of input channels in the stream
                    for(int theIndex = 0; theIndex < theBufferList->mNumberBuffers; ++theIndex)
                        theNumberInputChannels += theBufferList->mBuffers[theIndex].mNumberChannels;
                }
                free(theBufferList);
            }
        }
        /*
         if (theNumberInputChannels) {
         bool dev_found = false;
         for (int j=0; j<devicesAvailable; j++) {
         if (strcmp(Devices[j].device.name, device_name_selected) == 0 &&
         Devices[j].device.inputDevice == -1) {
         Devices[j].device.inputDevice = deviceID;
         dev_found = true;
         break;
         }
         }
         if (dev_found == false) {
         for (int j=0; j<devicesAvailable; j++) {
         if (strlen(Devices[j].device.name) == 0) {
         strcpy(Devices[j].device.name, device_name_selected);
         Devices[j].device.inputDevice = deviceID;
         Devices[j].priority = device_priority;
         break;
         }
         }
         }
         }
         */
        outSize = 0;
        theNumberOutputChannels = 0;
        err = AudioDeviceGetPropertyInfo( deviceID, 0, 0, kAudioDevicePropertyStreamConfiguration, &outSize, NULL);
        if((err == noErr) && (outSize != 0))
        {
            theBufferList = (AudioBufferList*)malloc(outSize);
            if(theBufferList != NULL)
            {
                // get the input stream configuration
                err = AudioDeviceGetProperty( deviceID, 0, 0, kAudioDevicePropertyStreamConfiguration, &outSize,
                                             theBufferList);
                if(err == noErr)
                {
                    // count the total number of input channels in the stream
                    for(int theIndex = 0; theIndex < theBufferList->mNumberBuffers; ++theIndex)
                        theNumberOutputChannels += theBufferList->mBuffers[theIndex].mNumberChannels;
                }
                free(theBufferList);
            }
        }
        /*
         if (theNumberOutputChannels) {
         bool dev_found = false;
         for (int j=0; j<devicesAvailable; j++) {
         if (strcmp(Devices[j].device.name, device_name_selected) == 0 &&
         Devices[j].device.outputDevice == -1) {
         Devices[j].device.outputDevice = deviceID;
         dev_found = true;
         break;
         }
         }
         if (dev_found == false) {
         for (int j=0; i<devicesAvailable; j++) {
         if (strlen(Devices[j].device.name) == 0) {
         strcpy(Devices[j].device.name, device_name_selected);
         Devices[j].device.outputDevice = deviceID;
         Devices[j].priority = device_priority;
         break;
         }
         }
         }
         }
         */
        fprintf(stderr, "Device[%d] name = %s, input channels = %d, output channels = %d\n", deviceID, deviceName, (int) theNumberInputChannels, (int) theNumberOutputChannels);
    }
    /*
     for (int i=0; i<devicesAvailable; i++) {
     fprintf(stderr, "strlen %s\n", Devices[i].device.name);
     fprintf(stderr, "strlen %i\n", strlen(Devices[i].device.name));
     fprintf(stderr, "DeviceCount %i  %i\n", DeviceCount, devicesAvailable);
     if (strlen(Devices[i].device.name)){
     DeviceCount ++;
     }
     }
     */
    
    
    //fprintf(stderr, "DeviceCount %i  %i\n", DeviceCount, devicesAvailable);
    return err;
    
}

void ofxAudioDeviceInfo::GetAudioDevice()
//AudioDeviceInfo * ofxAudioDeviceInfo::GetAudioDevice()
{
    fillDeviceList();
    
    /*
    AudioDeviceInfo * info = NULL;
    
    if (Devices == NULL) fillDeviceList();
    
    int priority = 0;
    
    for (int i=DeviceCount - 1; i>=0; i--) {
        if (Devices[i].assigned == false && Devices[i].priority > priority) {
            Devices[i].assigned = true;
            info = &Devices[i].device;
            priority = Devices[i].priority;
        }
    }
    return info;
     */
}


OSStatus ofxAudioDeviceInfo::GetAudioDevices( Ptr * devices, UInt16 * devicesAvailable )
{
    OSStatus    err = noErr;
    UInt32      outSize;
    Boolean     outWritable;
    
    // find out how many audio devices there are, if any
    err = AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDevices, &outSize, &outWritable);
    if ( err != noErr )
        return err;
    
    // calculate the number of device available
    *devicesAvailable = outSize / sizeof(AudioDeviceID);
    if ( *devicesAvailable < 1 )
    {
        fprintf(stderr, "No devices\n" );
        return err;
    }
    fprintf(stderr, "GetAudioDevices: %d devices available\n", *devicesAvailable);
    
    // make space for the devices we are about to get
    *devices = (Ptr) malloc(outSize);
    
    memset( *devices, 0, outSize );
    err = AudioHardwareGetProperty(kAudioHardwarePropertyDevices, &outSize, (void *) *devices);
    if (err != noErr ) {
        return err;
    }
    
    return err;
}
