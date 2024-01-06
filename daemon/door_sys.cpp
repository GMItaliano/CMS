#include "door_sys.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <string>

#define name "magnetic"
#define IOCTL_PID 1
#define SIGUSR1_HANDLER SIGUSR1


door_sys::door_sys(ISR isr){
    dev_str = "/dev/" + (std::string)name;
    sigemptyset(&sig.sa_mask);
    handler = isr;

}

door_sys::~door_sys(){
    close(device);
}   

void door_sys::enable(){

    //dev_str = "/dev/" + (std::string)name;
    pid_t pid;
    previous_val = false;
    //open and enable device driver

    device = open(dev_str.c_str(), O_RDWR);     //open the specific device driver

    if(device < 0)
        logError("[ERROR] Device Driver NOT found");
    
    pid = getpid();

    if(ioctl(device, IOCTL_PID, &pid))
	{
		close(device);
		logError("[ERROR] Failed system call. Closing device driver");
	}

    sig.sa_sigaction = handler;
    sig.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1_HANDLER, &sig, NULL) == -1) {
        logError("[ERROR] Failed to set up signal handler");
    }

}

void door_sys::disable(){

    sig.sa_handler = SIG_IGN;

	sigaction(SIGUSR2, &sig, NULL);

}

bool door_sys::get_isr_value(){

    char buffer[2];
    ssize_t bytesRead = read(device, buffer, sizeof(buffer) - 1);

    if (bytesRead < 0) {
        // Handle read error
        logError("[ERROR] Error reading from the device driver");
        return false;
    }

    buffer[bytesRead] = '\0';
    int value = std::stoi(buffer);

    // Process the read value as needed
    bool doorOpened = (value != 0);

    

    // No state change, return false
    return doorOpened;

}
