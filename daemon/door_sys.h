
#ifndef __DOOR_SYS_H__
#define __DOOR_SYS_H__

#include <string>
#include <signal.h>
#include <iostream>

class door_sys
{
private:
    struct sigaction sig;
    std::string dev_str;
    int device;
    bool previous_val;
public:
    door_sys(/* args */);
    ~door_sys();

    void enable();
    void disable();

    bool get_isr_value();
};

#endif