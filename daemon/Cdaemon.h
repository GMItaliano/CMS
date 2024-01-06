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
#include "motion_sys.h"
#include "door_sys.h"
#include "button_sys.h"

class Cdaemon{
    private:

    static Cdaemon *thisPtr;

    mqd_t msgqueue;
    struct mq_attr attr;
    door_sys door;
    motion_sys motion;
    button_sys button;

    //Flags
    bool button_flag = 0;
    bool door_flag = 0;
    bool motion_flag = 0;

    //daemon sensor interrupts
    void door_isr();
    void motion_isr();
    void button_isr();

    static void isr_control(int, siginfo_t *info, void *unused);        //function that will handle the signals sent from the device drivers

    public:

    Cdaemon();
    ~Cdaemon();

    void run();
    void stop();

    void set_flags(int, bool);
};

#endif 