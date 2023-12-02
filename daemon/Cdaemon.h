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

//Components
//#include "motion_sys.h"


class Cdaemon{
    private:

    mqd_t msgqueue;
    struct mq_attr attr;
    //door_sys door;
    //motion_sys motion;
    //button_sys button;

    //Flags
    bool door_flag = 0;
    bool motion_flag = 0;

    //daemon sensor interrupts
    void door_isr();
    void motion_isr();
    void button_isr();

    void isr_control(int);

    public:

    Cdaemon();
    ~Cdaemon();

    void run();
    void stop();

    void set_flags(int, bool);
};

#endif 