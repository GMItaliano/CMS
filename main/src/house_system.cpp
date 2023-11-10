#include "house_system.h"

#include <string.h>
#include <cerrno>
#include <cstdio>

//test linux interrupt
#include <time.h>
//--------------------

#define NUMTHREADS  4
#define CONDSIZE    10

//------- GLOBAL VARIABLES --------

int count = 0;
int shared_value = 0;
bool control_relay = 0;

//---------------------------------

//---------- OTHER FUNCTIONS NEEDED -------------

void checkFail(int status,int thread){                                                  //test errors from threads
    if(status){
        if(status == EPERM)
            std::cerr<< "pthread_create(" << thread << ") got EPERM" << std::endl;
        else if(status == EINVAL)
            std::cerr << "pthread_create(" << thread << ") got EINVAL" << std::endl;
        else 
            std::cerr << "pthread_create(" << thread << ") got error " << status << std::endl;

        errno = status;
        exit(1);
    }
}

void SetupThread(int prio, pthread_attr_t *attr, struct sched_param *param){        //define thread priority

    //slide 50 pthreads
    //higher number of PRIORITY more IMPORTANT the thread

    int rr_min_prio, rr_max_prio;

    pthread_attr_setschedpolicy(attr,SCHED_RR); 

    rr_min_prio = sched_get_priority_min(SCHED_RR);
    rr_max_prio = sched_get_priority_max(SCHED_RR);

    param -> sched_priority = prio;

    std::cout << "SCHED_RR priority Range is " << rr_min_prio << " to " << rr_max_prio
    << " using " << param->sched_priority << std::endl;

    pthread_attr_setschedparam(attr,param);
    //pthread_attr_setinheritsched(attr, PTHREAD_EXPLICIT_SCHED);
    //pthread_attr_setschedpolicy(attr,SCHED_RR);

}

//-----------------------------------------------

//-----CONSTRUCTOR & DESTRUCTOR-------

houseSystem::houseSystem() { //: livestream(0)
    int status[NUMTHREADS], i;

    //SIGNAL creation 
    //sig_ev.sigev_notify = SIGEV_SIGNAL;
    //sig_ev.sigev_signo = SIGUSR1;

    //open message queue for reading
    msgqueue = mq_open(MQ_NAME, O_CREAT | O_RDONLY, 0666, nullptr);
    if (msgqueue == (mqd_t)-1) {
        std::perror("mq_open");
        exit(1);
    }

    mq_getattr(msgqueue, &attr_msg);

    /*
    if(mq_notify(msgqueue,&sig_ev)){        //create the signal for notification
        std::perror("mq_notify");
        exit(1);
    }*/

    //mutexes creation
    pthread_mutex_init(&mutdata, nullptr);
    pthread_mutex_init(&mutsensors, nullptr);

    //condition varables creation
    pthread_cond_init(&cvsensors, nullptr);
    pthread_cond_init(&cvrelay, nullptr);
    //cvsensors = PHREAD_COND_INITIALIZER;

    //define Priority & Schedulling
    int policy;
    struct sched_param param;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_getschedpolicy(&attr,&policy);
    pthread_attr_getschedparam(&attr,&param);

    std::cout<< "Default policy is " << (policy == SCHED_FIFO ? "FIFO"
		: (policy == SCHED_RR ? "RR"
		: (policy == SCHED_OTHER ? "OTHER"
		: "unknown"))) << " priority is " << param.sched_priority << std::endl;


    //thread creation tupdate flags  -> high Priority
    SetupThread(90,&attr,&param);
    std::cout << "\t tupdateFlags\n Creating thread at RR/" << param.sched_priority << std::endl;
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
    status[0] = pthread_create(&thread1,&attr, tupdateFlags, this);

    //thread creation Stream        -> lowest Priority
    SetupThread(50,&attr,&param);
    std::cout << "\t tstream \n Creating thread at RR/" << param.sched_priority << std::endl;
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
    status[1] = pthread_create(&thread2,&attr, tstream, this);

    //thread creation Sensors       -> high Priority
    SetupThread(90,&attr,&param);
    std::cout << "\t tsensors\n Creating thread at RR/" << param.sched_priority << std::endl;
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
    status[2] = pthread_create(&thread3,&attr, tsensors, this);

    //thread creation Relay         -> mid Priority
    SetupThread(70,&attr,&param);
    std::cout << "\t trelay \n Creating thread at RR/" << param.sched_priority << std::endl;
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
    status[3] = pthread_create(&thread4,&attr, trelay, this);


    //test if threads were created and if there were any errors
    for (i=0; i < NUMTHREADS; i++){
        checkFail(status[i], i+1);
        std::cout << "\nThread created: " << i+1 << std::endl;
    }
        
    //std::cout << "\nThreads created: " << i << std::endl;
}

houseSystem::~houseSystem(){

    //mutexes destruction
    pthread_mutex_destroy(&mutdata);
    pthread_mutex_destroy(&mutsensors);
    //condition variables destruction
    pthread_cond_destroy(&cvsensors);

    mq_close(msgqueue);
    
}


//----MAIN FUNCTION-----

//Run() -> main process function all the main process threads will be activated here 
void houseSystem::run(){

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    while(!control_flag);

    //pthread_exit(NULL);
    std::cout<< "Tasks Finalized" << std::endl;
    if(control_flag)
        std::cout << "FLAG CONTROL WORKS" << std::endl;

}

//----------------------

//Threads functions 

void* houseSystem::tupdateFlags(void* arg){
    /*
    -> This Function is responsible with the establishment of communication with the database 
    it will await a sinal from the database or from the thread tsensors for altering its values 
    and update the database conformingly 
    */

    std::cout << " 1 update Flags in" << std::endl;
    houseSystem* instance = static_cast<houseSystem*>(arg);

    bool relay_val = 0;

    while(instance){
    pthread_mutex_lock(&instance -> mutdata);

    //get relay value from database

    if(relay_val != instance->relay){
        instance->relay = relay_val;
        pthread_cond_signal(&instance -> cvrelay);
        control_relay = 1;
    }

    pthread_cond_wait(&instance -> cvsensors, &instance->mutdata);

    

    std::cout<<"ENTERED UPDATE FLAGS" << std::endl;

    pthread_mutex_unlock(&instance -> mutdata);
    }
    
    //pthread_exit(NULL);
}

void* houseSystem::tstream(void* arg){
    /*
    -> This Function is responsible for the control of the livestream by establishing a connection between the livestream
    control and the AWS and can be altered some functionalities by the updating values of certain flags
    in the database.
    */

    std::cout << " 2 Stream in" << std::endl;
    houseSystem* instance = static_cast<houseSystem*>(arg);

    //instance->livestream.start_livestream();

    pthread_mutex_lock(&instance->mutdata);
    
    //testing communication
    if(instance -> sensors){
        std::cout << "SENSORS READ " << count << std::endl;
        count++;

        if(count == 3){
            instance->control_flag = 1;
        }

        instance->sensors = 0;

    }

    pthread_mutex_unlock(&instance->mutdata);

    //pthread_exit(NULL);
}

void* houseSystem::tsensors(void* arg){
    /*
    -> This function will receive the message queue from the daemon and inform each corresponding thread how
    they should proceed.
    It will define wich sensors were activated and proceed to call the update flags thread to establish 
    communication with the database to update the values.
    */

    houseSystem* instance = static_cast<houseSystem*>(arg);
    std::cout << " 3 Sensors in" << std::endl;
    
    ssize_t bytes_received = 0;
    std::string temp;               //Use as secondary memory for mqueues TEST DONT KNOW

    while(instance){

        pthread_mutex_lock(&instance ->mutsensors);

        bytes_received = mq_receive(instance->msgqueue, instance->data, sizeof(data), &instance->prio);
        instance->data[bytes_received] = '\0'; 

        

        if(instance->attr_msg.mq_curmsgs != 0){

            switch (instance->data[0])
            {
            case 'M':               //case motion triggered
                instance->house_sen.motion = 1;
                std::cout << "MSG: MOTION DETECTED" << std::endl;
                break;
            case 'D':               //case door triggered
                instance->house_sen.door = 1;
                std::cout << "MSG: DOOR MOVEMENT" << std::endl;
                break;  
            case 'B':               //case button pressed
                instance->house_sen.button = 1;
                std::cout << "MSG: BUTTON PRESSED" << std::endl;
                break;
            default:
                instance->house_sen.button = 0;
                instance->house_sen.door = 0;
                instance->house_sen.motion = 0;
                break;
            }

            //UPDATE TO DATABASE
            instance->control_flag = 1;
            instance->sensors = 1; //Signal to update flags to database;
            
            pthread_cond_signal(&instance->cvsensors);      //notify update_flags

        }else
            std::cout << "NO Message Queues" << std::endl;


    pthread_mutex_unlock(&instance -> mutsensors);
    }

    
    
}

void* houseSystem::trelay(void* arg){
    /*
    -> This function is responsible for the enabling and disabling of the relay this function will be interacted
    by the value of its flag in the database
    */
    houseSystem* instance = static_cast<houseSystem*>(arg);
    std::cout << " 4 Relay in" << std::endl;
    

    //CONTROL RELAY
    while(instance){
        pthread_mutex_lock(&instance ->mutrelay);    
        while(!control_relay)
            pthread_cond_wait(&instance->cvrelay, &instance->mutrelay);

        if(instance->relay && control_relay){    //ACTIVATE RELAY
            std::cout << "ACTION: RELAY ON" << std::endl;

        }else if(!instance->relay && control_relay){                  //DEACTIVATE RELAY
            std::cout << "ACTION: RELAY OFF" << std::endl;

        } 
        control_relay = 0;      

        pthread_mutex_unlock(&instance ->mutrelay);
    }
    
    //pthread_exit(NULL);
}

