#ifndef __BUTTON_SYS_H__
#define __BUTTON_SYS_H__

#include <sys/ioctl.h>
#include <signal.h> 
#include <string>

#include "interrupt.h"
#include "data_errors.h"

//typedef void (*ISR)(int, siginfo_t*, void*);

class button_sys
{
private:
    struct sigaction sig;
    std::string dev_str;
    int device;
    bool previous_val;
    ISR handler;
    pid_t ProcessID;

public:
    button_sys(ISR);
    ~button_sys();

    void enable();
    void disable();

    bool get_isr_value();
    pid_t get_pid();

};

#endif
