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
	const char* studentName = "Jeong Ho Yeob";
	char myPid[16];	
	sprintf(myPid, "%d", getpid());
	char producerPid[MYSOCK_BUF_SIZE], studentId[MYSOCK_BUF_SIZE];
	char buf[MYSOCK_BUF_SIZE];

	int mySocket = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un serverAddress;
	serverAddress.sun_family = AF_UNIX;
	strcpy(serverAddress.sun_path, MYSOCK_PATH);
	socklen_t addressLen = sizeof(serverAddress);

	while(1) {
		int result = connect(mySocket,
				(struct sockaddr*)&serverAddress, addressLen);
		if(result!=0) {
			sleep(0);
			continue;
		}
		// read 1.producerPid 2.studentId 
		read(mySocket, producerPid, MYSOCK_BUF_SIZE);
		read(mySocket, studentId, MYSOCK_BUF_SIZE);
		printf("producer_pid:%s, student_id:%s\n",
				producerPid, studentId);

		// write 1.myPid 2.studentName
		strcpy(buf, myPid);
		write(mySocket, buf, MYSOCK_BUF_SIZE);
		strcpy(buf, studentName);
		write(mySocket, buf, MYSOCK_BUF_SIZE);
		close(mySocket);
		mySocket = socket(AF_UNIX, SOCK_STREAM, 0);
	}
	exit(EXIT_FAILURE);
}
