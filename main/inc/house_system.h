#ifndef HOUSE_SYSTEM_H
#define HOUSE_SYSTEM_H

//This class is the main class of the project responsible for all the tasks and syncronization of the same 

#include <iostream>
#include <pthread.h>
//thread syncronization and priority
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <mqueue.h>
#include <signal.h>

//subsystems Classes
//#include "speaker_sys.h"
//#include "relay_sys.h"
#include "livestream_ctrl.h"
//#include "database_sys.h"


//message queue organization
#define MQ_NAME "/MSGQUEUE_SENSORS"
#define MQ_MAXSIZE 50
#define MQ_MAXMSGS 5

struct sinp_flags{
    //struct for the sensor flags
    bool motion, door, button;
};

class houseSystem{
    private:

    //speaker_sys speaker;
    //relay_sys relay;
    livestream_ctrl livestream;
    //database_sys database;
    
    //signals
    struct sigevent sig_ev;

    //Mutexes
    pthread_mutex_t mutdata;    //mutex for tupdateflags
    pthread_mutex_t mutrelay;   //mutex for trelay
    pthread_mutex_t mutsensors; //mutex for tsensors

    //Condition variables
    pthread_cond_t cvsensors;   //condition var for tsensors
    pthread_cond_t cvrelay;     //condirion var for trelay

    //FLAGS
    sinp_flags house_sen;
    bool sensors = 0;
    bool relay = 0;
    bool control_flag = 0;


    //message queue organization
    mqd_t msgqueue;
    char data[MQ_MAXSIZE];
    unsigned int prio;

    //create 1 thread and add conformingly or create already 4 threads one fors each task
    pthread_t thread1, thread2, thread3, thread4;
    
    //thread functions
    static void *tupdateFlags(void* );
    static void *tstream(void* );
    static void *tsensors(void* );
    static void *trelay(void* );

    public:

    houseSystem();              // create all the tasks and necessities that will be needed
    ~houseSystem();             //destroy all the tasks and necessities 

    void run();

};

#endif HOUSE_SYSTEM_H
