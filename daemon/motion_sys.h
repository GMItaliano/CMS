#ifndef MOTION_SYS_H
#define MOTION_SYS_H

#include <signal.h>
//include device driver

class motion_sys{

    private:

    public:
        motion_sys();
        ~motion_sys();

        void enable();
        void disable();

};

#endif MOTION_SYS_H