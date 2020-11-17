// Part 3: list.c 

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int list() {

	pid_t child_reset = fork();

	// error check for fork
	if (child_reset < 0) {	

		perror("ERROR: list*: child_reset: fork: ");
		return 1;

	}
	else if (child_reset == 0) {			// child_reset code

		// clear terminal screen
		char *args[] = {"reset", NULL};		
		execvp("reset", args);	
			
	}
	else {						// parent code

		// wait for child_reset to terminate		
		wait(NULL);
		
		pid_t child_list = fork();

		// error check for fork
		if (child_list < 0) {
	
			perror("ERROR: list*: child_list: fork: ");
			return 1;

		}
		else if (child_list == 0) {			// child_list code

			// print a detailed list of all the content in current directory
			char *args[] = {"ls", "-l", "-a", "-h", NULL};
			execvp("ls", args);	

		}
		else {						// parent code

			// wait for child_list to terminate			
			wait(NULL);
			
			pid_t child_text = fork();
			
			// error check in forking
			if (child_text < 0) {

				perror("list*: child_text: ");
				return -1;

			}
			else if (child_text == 0) {		// child_text code
		
				// open file				
				int fd = open("t1.txt", O_CREAT | O_RDWR, S_IRWXU);

				// error check for open
				if (fd < 0) {
				
					perror("ERROR: list*: fd: open: ");
					exit(1);				

				}
				
				// make stdout go to file
				dup2(fd, 1);				
						
				close(fd);				
				
				// print a detailed list of all the content in currect directory to file
				char *args[] = {"ls", "-l", "-a", "-h", NULL};
				execvp("ls", args);			
				
			}
			else {					// parent code

				// wait for child_text to terminate				
				wait(NULL); 	
				
				pid_t child_rename = fork();
				
				// error check for fork
				if (child_rename < 0) {	
		
					perror("ERROR: list*: child_rename: fork: ");
					return 1;

				}
				else if (child_rename == 0) {			// child_remain code

					// change name of file
					char *args[] = {"mv", "t1.txt", "tree.txt", NULL};
					execvp("mv", args);	
		
				}
				else {						// parent code

					// wait for child_rename to terminate
					wait(NULL);		
					
					// TESTING PURPOSES
					printf("<-------------------- FINISHED: list* -------------------->\n");

				}
			}
		}
	}
}

int main(int argc, char *argv[]) {

	list();		// list*

	return 0;
}

