#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/types.h>

//function called by command line to run backup
int system_backup() {

	//function is called to lock down the folder within back  
	int msg = set_permissions_fun(0);
	if(msg == 1) {

		// if txt was succesfully lock now carry out the backing up 
		int message;
		message = system("cp -a -u /var/www/html/intra/. /wwwbaks/");

		// If backup failed do not backup the file 
		if(message == -1){

			syslog(LOG_INFO,"CP Command failed for the backup");
			return -1;

		}
		else{
			// If backup was successfull
			// unlock the txt file 
			int msg2 = set_permissions_fun(1);

			if(msg2 == 1){
				// If the file file has been unlocked 
				return 1;

			}else{
				// If file was fail to be unlocked 
				syslog(LOG_INFO, "file was not unlocked set permission failed");
				return -1;

			}


		}

	}
	else{

		syslog(LOG_INFO,"Permissions failed to lock - backup");
		return -1;

	}
}


void backup_fun() {

	// Fork and create pipe

	int fd[2], nbytes;

	char readbuffer[100];

	pipe(fd);

	syslog(LOG_INFO, "Running Backup");

	int bak_pid = fork();

	// if parent wait for child and read pipe
	if(bak_pid > 0 ){

		syslog(LOG_INFO,"Main process waiting fork to complete backup");
		close(fd[1]);
		nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
		syslog(LOG_INFO, readbuffer);
		exit(0);
	}
	else if(bak_pid == 0){

		// Run backup from child

		syslog(LOG_INFO, "Backup child process running backup");
		close(fd[0]);
		int response_bup;
		//call the backup function and await result
		response_bup = system_backup();

		if(response_bup == -1){
			// Backup failed
			char ret_msg [] = "Backup process failed check logs";
			write(fd[1], ret_msg, (strlen(ret_msg) + 1));


		}
		else{

			// Return to parent if successful
			char ret_msg [] = "Backup was successful";
			write(fd[1],ret_msg,(strlen(ret_msg)+1));


		}

	}

}




