#ifndef MICROPHONE_SYS_H
#define MICROPHONE_SYS_H

#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

class microphone_sys{
    
    private:
        
        ALCdevice* cap_device;
        ALCint samplerate;
        ALCsizei buffersize;

        bool micro_flag;

    public:

        microphone_sys();
        ~microphone_sys();

        void record_audio(bool);

};

#endif MICROPHONE_SYS_H