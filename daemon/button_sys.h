#ifndef __BUTTON_SYS_H__
#define __BUTTON_SYS_H__

#include <sys/ioctl.h>
#include <signal.h> 
#include <string>

class button_sys
{
private:
    struct sigaction sig;
    std::string dev_str;
    int device;
    bool previous_val;
public:
    button_sys();
    ~button_sys();

    void enable();
    void disable();

    bool get_isr_value();

};

#endif
