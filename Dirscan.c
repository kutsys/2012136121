#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void printdir(char *dir, int depth) {
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if((dp = opendir(dir)) == NULL) {
		fprintf(stderr,"cannot open directory : %s\n", dir);
		return;
	}
	chdir(dir);

	while((entry = readdir(dp)) !=NULL) {
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) {
		/* Found a directory, but ignore . and ..*/
			if(strcmp(".",entry->d_name) == 0 ||
			strcmp("..",entry->d_name) == 0) continue;
		//This code print folder, ignore hidden folder
		// folder's first name is .(hidden),don't anything
		if(entry->d_name[0] == '.') {
		}
		else { // print folder name
			printf("%*s%s\n",depth,"",entry->d_name);
		}

		/* Recurs at a new indent level*/
		/* This code is not need homework2
		because this code explore subdirectory
		printdir(entry->d_name,depth +4);
		*/
		} else {
		// This code print file,ignore hidden file
		// file's first name is .(hidden), don't anything
			if(entry->d_name[0]=='.') {
			}else { // print file name
				printf("%*s%s\n",depth,"",entry->d_name);
			}
		}
	}
	chdir("..");
	closedir(dp);
}

int main() {
	printf("Directory scan of /home:\n");
	printdir("/home/hoyeop",0);
	printf("done.\n");
	exit(0);
}

	

