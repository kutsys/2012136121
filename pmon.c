#include<sys/types.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

// get PID(process id) belong processName
pid_t getPid(char *processName){
	FILE *pRead;
	int len;
	char buffer[20];
	char command[100];
	
	//String copy
	strcpy(command, "pidof ");
	// String add 
	strcat(command, processName);
	
	if((pRead = popen(command, "r")) == NULL){
		printf("popen error\n");
	}

	while((len = fscanf(pRead, "%s", buffer)) > 0){
		pclose(pRead);
		return (pid_t)atoi(buffer);
	}
	
	pclose(pRead);

	return (pid_t)-1;
}

void intervalAlarm(int sig){
	pid_t pid = getPid("ptest");


	fflush(stdout);
	// process not exist
	if(pid < 0)
		printf("not existed\n");
	// process exist
	else
		printf("running\n");
	
	alarm(5);
}

int main(int argc, char* argv[]){
	pid_t  pid;
	char command;
	struct sigaction act;
	
	//
	act.sa_handler = intervalAlarm;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGALRM, &act, NULL);
	// input SIGALRM -> act intervalAlarm func
	alarm(5);// 5 second after SIGALRM

	while(1){
		// pid is a ptest's process id
		pid = getPid("ptest");
		printf(">>");
		scanf("%c", &command);
		// pmon command
		switch(command){
		case 'Q':
		case 'q':
			printf("exit pmon\n");
			// pmon quit, but ptest is existed
			if(pid >= 0)
				printf("warning: ptest is still running\n");
			exit(EXIT_SUCCESS); // program exit
		case 'K':
		case 'k':
			// kill the ptest's process
			if(pid >= 0)
				kill(pid, SIGKILL);
			break;
		case 'S':
		case 's':
			// if exist ptest
			if(pid >= 0){
				printf("already executed\n");
			}
			else{
			// system call ptest file
				system("./ptest &");
			}
			break;
		case 'R':
		case 'r':
			// if exist ptest first kill
			if(pid >= 0)
				kill(pid, SIGKILL);
			// system call ptest file
			system("./ptest &");
		}
	}

	exit(EXIT_SUCCESS);
}
