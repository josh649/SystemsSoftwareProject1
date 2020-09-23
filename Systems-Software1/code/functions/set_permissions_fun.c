#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/types.h>

//function for setting folder lock and unlock 
int set_permissions_fun(int rev){
	//if  lock is needed 
	if(rev == 0){
		
		int res;
		syslog(LOG_INFO, "Locking HTML");

		// hmtl folder gets locked with read only privileges
		res = system("chmod 444 /var/www/html");

		if(res == -1){
		
			syslog(LOG_INFO, "failed to lock folder html folder");
			return -1;
		
		}
		else{
			syslog(LOG_INFO, " html folder locked successfully ");
			return 1;
		}

	}
	else if(rev == 1)
	{
		// if unlock is needed
		int res;
		syslog(LOG_INFO, "Unlocking html folder");

		// Unlock the html folder 
		res = system("chmod 777 /var/www/html");

		if(res == -1){
		
			syslog(LOG_INFO, "failed to unlock html folder");
			return -1;
		
		}
		else{
			syslog(LOG_INFO, "successfull unlock of html folder");
			return 1;
		}

	}
	else{
	
		syslog(LOG_INFO, "unlock neither failed or was sucessful check code");
		return 0;
	
	}

}



