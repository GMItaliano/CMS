#include "Cdaemon.h"

#include <string>
#include <cerrno>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <sys/ioctl.h>
#include <chrono>

#include "data_errors.h"

/*
instead of #define XXX "msg" -> constexpr const char* 'name' = "XXXXX"

funciona da mesma maneira de um define
*/

#define MQ_NAME "/MSGQUEUE_SENSORS"
#define MQ_MAXSIZE 50
#define MQ_MAXMSGS 5

#define BUTTON_N 10
#define MOTION_N 40
#define MAGNET_N 30

//static int count = 0;
//int value_sig = 0;
bool send_message = 0;
bool update = 0;



struct sensor_type{
	int type;
	bool value;
};

Cdaemon* Cdaemon::thisPtr = NULL;

//---- Other Functions ----


// void signal_handler(int sig) {
// 	switch(sig) {
// 		case SIGHUP:
// 			syslog(LOG_INFO,"Hangup signal catched");
// 			break;
// 		case SIGTERM:
// 			syslog(LOG_INFO,"Terminate signal catched");
// 			exit(0);
// 			break;
// 	}
// }


sensor_type parser_info(int value){

	int digit = value%10;

	value = value - digit;

	sensor_type parser;
	parser.type = value;
	parser.value = (bool)digit;

	return parser;
}


//-------------------------

//----Constructor & Destructor----

Cdaemon::Cdaemon():
	motion(isr_control),
	button(isr_control),
	door(isr_control)
{

	mq_unlink(MQ_NAME);

    //message queues initialization attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = MQ_MAXMSGS;
    attr.mq_msgsize = MQ_MAXSIZE;
    attr.mq_curmsgs = 0;

    //create msg queue
    msgqueue = mq_open(MQ_NAME, O_CREAT | O_WRONLY, 0666, &attr);		//create the msg queue with write only 
    if(msgqueue == (mqd_t)-1){
		logError("mq_open");
		mq_unlink(MQ_NAME);
	}

	thisPtr = this;

}

Cdaemon::~Cdaemon(){
	mq_unlink(MQ_NAME);

	// button.disable();
	// door.disable();
	// motion.disable();
}

//--------------------------------

//---- main functions ----

void Cdaemon::run(){

	//enable the peripherals device drivers
	button.enable();
	door.enable();
	motion.enable();

	//test
	char* msg[5] = {"B:1 M:1 D:1", "B:0 M:0 D:1", "B:0 M:1 D:0", "B:1 M:0 D:0", "B:0 M:0 D:0"};
	//char* end_msg = "END MESSAGE";
	unsigned int prio = 0;

	// Set the desired time duration
    auto duration = std::chrono::seconds(5);
    // Get the current time
    auto startTime = std::chrono::high_resolution_clock::now();

	while(1){

		auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);

		if(send_message){
			send_message = 0;
			logError("[DEBUG] INSIDE LOOP FOR SENDING MESSAGE");

			if(button_flag == 1){
				button_flag = 0;
				logError("[BUTTON] ENABLED");
				mq_send(msgqueue, msg[3], sizeof(msg), 3);
				
			}else if(motion_flag == 1){
				motion_flag = 0;
				logError("[MOTION] ENABLED");
				mq_send(msgqueue, msg[2], sizeof(msg), 1);
			}else if(door_flag == 1){
				door_flag = 0;
				logError("[DOOR] ENABLED");
				mq_send(msgqueue, msg[1], sizeof(msg), 2);
			}
			startTime = std::chrono::high_resolution_clock::now();

		}else if((elapsedTime >= duration) && (update == 0)){
			logError("[TIMEOUT] Reseting values to zero");
			startTime = std::chrono::high_resolution_clock::now();
			mq_send(msgqueue, msg[4], sizeof(msg), 0);
		}
			
			


	}

	mq_close(msgqueue);
	mq_unlink(MQ_NAME);
}

void Cdaemon::stop(){

}



//------------------------

//----- ISR Functions ----
//this functions will send to the message queue the determined sensor that was triggered
//int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);

#define DOORMSG 	"Dtrig" 	//door triggered
#define MOTIONMSG 	"Mtrig"		//motion triggered
#define BUTTONMSG	"Btrig"		//button triggered

unsigned int prio = 0; 			// Low prio  -> 0 < 1 < 2 <- High prio

void Cdaemon::door_isr(){			//Mid msg Queue Priority	
	
	logError(DOORMSG);
	if(mq_send(msgqueue, DOORMSG, strlen(DOORMSG)+1, 1));

}

void Cdaemon::motion_isr(){			//Low msg Queue Priority

	logError(MOTIONMSG);
	if(mq_send(msgqueue, MOTIONMSG, strlen(MOTIONMSG)+1, 0));

}

void Cdaemon::button_isr(){			//Highest msg Queue Priority

	logError(BUTTONMSG);
	if(mq_send(msgqueue, BUTTONMSG, strlen(BUTTONMSG)+1, 2));

}

void Cdaemon::isr_control(int control, siginfo_t *info, void *unused) {
    std::string msg;  // Adjust the size as needed

    switch (control) {
        case SIGUSR2:
			logError("[SIGNAL] Sigusr2 triggered button ISR");
			thisPtr->button_flag = 1;
            send_message = 1;
            //thisPtr->button_isr();
            break;

        case SIGUSR1:
			logError("[SIGNAL] Sigusr1 triggered door/motion ISR");
			//msg = "[PID] Motion pid: " + std::to_string(thisPtr->motion.get_pid());

			//test
			msg = "[DEBUG] SENSOR VALUE: " + std::to_string(info->si_value.sival_int);
			logError(msg);

			if(info->si_value.sival_int == 40){
				msg = "[VALUE] sensor Motion with value: " + std::to_string(info->si_value.sival_int) + '\0';
				logError(msg);
				thisPtr->motion_flag = 1;
				//thisPtr->motion_isr();
			}
			if(info->si_value.sival_int == 30){
				msg = "[VALUE] sensor Magnet with value: " + std::to_string(info->si_value.sival_int) + '\0';
				logError(msg);
				thisPtr->door_flag = 1;
				//thisPtr->door_isr();
			}
            send_message = 1;
            break;

        default:
			//adicionar asserts para resolver possiveis erros 
            send_message = 0;
            logError("[ERROR] Signal was undefined");
            break;
    }
}


//------------------------
