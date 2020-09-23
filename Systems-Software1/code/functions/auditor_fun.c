#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/types.h>


int system_audit(){
	//the audit is done by running an ausearch and then piping this into the auditfile.txt
	int message;
	message = system("sudo ausearch -f  /var/www/html/ > /auditfiles/auditfile.txt ");
	
// dipslays result of audit being successfull or not 
	if(message == -1){

		syslog(LOG_INFO,"Ausearch has failed");
		return -1;

	}
	else{

		syslog(LOG_INFO, "Audit has been recorded");
		return 1;
	}


}

// This is audit function is ran by our my own daemon 
void audit_fun() {

	// fork and pipe are created 
	int fd[2], nbytes;
	char readbuffer[100];
	pipe(fd);
	syslog(LOG_INFO, "Running Audit");
	int aud_pid = fork();

	// If parent wait and read pipe 
	if(aud_pid == 0){

		syslog(LOG_INFO,"Main process waiting for audit");
		close(fd[1]);
		nbytes = read(fd[0], readbuffer, sizeof(readbuffer));	
		syslog(LOG_INFO, readbuffer);
		exit(0);
	}
	else {
		// If the child process gets ran with the audit function send the information back to the parent by piping.
		syslog(LOG_INFO, "The audit child process is currently running ");
		close(fd[0]);
		int response_bup;

		response_bup = system_audit();

		if(response_bup == -1){

			char ret_msg [] = "The audit has failed";
			write(fd[1], ret_msg, (strlen(ret_msg) + 1));

		}
		else{
			char ret_msg [] = "The audit has been successfull";
			write(fd[1], ret_msg, (strlen(ret_msg) + 1));
		}



	}



}
