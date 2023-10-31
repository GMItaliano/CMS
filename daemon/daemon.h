#ifndef DAEMON_H
#define DAEMON_H

//Daemon
#include <fcntl.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/syslog.h>

//Communication
#include <mqueue.h>
#include <signal.h>

class daemon{
    private:

    mqd_t msgqueue;
    struct mq_attr attr;
    //door_sys door;
    //motion_sys motion;
    //button_sys button;

    //Flags
    

    //daemon sensor interrupts
    void door_isr();
    void motion_isr();
    vood button_isr();

    public:

    daemon();
    ~daemon();

    void run();
    void stop();
};

#endif DAEMON_H