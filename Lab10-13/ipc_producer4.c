#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>

#define MYSOCK_PATH "/tmp/mysocket_path"
#define MYSOCK_BUF_SIZE 64

int main(int argc, char* argv[]) {
	const char* studentId = "2012136121";
	char myPid[16];	
	sprintf(myPid, "%d", getpid());
	char consumerPid[MYSOCK_BUF_SIZE], studentName[MYSOCK_BUF_SIZE];
	char buf[MYSOCK_BUF_SIZE];

	unlink(MYSOCK_PATH);
	int mySocket = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un serverAddress, clientAddress;
	serverAddress.sun_family = AF_UNIX;
	strcpy(serverAddress.sun_path, MYSOCK_PATH);
	bind(mySocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	listen(mySocket, 5);

	char cmdBuf[1024];
	while(1) {
		scanf("%s", cmdBuf);
		if(strcmp(cmdBuf, "start")==0) {
			socklen_t clientLen = sizeof(clientAddress);
			int clientSocket = accept(mySocket,
					(struct sockaddr*)&clientAddress, &clientLen);
			// write 1.myPid 2.studentId 
			strcpy(buf, myPid);
			write(clientSocket, buf, MYSOCK_BUF_SIZE);
			strcpy(buf, studentId);
			write(clientSocket, buf, MYSOCK_BUF_SIZE);

			// read 1.consumerPid 2.studentName
			read(clientSocket, consumerPid, MYSOCK_BUF_SIZE);
			read(clientSocket, studentName, MYSOCK_BUF_SIZE);
			close(clientSocket);
			printf("producer4_pid:%s, consumer4_pid:%s, student_id:%s, student_name:%s\n", myPid, consumerPid, studentId, studentName);
		}
		if(strcmp(cmdBuf, "exit")==0) {
			printf("exit program...\n");
			exit(EXIT_SUCCESS);
		}
	}
	exit(EXIT_FAILURE);
}
