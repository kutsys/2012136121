#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>

#define PRODUCER_FIFO "/tmp/prdc_fifo"
#define CONSUMER_FIFO "/tmp/csm_fifo"


int main(int argc, char* argv[]) {
	const char studentName[PIPE_BUF+1] = "Jeong Ho Yeob";
	char myPid[PIPE_BUF+1], producerPid[PIPE_BUF+1], studentId[PIPE_BUF+1];	
	sprintf(myPid, "%d", getpid());
	if(access(CONSUMER_FIFO, F_OK)==-1) {
		int res = mkfifo(CONSUMER_FIFO, 0777);
		if(res!=0) {
			printf("make fifo failed. exit program...\n");
			exit(EXIT_FAILURE);
		}
	}
	int pipeId = -1;
	while(1) {
		// open producer fifo for read
		pipeId = open(PRODUCER_FIFO, O_RDONLY);
		if(pipeId==-1) {
			printf("producer fifo open failed.\n");
			continue;
		}
		// read 1.myPid 2.studentId
		read(pipeId, producerPid, PIPE_BUF);
		read(pipeId, studentId, PIPE_BUF);
		close(pipeId);
		printf("producer_pid:%s, studnet_id:%s\n", producerPid
				, studentId);

		// open consumer fifo for write
		pipeId = open(CONSUMER_FIFO, O_WRONLY);
		if(pipeId==-1) {
			printf("consumer fifo open failed.\n");
			continue;
		}
		// write 1.consumerPid 2.studentName
		write(pipeId, myPid, PIPE_BUF);
		write(pipeId, studentName, PIPE_BUF);
		close(pipeId);
	}
	exit(EXIT_FAILURE);
}
