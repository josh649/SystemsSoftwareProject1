#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/types.h>

// Function called on the command line to run the transfer
int system_transfer() {

	int msg = set_permissions_fun(0);
	if(msg == 1) {

		int message;
		// Run transfer which is a cp copy command 
		message = system("cp -a -u /var/www/html/intra/. /var/www/html/live/");

		if(message == -1){

			syslog(LOG_INFO,"CP copy command failed make sure you are root");
			return -1;

		}
		else{

			int msg2 = set_permissions_fun(1);

			if(msg2 == 1){

				return 1;

			}else{

				syslog(LOG_INFO, "Permissions failed to unlock for the transfer");
				return -1;

			}


		}

	}
	else{

		syslog(LOG_INFO,"Permissions failed to lock for the Transfer");
		return -1;

	}
}

//transfer functions ran by my daemon
void transfer_fun() {

	// Fork and create pipe

	int fd[2], nbytes;

	char readbuffer[100];

	pipe(fd);

	syslog(LOG_INFO, "Transfer now running");

	int t_pid = fork();

	// parent process waits for child 
	if(t_pid > 0 ){

		syslog(LOG_INFO,"waiting for the fork to complete the transfer");
		close(fd[1]);
		nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
		syslog(LOG_INFO, readbuffer);
		exit(0);
	}
	else if(t_pid == 0){

		// transfer is ran from child process

		syslog(LOG_INFO, "Transfer child processs is now  running  the backup");
		close(fd[0]);
		int response;
		//call the transfer function and check if it was successful
		response = system_transfer();

		if(response == -1){
			// Transfer was unsuccessful
			char ret_msg [] = "Transfer process failed ";
			write(fd[1], ret_msg, (strlen(ret_msg) + 1));


		}
		else{
			// transfer was successful return to the parent 
			char ret_msg [] = "Transfer a success";
			write(fd[1],ret_msg,(strlen(ret_msg)+1));
								
			
		
		}

	}

}


