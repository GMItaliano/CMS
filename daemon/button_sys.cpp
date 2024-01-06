#include "button_sys.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <string>

#define name "button"
#define IOCTL_PID 1
#define SIGUSR2_HANDLER SIGUSR2

std::string error_msg;

button_sys::button_sys(ISR isr){

    dev_str = "/dev/" + (std::string)name;
    sigemptyset(&sig.sa_mask);
    handler = isr;
    
    sig.sa_flags = SA_SIGINFO;
    sig.sa_sigaction = handler;


}

button_sys::~button_sys(){
    close(device);
}   

void button_sys::enable(){

    //dev_str = "/dev/" + (std::string)name;
    pid_t pid;
    uid_t pidu;
    previous_val = false;
    //open and enable device driver

    device = open(dev_str.c_str(), O_RDWR);     //open the specific device driver
    
    if(device < 0)
        logError("[ERROR] Device Driver NOT found");
    
    pid = getpid();
    //pidu = getuid(); 
    ProcessID = pid;

    if(ioctl(device, IOCTL_PID, &pid))
	{
		close(device);
		logError("[ERROR] Failed system call. Closing device driver.");
	}

    if (sigaction(SIGUSR2_HANDLER, &sig, NULL) == -1) {
        logError("[ERROR] Failed to set up signal handler");
    }

}

void button_sys::disable(){

    close(device);
    sig.sa_handler = SIG_IGN;
	sigaction(SIGUSR2, &sig, NULL);

}

bool button_sys::get_isr_value(){

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
    bool button_trigger = (value != 0);

    return button_trigger;

}

pid_t button_sys::get_pid(){
    
    return ProcessID;
}
