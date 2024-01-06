#include "relay_sys.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>


#define NAME "relay"
#define IOCTL_PID 1


relay_sys::relay_sys(){
    dev_str = "/dev/" + (std::string)NAME;
    device = open(dev_str.c_str(), O_RDWR);
    pid_t pid;

    if (device == -1)
        {
            std::cerr << "[RELAY] Failed to open the device file: " << std::endl;
        }
    
    pid = getpid();
    std::cout << "[PID] Relay PID: " << (int)pid << std::endl;
}

relay_sys::~relay_sys(){
    close(device);
}

void relay_sys::activate_relay(){
    if(device != -1){
        const char* msg = "1";
        ssize_t len = write(device,msg, strlen(msg));
        if(len = -1)
            std::cerr << "[RELAY] Failed to activate Relay" << std::endl;
        else
            std::cerr << "[RELAY] Relay activated" << std::endl;
    }
}

void relay_sys::deactivate_relay(){

    if(device != -1){
        const char* msg = "0";
        ssize_t len = write(device,msg, strlen(msg));
        if(len = -1)
            std::cerr << "[RELAY] Failed to deactivate Relay" << std::endl;
        else
            std::cerr << "[RELAY] Relay deactivated" << std::endl;
    }

}