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


bool last_motion = 0;
bool last_magn = 0;
bool last_button = 0;

std::string database_data;
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

sinp_flags parser_mqueues(std::string data){
    
    sinp_flags s_mq;
    int count = 0;

    for(int i=0; i < data.size(); i++){
        if(data[i-1]==':'){

            if(count == 0)
                s_mq.button = data[i] -'0';
            else if(count == 1)
                s_mq.motion = data[i] -'0';
            else if(count == 2)
                s_mq.door = data[i] -'0';
            
            count++;
        }
    }

    std::cout << "---> PARSER MQ: \nButton: " << s_mq.button << "\nMotion: "
        << s_mq.motion << "\nDoor: " << s_mq.door << std::endl; 

    return s_mq;
}

sinp_flags parser_database(std::string data){
    int count = 0;
    sinp_flags s_data;

    for(int i = 0; data[i] != '\0'; i++){
        if(data[i-2] == ':' && data[i-1] == ' '){
            switch (count)
            {
            case 0:
                if(data[i] == 0 || data[i] == 'F')
                    s_data.button = 0;
                else    
                    s_data.button = 1;
                break;
            case 1:
                if(data[i] == 0 || data[i] == 'F')
                    s_data.door = 0;
                else    
                    s_data.door = 1;
                break;
            case 2:
                if(data[i] == 0 || data[i] == 'F')
                    s_data.motion = 0;
                else    
                    s_data.motion = 1;
                break;
            case 3:
                if(data[i] == 0 || data[i] == 'F')
                    s_data.relay = 0;
                else    
                    s_data.relay = 1;
                break;
            }
            count++;
        }
    }

    return s_data;
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
    }else   
        std::cerr << "[Constructor] -> Received MQueues" << std::endl;

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
    std::cout << "\n\t [Constructor] -> tupdateFlags\n Creating thread at RR/" << param.sched_priority << std::endl;
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
    status[0] = pthread_create(&thread1,&attr, tupdateFlags, this);

    //thread creation Stream        -> lowest Priority
    SetupThread(50,&attr,&param);
    std::cout << "\n\t [Constructor] -> tstream \n Creating thread at RR/" << param.sched_priority << std::endl;
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
    status[1] = pthread_create(&thread2,&attr, tstream, this);

    //thread creation Sensors       -> high Priority
    SetupThread(90,&attr,&param);
    std::cout << "\n\t [Constructor] -> tsensors\n Creating thread at RR/" << param.sched_priority << std::endl;
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
    status[2] = pthread_create(&thread3,&attr, tsensors, this);

    //thread creation Relay         -> mid Priority
    SetupThread(70,&attr,&param);
    std::cout << "\n\t [Constructor] -> trelay \n Creating thread at RR/" << param.sched_priority << std::endl;
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);
    status[3] = pthread_create(&thread4,&attr, trelay, this);


    //test if threads were created and if there were any errors
    for (i=0; i < NUMTHREADS; i++){
        checkFail(status[i], i+1);
        std::cout << "\t\t[Constructor] -> Thread created: " << i+1 << std::endl;
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

    database.~database_sys();
    
}


//----MAIN FUNCTION-----

//Run() -> main process function all the main process threads will be activated here 
void houseSystem::run(){

    /*
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    */

    while(!control_flag);

    //pthread_exit(NULL);
    std::cout<< "Tasks Finalized" << std::endl;
    if(control_flag)
        std::cout << "FLAG CONTROL WORKS" << std::endl;

    if (mq_close(msgqueue) == -1) {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }

    if (mq_unlink(MQ_NAME) == -1) {
        perror("mq_unlink");
        exit(EXIT_FAILURE);
    }

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

    while(!instance->control_flag){
        pthread_mutex_lock(&instance -> mutdata);
        
        
        //check for relay activation
        sleep(1);

        if((database_data = instance->database.receive_data("/Notifications")) != ""){
            
            std::cout << "\t > Read from database: " << database_data << std::endl;
            instance->database_sen = parser_database(database_data);

            // -> check for variations
            //check if the value of the button differs from the database
            if(instance->database_sen.relay != instance->house_sen.relay){
                //update databasse with the value read from the sensor
                instance->house_sen.relay = instance->database_sen.relay;
                control_relay = 1;
                pthread_cond_signal(&instance->cvrelay);

            }
        }
        
        //check for condition signal tSensors because sensors were updated
        if(!instance->sensors)
            pthread_cond_wait(&instance->cvsensors, &instance->mutdata);
        else{
            
            std::cout << "\t::Updating Sensor Values to Database::" << std::endl;

            if(instance->database_sen.button != instance->house_sen.button){
                instance->database.flags_update(0, (instance->house_sen.button == 0 ? false : true));
                std::cout << "Updating BUTTON to Database with value " << instance->house_sen.button << std::endl;
            }
            if(instance->database_sen.motion != instance->house_sen.motion){
                instance->database.flags_update(1, (instance->house_sen.motion == 0 ? false : true));
                std::cout << "Updating MOTION to Database with value " << instance->house_sen.motion << std::endl;
            }
            if(instance->database_sen.door != instance->house_sen.door){
                instance->database.flags_update(2, (instance->house_sen.door == 0 ? false : true));
                std::cout << "Updating MAGNETIC to Database with value " << instance->house_sen.door << std::endl;
            }

            instance->sensors = 0;
        }

        pthread_mutex_unlock(&instance -> mutdata);
    }
    
    std::cout << "EXITING THREAD tUF" << std::endl;
    pthread_exit(NULL);
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

    while(instance->control_flag){
        //testing communication
        pthread_mutex_lock(&instance->mutdata);

        sleep(1);
        std::cout << "\t ::Waiting for SENSORS::" << std::endl;

        if(instance -> sensors){
            std::cout << "SENSORS READ " << count << std::endl;
            count++;
            instance->sensors = 0;

        } 
        pthread_mutex_unlock(&instance->mutdata);
    }
    //testing communication

   

    std::cout << "EXITING THREAD tSTREAM" << std::endl;
    pthread_exit(NULL);
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
    //char* temp[10];               //Use as secondary memory for mqueues TEST DONT KNOW
    int mq_count = 0;

    while(!instance->control_flag){

        sleep(1);
        pthread_mutex_lock(&instance ->mutsensors);

        bytes_received = mq_receive(instance->msgqueue, instance->data, sizeof(data), &instance->prio);
        instance->data[bytes_received] = '\0'; 
        if(bytes_received != NULL)
            std::cout << "MSG Received: " << instance->data << " msg number: " << ++mq_count << std::endl;
        
        //strcpy(temp[mq_count],instance->data);
        //std::cout << "::copy MSG Received: " << temp[mq_count] << " msg number: " << mq_count << std::endl;

        if(instance->attr_msg.mq_curmsgs != 0){ 


            instance->house_sen = parser_mqueues(instance->data);

            //UPDATE TO DATABASE
            instance->sensors = 1; //Signal to update flags to database;
            std::cout << "Arrived to sensors!!!" << std::endl;
            pthread_cond_signal(&instance->cvsensors);      //notify update_flags

        }
 
        //     switch (instance->data[0])
        //     {
        //     case 'M':               //case motion triggered
        //         instance->house_sen.motion = 1;

        //         instance->house_sen.button = 0;
        //         instance->house_sen.door = 0;
        //         std::cout << "MSG: MOTION DETECTED" << std::endl;
        //         break;
        //     case 'D':               //case door triggered
        //         instance->house_sen.door = 1;

        //         instance->house_sen.button = 0;
        //         instance->house_sen.motion = 0;
        //         std::cout << "MSG: DOOR MOVEMENT" << std::endl;
        //         break;  
        //     case 'B':               //case button pressed
        //         instance->house_sen.button = 1;
                
        //         instance->house_sen.door = 0;
        //         instance->house_sen.motion = 0;
        //         std::cout << "MSG: BUTTON PRESSED" << std::endl;
        //         break;
        //     default:
        //         instance->house_sen.button = 0;
        //         instance->house_sen.door = 0;
        //         instance->house_sen.motion = 0;
        //         break;
        //     }

        //     //UPDATE TO DATABASE
        //     instance->sensors = 1; //Signal to update flags to database;
        //     std::cout << "Arrived to sensors!!!" << std::endl;
        //     pthread_cond_signal(&instance->cvsensors);      //notify update_flags

        // }else
        //     std::cout << "NO Message Queues" << std::endl;


        pthread_mutex_unlock(&instance -> mutsensors);
    }

    std::cout << "EXITING THREAD tSENSORS" << std::endl;
    pthread_exit(NULL);
    
}

void* houseSystem::trelay(void* arg){
    /*
    -> This function is responsible for the enabling and disabling of the relay this function will be interacted
    by the value of its flag in the database
    */
    houseSystem* instance = static_cast<houseSystem*>(arg);
    std::cout << " 4 Relay in" << std::endl;
    

    //CONTROL RELAY
    while(!instance->control_flag){

        sleep(3);

        pthread_mutex_lock(&instance ->mutrelay);    
        
        while(!control_relay)
            pthread_cond_wait(&instance->cvrelay, &instance->mutrelay);

        //call Relay device driver

        std::cout << "--> Relay at value: " << instance->house_sen.relay << std::endl;

        control_relay = 0;      

        pthread_mutex_unlock(&instance ->mutrelay);
    }
    
    std::cout << "EXITING THREAD tRELAY" << std::endl;
    pthread_exit(NULL);
}

