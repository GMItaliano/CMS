
#ifndef __DOOR_SYS_H__
#define __DOOR_SYS_H__

#include <string>
#include <signal.h>
#include <iostream>

#include "interrupt.h"
#include "data_errors.h"

//typedef void (*ISR)(int, siginfo_t*, void*);

class door_sys
{
private:
    struct sigaction sig;
    std::string dev_str;
    int device;
    bool previous_val;
    ISR handler;
public:
    door_sys(ISR);
    ~door_sys();

    void enable();
    void disable();

    bool get_isr_value();
};

#endif