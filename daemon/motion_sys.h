#ifndef MOTION_SYS_H
#define MOTION_SYS_H

#include <signal.h>
#include <string>
//include device driver

class motion_sys{

    private:
        struct sigaction sig;
        std::string dev_str;
        int device;
        bool previous_val;
        
    public:
        motion_sys();
        ~motion_sys();

        void enable();
        void disable();

        bool get_isr_value();

};

#endif MOTION_SYS_H