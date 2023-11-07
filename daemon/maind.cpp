#include <cstdio>
#include <syslog.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#include "daemon.h"

int main(){
    
    pid_t pid, sid;
	const time_t current_time = time(nullptr);

	pid = fork(); // create a new process

	if (pid < 0) { // on error exit
		std::perror("fork");
	exit(EXIT_FAILURE);
	}

	if (pid > 0){  
		std::printf("Deamon PID: %d\n", pid);	
		exit(EXIT_SUCCESS); // parent process (exit)
	}
	sid = setsid(); // create a new session

	if (sid < 0) { // on error exit
		std::perror("setsid");
		exit(EXIT_FAILURE);
	}
	// make '/' the root directory
	if (chdir("/") < 0) { // on error exit
		std::perror("chdir");
		exit(EXIT_FAILURE);
	}
	umask(0);
	close(STDIN_FILENO);  // close standard input file descriptor
	close(STDOUT_FILENO); // close standard output file descriptor
	close(STDERR_FILENO); // close standard error file descriptor

	Cdaemon dprocess;
    dprocess.run();

exit(EXIT_SUCCESS);

}
