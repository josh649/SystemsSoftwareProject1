my-own-daemon : main.o backup_fun.o transfer_fun.o set_permissions_fun.o auditor_fun.o
	gcc -o my-own-daemon main.o backup_fun.o transfer_fun.o set_permissions_fun.o auditor_fun.o

main.o : main.c
	gcc -c main.c

backup_fun.o : functions/backup_fun.c functions/functions.h
	gcc -c functions/backup_fun.c

transfer_fun.o : functions/transfer_fun.c functions/functions.h 
	gcc -c functions/transfer_fun.c

set_permissions_fun.o : functions/set_permissions_fun.c functions/functions.h
	gcc -c functions/set_permissions_fun.c

auditor_fun.o : functions/auditor_fun.c functions/functions.h
	gcc -c functions/auditor_fun.c

clean :
	rm my-own-daemon main.o backup_fun.o transfer_fun.o auditor_fun.o set_permissions_fun.o
