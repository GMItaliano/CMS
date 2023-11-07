#include "daemon.h"

#include <string.h>
#include <cerrno>
#include <cstdio>

#define MQ_NAME "/MSGQUEUE_SENSORS"
#define MQ_MAXSIZE 50
#define MQ_MAXMSGS 5


//---- Other Functions ----

void signal_handler(int sig) {
	switch(sig) {
		case SIGHUP:
			syslog(LOG_INFO,"Hangup signal catched");
			break;
		case SIGTERM:
			syslog(LOG_INFO,"Terminate signal catched");
			exit(0);
			break;
	}
}

//-------------------------

//----Constructor & Destructor----

Cdaemon::Cdaemon(){

    //message queues initialization attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = MQ_MAXMSGS;
    attr.mq_msgsize = MQ_MAXSIZE;
    attr.mq_curmsgs = 0;

    //create msg queue
    msgqueue = mq_open(MQ_NAME, O_CREAT | O_WRONLY, 0666, &attr);		//create the msg queue with write only 
    if(msgqueue == (mqd_t)-1)
		std::perror("mq_open");

}

Cdaemon::~Cdaemon(){

}

//--------------------------------

//---- main functions ----

void Cdaemon::run(){
	//do sensors activation


	while(!SIGTERM){			

		if(SIGUSR1 == 1)		
			isr_control(SIGUSR1);
		else if(SIGUSR2 == 1)
			isr_control(SIGUSR2);

	}

}

void Cdaemon::stop(){

}

void Cdaemon::set_flags(int type, bool value){			//conreol the flags of the sensors

	switch(type){
		case 1: 
			door_flag = value;
			break;
		case 2:
			motion_flag = value;
			break;
		default:					
			break;
	}

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

	if(mq_send(msgqueue, DOORMSG, strlen(DOORMSG)+1, 1));

}

void Cdaemon::motion_isr(){			//Low msg Queue Priority

	if(mq_send(msgqueue, MOTIONMSG, strlen(MOTIONMSG)+1, 0));

}

void Cdaemon::button_isr(){			//Highest msg Queue Priority

	if(mq_send(msgqueue, BUTTONMSG, strlen(BUTTONMSG)+1, 2));

}

void Cdaemon::isr_control(int control){

	switch(control){
		case SIGUSR1:					//Daemon signal that will control the button trigger as is the one with the highest priority
			Cdaemon::button_isr();
		break;
		case SIGUSR2:					//daemon signal responsile to control the trigger of both the motion sensor and door sensor

			if(door_flag)
				Cdaemon::door_isr();
			else if(motion_flag)
				Cdaemon::motion_isr();

		break;
		default:
		break;
	}

}

//------------------------
