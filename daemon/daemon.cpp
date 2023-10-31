#include "daemon.h"

#define MQ_NAME "SENSORS_MQ"
#define MQ_MAXSIZE 10
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

daemon::daemon(){

    //message queues initialization attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = MQ_MAXMSGS;
    attr.mq_msgsize = MQ_MAXSIZE;
    attr.mq_curmsgs = 0;

    //create msg queue
    msgqueue = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if(msgqueue == )


}

daemon::~daemon(){

}

//--------------------------------

//---- main functions ----

void daemon::run(){

}

void daemon::stop(){

}

//------------------------

//----- ISR Functions ----
//this functions will send to the message queue the determined sensor that was triggered
//int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);

#define DOORMSG 	"Dtrig" 	//door triggered
#define MOTIONMSG 	"Mtrig"		//motion triggered
#define BUTTONMSG	"Btrig"		//button triggered

void daemon::door_isr(){		

	

}

void daemon::motion_isr(){

}

void daemon::button_isr(){

}

//------------------------
