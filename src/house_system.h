#ifndef HOUSE_SYSTEM_H
#define HOUSE_SYSTEM_H

//This class is the main class of the project responsible for all the tasks and syncronization of the same 

#include <iostream>
#include <pthread.h>
//thread syncronization and priority
#include <sched.h>
#include <errno.h>
#include <unistd.h>

//subsystems Classes
//#include "camera_sys.h"
//#include "microphone_sys.h"
//#include "speaker_sys.h"
//#include "relay_sys.h"
//#include "livestream_ctrl.h"
//#include "database_sys.h"

class houseSystem{
    private:

    //camera_sys camera;
    //microphone_sys microphone;
    //speaker_sys speaker;
    //relay_sys relay;
    //livestream_ctrl livestream;
    //database_sys database;
    pthread_cond_t cvsensors;
    pthread_mutex_t mutdata;
    pthread_mutex_t mutsensors; //= PTHREAD_MUTEX_INITIALIZER 

    //create 1 thread and add conformingly or create already 4 threads one fors each task

    bool control_flag = 0;

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
