#include<stdio.h>
#include<stdlib.h>
#include<syslog.h>
#include<unistd.h>
#include"functions/functions.h"
#include<sys/types.h>
#include<string.h>
#include <time.h>


int main(int argc, char *argv[]) {
	
	//start and instant the logger
	openlog("main-system-software",LOG_PID|LOG_CONS,LOG_USER);
	//look for functions typed 
	
	if(argc > 1){

		//arument passed must be less than 50 characters
		char argument[50];
		strcpy(argument,argv[1]);

		if(strcmp("backup_fun",argument) == 0){

			backup_fun();
			syslog(LOG_INFO,"\n backup has now been ran \n");
			exit(0);

		}
		else if(strcmp("transfer_fun",argument) == 0){

			transfer_fun();
			syslog(LOG_INFO,"\n transfer has now been ran \n");
			exit(0);

		}
		else{

			printf("\n function doesnt exist \n");
			exit(0);

		}
	}

	// daemon creation code
	int pid = fork();

	if (pid > 0) {

		printf("parent process exiting");
		exit(EXIT_SUCCESS);

	}
	else if (pid == 0) {

		printf("child process now running ");
		int sid;	
		sid = setsid();

		printf("SID : %d",sid);

		if (sid <  0) {
			printf("SID Failure");	
			exit(EXIT_FAILURE);
		}

		umask(0);

		if (chdir("/") < 0) {

			printf("directory failure");
			exit(EXIT_FAILURE);

		}

		int x;

		for (x = sysconf(_SC_OPEN_MAX); x>=0; x--){

			close (x);
		}

		syslog(LOG_INFO, "my own daemon is now running");
		// make sure backup is only ran once 

		int backup_check;
		backup_check = 0;
		

		//This while loop is ran constantly the entire time to check 
		//for the time and current time to be alligned

		while (1) {

			//Get the current time and check for a match
			//if the match is correct run the following bellow

			time_t currTime;
			time(&currTime);

			struct tm *sTime = localtime(&currTime);

			int current_hour = sTime->tm_hour;
			int current_minute = sTime->tm_min;

			//if its now the correct time set back to 0 so it will
			//be backed up 
			if(current_hour == 18 && current_minute == 48 &&  backup_check == 0){				
			//if time and check is correct run these functions  
			

				backup_check = 1;
				audit_fun();
				backup_fun();
				transfer_fun();
			}
			
			//backups cannot be done for this minute 
			//This stops mutiple backups being carried out
			//once the miute moves on backups can be carried out
			//again   
			if (current_minute !=59 && backup_check== 1){

				syslog(LOG_INFO,"Backup back open ");
				backup_check = 0;
			}



		}

		closelog();
	}

	return 0;
}

