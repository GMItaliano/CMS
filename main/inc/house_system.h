#ifndef HOUSE_SYSTEM_H
#define HOUSE_SYSTEM_H

//This class is the main class of the project responsible for all the tasks and syncronization of the same 

#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <mqueue.h>
#include <signal.h>

//subsystems Classes
//#include "speaker_sys.h"
#include "relay_sys.h"
#include "livestream_ctrl.h"
#include "database_sys.h"


//message queue organization
#define MQ_NAME "/MSGQUEUE_SENSORS"
#define MQ_MAXSIZE 50
#define MQ_MAXMSGS 5

//struct for the sensor flags
struct sinp_flags{
    bool motion, door, button;
};

struct control_flags{
    bool relay, sound, live;
};

class houseSystem{
    private:

    //speaker_sys speaker;
    relay_sys relay;
    livestream_ctrl livestream;
    database_sys database;
    
    //signals
    //struct sigevent sig_ev;

    //Mutexes
    pthread_mutex_t mutdata;    //mutex for tupdateflags
    pthread_mutex_t mutrelay;   //mutex for trelay
    pthread_mutex_t mutsensors; //mutex for tsensors
    pthread_mutex_t mutsound;   //mutex for tsound
    pthread_mutex_t mutstream;  //mutex for tlivestream

    //Condition variables
    pthread_cond_t cvsensors;   //condition var for tsensors
    pthread_cond_t cvrelay;     //condition var for trelay
    pthread_cond_t cvsound;     //condition var for tsound
    pthread_cond_t cvstream;    //condition var for tstream

    //FLAGS
    sinp_flags house_sen;
    sinp_flags database_sen;
    control_flags house_periph;
    control_flags database_periph;

    bool sensors = 0;
    bool control_flag = 0;


    //message queue organization
    struct mq_attr attr_msg;
    mqd_t msgqueue;
    char data[MQ_MAXSIZE];
    unsigned int prio;

    //create 1 thread and add conformingly or create already 4 threads one fors each task
    pthread_t thread1, thread2, thread3, thread4, thread5;
    
    //thread functions
    static void *tupdateFlags(void* );
    static void *tstream(void* );
    static void *tsensors(void* );
    static void *trelay(void* );
    static void *tsound(void* );

    static void sigHandler(int sig);

    public:

    houseSystem();              // create all the tasks and necessities that will be needed
    ~houseSystem();             //destroy all the tasks and necessities 

    void run();

};

#endif 
