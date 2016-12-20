#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>

#define MYSHM_KEY 11423
#define MYSHM_SIZE 64

#define ANYONE_WRITED 0
#define PRODUCER_WRITED 1
#define CONSUMER_WRITED 2

char* shm = NULL;
int main(int argc, char* argv[]) {
	const char* studentId = "2012136121";
	char myPid[16];	
	sprintf(myPid, "%d", getpid());
	char consumerPid[MYSHM_SIZE], studentName[MYSHM_SIZE];
	int shmid = shmget(MYSHM_KEY, MYSHM_SIZE, 0666 | IPC_CREAT);
	if(shmid==-1) {
		perror("shmget failed\n");
		exit(EXIT_FAILURE);
	}
	shm = shmat(shmid, NULL, 0);
	if(shm == (char*)(-1)) {
		perror("shmat failed\n");
		exit(EXIT_FAILURE);
	}
	char cmdBuf[1024];
	while(1) {
		scanf("%s", cmdBuf);
		if(strcmp(cmdBuf, "start")==0) {
			// write 1.myPid 2.studentId 
			strcpy(shm+1, myPid);
			shm[0] = PRODUCER_WRITED;
			while(shm[0]!=ANYONE_WRITED)
				sleep(0);
			strcpy(shm+1, studentId);
			shm[0] = PRODUCER_WRITED;

			// read 1.consumerPid 2.studentName
			while(shm[0]!=CONSUMER_WRITED)
				sleep(0);
			strncpy(consumerPid, shm+1, MYSHM_SIZE-1);
			shm[0] = ANYONE_WRITED;
			while(shm[0]!=CONSUMER_WRITED)
				sleep(0);
			strncpy(studentName, shm+1, MYSHM_SIZE-1);
			shm[0] = ANYONE_WRITED;
			printf("producer2_pid:%s, consumer2_pid:%s, student_id:%s, student_name:%s\n", myPid, consumerPid, studentId, studentName);
		}
		if(strcmp(cmdBuf, "exit")==0) {
			printf("exit program...\n");
			exit(EXIT_SUCCESS);
		}
	}
	exit(EXIT_FAILURE);
}
