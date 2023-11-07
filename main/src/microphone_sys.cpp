#include "microphone_sys.h"

microphone_sys::microphone_sys(){

    samplerate = 44100;
    buffersize = 1024;

    cap_device = alcCaptureOpenDevice(nullptr, samplerate, AL_FORMAT_MONO16, buffersize);

    if(!cap_device)
        std::cerr << "ERROR: FAILED TO OPEN MICROPHONE DEVICE" << std::endl;

}   

microphone_sys::~microphone_sys(){
    alcCaptureCloseDevice(cap_device);
}

void microphone_sys::record_audio(bool ctrl){
    
    ALshort* audio = new ALshort[buffersize];

    if(micro_flag && cap_device){
        alcCaptureStart(cap_device);
        alcCaptureSamples(cap_device, audio, buffersize);
    }else 
        alcCaptureStop(cap_device);
}