#ifndef MOTION_SYS_H
#define MOTION_SYS_H

#include <signal.h>
#include <string>
//include device driver
#include "interrupt.h"
#include "data_errors.h"

//typedef void (*ISR)(int, siginfo_t*, void*);

class motion_sys{

    private:
        struct sigaction sig;
        std::string dev_str;
        int device;
        bool previous_val;
        ISR handler;
        pid_t ProcessID;
        
    public:
        motion_sys(ISR);
        ~motion_sys();

        void enable();
        void disable();

        bool get_isr_value();
        pid_t get_pid();

};

#endif 