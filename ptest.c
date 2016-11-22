#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char *argv[]){
	time_t timer;
	char buffer[26];
	struct tm* tm_info;
	
	while(1){ // loop
		time(&timer);
		tm_info = localtime(&timer);

		strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info); // input currnet time data in buffer
		fflush(stdout); // print buffer data
		printf("%s\n", buffer); // print current time

		sleep(2); // 2second interval
	}
	exit(EXIT_SUCCESS);
}
