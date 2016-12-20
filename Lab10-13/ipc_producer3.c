#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define MYMSG_KEY 12188
#define MYMSG_SIZE 64

#define PRODUCER_PUSHED 1
#define CONSUMER_PUSHED 2

typedef struct _MyMsg {
	long msgType;
	char msgBody[MYMSG_SIZE];
} MyMsg;

int main(int argc, char* argv[]) {
	const char* studentId = "2012136121";
	char myPid[16];	
	sprintf(myPid, "%d", getpid());
	char consumerPid[MYMSG_SIZE], studentName[MYMSG_SIZE];
	int msgKey = msgget(MYMSG_KEY, 0666 | IPC_CREAT);
	if(msgKey==-1) {
		perror("msgget failed\n");
		exit(EXIT_FAILURE);
	}
	MyMsg msgBuf; 
	// clear message queue
	while(msgrcv(msgKey, &msgBuf, sizeof(MyMsg)-sizeof(long),
				0, IPC_NOWAIT)!=-1);
	char cmdBuf[1024];
	while(1) {
		scanf("%s", cmdBuf);
		if(strcmp(cmdBuf, "start")==0) {
			// write 1.myPid 2.studentId 
			msgBuf.msgType = PRODUCER_PUSHED;
			strcpy(msgBuf.msgBody, myPid);
			msgsnd(msgKey, &msgBuf, sizeof(MyMsg)-sizeof(long), 0);
			msgBuf.msgType = PRODUCER_PUSHED;
			strcpy(msgBuf.msgBody, studentId);
			msgsnd(msgKey, &msgBuf, sizeof(MyMsg)-sizeof(long), 0);

			// read 1.consumerPid 2.studentName
			msgrcv(msgKey, &msgBuf, sizeof(MyMsg)-sizeof(long),
					CONSUMER_PUSHED, 0);
			strcpy(consumerPid, msgBuf.msgBody);
			msgrcv(msgKey, &msgBuf, sizeof(MyMsg)-sizeof(long),
					CONSUMER_PUSHED, 0);
			strcpy(studentName, msgBuf.msgBody);
			printf("producer3_pid:%s, consumer3_pid:%s, student_id:%s, student_name:%s\n", myPid, consumerPid, studentId, studentName);
		}
		if(strcmp(cmdBuf, "exit")==0) {
			printf("exit program...\n");
			exit(EXIT_SUCCESS);
		}
	}
	exit(EXIT_FAILURE);
}
