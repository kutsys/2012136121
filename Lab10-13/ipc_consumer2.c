#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MYSHM_KEY 11423
#define MYSHM_SIZE 64

#define ANYONE_WRITED 0
#define PRODUCER_WRITED 1
#define CONSUMER_WRITED 2

char* shm = NULL;
void shm_free(int sig) {
	if(shm!=NULL)
		shmdt(shm);
}
int main(int argc, char* argv[]) {
	const char* studentName = "Jeong Ho Yeob";
	char myPid[16];	
	sprintf(myPid, "%d", getpid());
	char producerPid[MYSHM_SIZE], studentId[MYSHM_SIZE];
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
	(void) signal(SIGTERM, shm_free);
	(void) signal(SIGKILL, shm_free);
	(void) signal(SIGSTOP, shm_free);
	while(1) {
		// read 1.myPid 2.studentId 
		while(shm[0]!=PRODUCER_WRITED)
			sleep(0);
		strncpy(producerPid, shm+1, MYSHM_SIZE-1);
		shm[0] = ANYONE_WRITED;
		while(shm[0]!=PRODUCER_WRITED)
			sleep(0);
		strncpy(studentId, shm+1, MYSHM_SIZE-1);
		shm[0] = ANYONE_WRITED;
		printf("producer_pid:%s, student_id:%s\n",
				producerPid, studentId);

		// write 1.consumerPid 2.studentName
		strcpy(shm+1, myPid);
		shm[0] = CONSUMER_WRITED;
		while(shm[0]!=ANYONE_WRITED)
			sleep(0);
		strcpy(shm+1, studentName);
		shm[0] = CONSUMER_WRITED;
	}
	exit(EXIT_FAILURE);
}
