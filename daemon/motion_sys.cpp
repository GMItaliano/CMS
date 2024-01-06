#include "motion_sys.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <string>

#define name "motion"
#define IOCTL_PID 1
#define SIGUSR1_HANDLER SIGUSR1

// EXTRA FUNCTIONS 
// typedef void (*ISR)(int, siginfo_t*, void*);
// ISR handler;

motion_sys::motion_sys(ISR isr){

    sigemptyset(&sig.sa_mask);
    handler = isr;

}

motion_sys::~motion_sys(){
    close(device);
}   

void motion_sys::enable(){

    dev_str = "/dev/" + (std::string)name;
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
		logError("[ERROR] Failed system call. Closing device driver");
	}

    sig.sa_sigaction = handler;
    sig.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1_HANDLER, &sig, NULL) == -1) {
         logError("[ERROR] Failed to set up signal handler");
    }

}

void motion_sys::disable(){

    close(device);

    sig.sa_handler = SIG_IGN;

	sigaction(SIGUSR1_HANDLER, &sig, NULL);

}

bool motion_sys::get_isr_value(){

    char buffer[2];
    ssize_t bytesRead = read(device, buffer, sizeof(buffer) - 1);

    if (bytesRead < 0) {
        // Handle read error
        logError("[ERROR] Error reading from the device driver");
        return false;
    }

    buffer[bytesRead] = '\0';
    int value = std::stoi(buffer);

    bool motion_detected = (value !=0);

    return motion_detected;

}

pid_t motion_sys::get_pid(){
    return ProcessID;
}
