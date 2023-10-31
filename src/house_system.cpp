#include "house_system.h"

// O QUE FALTA TESTAR sobre PTHREADS?
/*
    uso de condition codes para transportar informação entre threads como Flags ou outras variaveis
    implementação de mutexes para controlar estas conditions variables

    1 teste: prioridades                            [X]
    2 teste: alterar a memoria entre threads        [X]
    3 teste: trocar de valores entre threads        [X]

*/





//test linux interrupt
#include <signal.h>
#include <time.h>
//--------------------

#define NUMTHREADS  4
#define CONDSIZE    10

//------- GLOBAL VARIABLES --------

int t1 = 100, t2 = 1000, t3 = 100, t4 = 100;
int shared_value = 0;





/*int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine)(void*), void *arg);*/
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
    std::cout << " 1 update Flags in" << std::endl;
    houseSystem* instance = static_cast<houseSystem*>(arg);

    //------TEST-------
    while(t1>0){
        t1--;
        std::cout << "TASK 1: Counter -> " << t1 << std::endl;
    }

    pthread_mutex_lock(&instance -> mutdata);
    while (shared_value == 0){
        pthread_cond_wait(&instance -> cvsensors, &instance -> mutdata);
    }
    pthread_mutex_unlock(&instance -> mutdata);

    while(shared_value > 0){
        shared_value--;
        std::cout << "TASK 1: SHAREDVALUE -> " << shared_value << std::endl;
    }

    std::cout << "SHARED VALUE FINISHED" << std::endl;

    //mutex control for shared data -> test 
    pthread_mutex_lock(&instance -> mutsensors);
    instance->control_flag = 1;
    pthread_mutex_unlock(&instance -> mutsensors);

    //------TEST-------

    pthread_exit(NULL);
}

void* houseSystem::tstream(void* arg){
    std::cout << " 2 Stream in" << std::endl;
    houseSystem* instance = static_cast<houseSystem*>(arg);
    
    //------TEST-------
    while(t2>0){
        t2--;
        std::cout << "TASK 2: Counter -> " << t2 << std::endl;

        //mutex control for shared data -> test
        pthread_mutex_lock(&instance -> mutsensors);
        if (t2 == 100){
            shared_value = 255;
            std::cout << "shared value activated: " << shared_value << std::endl;
            pthread_cond_signal(&instance -> cvsensors);
        }
        pthread_mutex_unlock(&instance -> mutsensors);
    }
    //------TEST-------

    pthread_exit(NULL);
}

void* houseSystem::tsensors(void* arg){
    std::cout << " 3 Sensors in" << std::endl;
    
    //------TEST-------
    while(t3>0){
        t3--;
        std::cout << "TASK 3: Counter -> " << t3 << std::endl;
    }
    //------TEST-------
    
    pthread_exit(NULL);
}

void* houseSystem::trelay(void* arg){
    std::cout << " 4 Relay in" << std::endl;
    
    //------TEST-------
    while(t4>0){
        t4--;
        std::cout << "TASK 4: Counter -> " << t4 << std::endl;
    }
    //------TEST-------
    
    pthread_exit(NULL);
}

