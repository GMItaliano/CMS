/* ------- TESTS TO DO ------------
    -> msg queues how they send information
    -> communication between daemon and main process
    -> activation of the threads with the new configurations
    -> finish the parser function after testing the mqueues
    -> signals activations both in the main process and daemon process
    -> test if there is any type of conflict when use SIGUSR1 in both processes 
*/
/*  ------ DONE --------
    -> Threads definition and communication between them
    -> all classes created 
    -> daemon process initialized and almost done
*/
/* ------- TO DO --------
    -> finish all class
    -> make device drivers
    -> database implementation (last)
    -> Establish interrupts for the GPIO pins
    -> See the necessity for a char device or block device (at the moment don't see any)
    -> organize the code and classes to \inc file(header) and \src file(.cpp)

    -> make files see how to do them
    -> how to run daemon
    -> how to make device drivers
*/

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

int t1 = 100, t2 = 1000, t3 = 100, t4 = 100;
int shared_value = 0;

bool old_relay = 0;

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

void parser_mqueue(std::string msg, sinp_flags flags){
    /*
    -> This function is responsible for the parsing of the message queue
    */


   for(int i = 0; msg[i] != '\0' ; i++){

   }

}

//timer test for interrupt 
timer_t timer_id;

void timerCallback(union sigval sv) {
    std::cout << "Timer interrupt occurred." << std::endl;
    // Perform your desired actions here
}


//-----------------------------------------------

//-----CONSTRUCTOR & DESTRUCTOR-------

houseSystem::houseSystem(){
    int status[NUMTHREADS], i;

    //SIGNAL creation 
    sig_ev.sigev_notify = SIGEV_SIGNAL;
    sig_ev.sigev_signo = SIGUSR1;

    //open message queue for reading
    msgqueue = mq_open(MQ_NAME, O_CREAT | O_RDONLY, 0666, nullptr);
    if (msgqueue == (mqd_t)-1) {
        std::perror("mq_open");
        exit(1);
    }

    if(mq_notify(msgqueue,&sig_ev)){        //create the signal for notification
        std::perror("mq_notify");
        exit(1);
    }

    //mutexes creation
    pthread_mutex_init(&mutdata, nullptr);
    pthread_mutex_init(&mutsensors, nullptr);
    //mutdata = PTHREAD_MUTEX_INITIALIZER; 
    //mutsensors = PTHREAD_MUTEX_INITIALIZER;
    
    //condition varables creation
    pthread_cond_init(&cvsensors, nullptr);
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
    for (i=0; i<NUMTHREADS; i++){
        checkFail(status[i], i+1);
    }
        
    std::cout << "\nThreads created: " << i << std::endl;
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

    // //------TEST-------
    // while(t1>0){
    //     t1--;
    //     std::cout << "TASK 1: Counter -> " << t1 << std::endl;
    // }

    // pthread_mutex_lock(&instance -> mutdata);
    // while (shared_value == 0){
    //     pthread_cond_wait(&instance -> cvsensors, &instance -> mutdata);
    // }
    // pthread_mutex_unlock(&instance -> mutdata);

    // while(shared_value > 0){
    //     shared_value--;
    //     std::cout << "TASK 1: SHAREDVALUE -> " << shared_value << std::endl;
    // }

    // std::cout << "SHARED VALUE FINISHED" << std::endl;

    // //mutex control for shared data -> test 
    // pthread_mutex_lock(&instance -> mutsensors);
    // instance->control_flag = 1;
    // pthread_mutex_unlock(&instance -> mutsensors);
    // //------TEST-------

    pthread_mutex_lock(&instance -> mutdata);

    if(!instance -> sensors)
        pthread_cond_wait(&instance -> cvsensors, &instance -> mutdata);

    if(!instance -> relay)
        pthread_cond_wait(&instance -> cvrelay, &instance -> mutdata);

    if(instance -> sensors){                //update flags to the database

    }else if(instance -> relay){            //updates the flag and call the trelay
        pthread_cond_signal(&instance -> cvrelay);
        instance -> relay = 1;      //WRONG!!! IT NEEDS TO BE UPDATED CONFORMING THE DATABASE!!!!!!!
    }else{                                  //prints something for debug

    }

    pthread_mutex_unlock(&instance -> mutdata);

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
    
    //------TEST-------
    // while(t2>0){
    //     t2--;
    //     std::cout << "TASK 2: Counter -> " << t2 << std::endl;

    //     //mutex control for shared data -> test
    //     pthread_mutex_lock(&instance -> mutsensors);
    //     if (t2 == 100){
    //         shared_value = 255;
    //         std::cout << "shared value activated: " << shared_value << std::endl;
    //         pthread_cond_signal(&instance -> cvsensors);
    //     }
    //     pthread_mutex_unlock(&instance -> mutsensors);
    // }
    //------TEST-------

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
    
    pthread_mutex_lock(&instance ->mutsensors);

    while(!SIGUSR1);

    if(SIGUSR1){

        ssize_t bytes_received = mq_receive(instance -> msgqueue,instance -> data, sizeof(data), &instance->prio);
        instance -> data[bytes_received] = '\0';

        std::cout << instance -> data << std::endl;

        parser_mqueue(instance -> data , instance -> house_sen);

        pthread_cond_signal(&instance -> cvsensors);

        instance -> sensors = 1;

    }

    pthread_mutex_unlock(&instance -> mutsensors);
    
    //pthread_exit(NULL);
}

void* houseSystem::trelay(void* arg){
    /*
    -> This function is responsible for the enabling and disabling of the relay this function will be interacted
    by the value of its flag in the database
    */
     houseSystem* instance = static_cast<houseSystem*>(arg);
    std::cout << " 4 Relay in" << std::endl;
    
    pthread_mutex_lock(&instance ->mutrelay);

    while(old_relay == instance->relay)
        pthread_cond_wait(&instance -> cvrelay, &instance -> mutrelay);

    if(old_relay != instance -> relay){
        //ACTIVATE OR DEACTIVATE RELAY CONFORMING THE VALUE IN RELAY
        //RELAY DEVICE DRIVER
        old_relay = instance-> relay;
    }

    pthread_mutex_unlock(&instance ->mutrelay);
    
    //pthread_exit(NULL);
}

