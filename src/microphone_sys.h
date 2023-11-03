#ifndef MICROPHONE_SYS_H
#define MICROPHONE_SYS_H

class microphone_sys{
    
    private:
        
        bool micro_flag;

    public:

        microphone_sys();
        ~microphone_sys();

        void record_audio();

};

#endif MICROPHONE_SYS_H