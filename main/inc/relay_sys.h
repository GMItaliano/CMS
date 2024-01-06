#ifndef __RELAY_SYS_H__
#define __RELAY_SYS_H__

#include <iostream>
#include <string>

class relay_sys
{
private:
    bool control;
    std::string dev_str;
    int device;
public:
    relay_sys();
    ~relay_sys();

    void activate_relay();
    void deactivate_relay();
};

#endif