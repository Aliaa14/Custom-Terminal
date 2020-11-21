#include <unistd.h>
#include <errno.h>
#include <sys/wait.h> 
#include <stdbool.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h> 
#include<fcntl.h> 
#include<stdlib.h> 
#include <string.h>
#include <limits.h>

// ----------------------------- TREE 
void tree(){
     struct stat status = {0};
     int fileDescriptor;

     // check if Directory /Dir0 already does not exist
     if (stat("./Dir0", &status) == -1){
          //create directory Dir0, 0777 default mode with widest possible access
          if(mkdir("./Dir0", 0777) == -1){
               perror("Error while creating directory");
               return;
          }

          // changing directory to Dir0
          if (chdir("Dir0") != 0){
               perror("chdir() directory change to Dir0 failed");
               return;
          }
          
          // Creating the txt files now as follows inside Dir0, when done close the files
          fileDescriptor = open("t1.txt", O_CREAT | O_RDWR, S_IRWXU);

          if(fileDescriptor < 0){
               perror("Error creating file t1.txt");
               return;
               }
          
          //close the file
          close(fileDescriptor);
          
          fileDescriptor = open("t2.txt", O_CREAT | O_RDWR, S_IRWXU);

          if(fileDescriptor < 0){
               perror("Error creating file t2.txt");
               return;
               }
          
          close(fileDescriptor);

          fileDescriptor = open("t3.txt", O_CREAT | O_RDWR, S_IRWXU);
          
          if(fileDescriptor < 0){
               perror("Error creating file t3.txt");
               return;
               }
          close(fileDescriptor);

          // check if Directory /Dir1 doesnt already exist
          if(stat("./Dir1", &status) == -1){
               // Create directory /Dir1
               if(mkdir("./Dir1", 0777) == -1){
                    perror("Error while creating directory ./Dir1");
                    return;
               }
          }
     }
}


//--------------------LIST

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
		
                chdir("Dir0");
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
                    chdir("Dir0");
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

return 0;
}


// path

int path(){
    char cwd[PATH_MAX];

   chdir("Dir0");
   // Open two files to be merged
   FILE *fp1 = fopen("tree.txt", "r");
   FILE *fp2 = fopen("path-info.txt", "r");
   FILE *fp4 = fopen("t2.txt", "w");
   // Open file to store the result
   FILE *fp3 = fopen("t3.txt", "w");
   char c;

   // initiates variables to delete files at the end...
   //int del_tree = remove("tree.txt");
   int del_path = remove("path-info.txt");

//--------------------------------------------------------------------------
   // original File_name (t2.txt --> path-info.txt)
   char old_name[] = "t2.txt";
   // new File_name (t2.txt --> path-info.txt)
   char new_name[] = "path-info.txt";
   int value;

   // original File_name (t3.txt --> log.txt)
   char old_name_2[] = "t3.txt";
   // new File_name (t3.txt --> log.txt)
   char new_name_2[] = "log.txt";
   int value_2;

//--------------------------------------------------------------------------

   // Prints current directory. Otherwise, generates error.
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working directory is: %s\n\n", cwd);
       fprintf(fp4, "Current working directory is: %s\n\n", cwd);
   } else {
       perror("getcwd() error");
       return 1;
   }

//--------------------------------------------------------------------------
/* Changing the filename of t2.txt to path-info.txt */

   // File name is changed from t2.txt --> path-info.txt
   value = rename(old_name, new_name);

   // Prints whether or not the File_name was successfully changed
   if(!value) {
       printf("%s", "File name t2.txt to path-info.txt changed successfully. \n\n");
     } else {
       perror("Error: t2.txt does not exist within directory...\n");
     }

//--------------------------------------------------------------------------
/* Merge tree.txt with path-info.txt into t3.txt */

printf("----------FPPPP111111-------------------\n");
    if(fp1){
        while ((c = getc(fp1)) != EOF)
        putchar(c);
    }

 printf("--------------FP22222222---------------\n");

    if(fp2){
        while ((c = getc(fp2)) != EOF)
        putchar(c);
    }
 printf("------------FPPPP33333-----------------\n");
 if(fp3){
        while ((c = getc(fp3)) != EOF)
        putchar(c);
    }

 printf("------------FPPPP444444-----------------\n");
 if(fp4){
        while ((c = getc(fp4)) != EOF)
        putchar(c);
    }

   if (fp1 == NULL || fp2 == NULL) {
     puts("Could not open files");
     exit(0);
   }

   // Copy contents of first file to file3.txt
   while ((c = fgetc(fp1)) != EOF){
     fputc(c, fp3);
   }

    // Copy contents of second file to file3.txt
    while ((c = fgetc(fp2)) != EOF){
      fputc(c, fp3);
    }

    printf("Merged tree.txt and path-info.txt into t3.txt \n\n");

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

//--------------------------------------------------------------------------
/* Changing the filename of t3.txt to log.txt */

   // File name is changed from t3.txt --> log.txt
   value = rename(old_name_2, new_name_2);

   // Prints whether or not the File_name was successfully changed
   if(!value) {
       printf("%s", "File name t3.txt to log.txt changed successfully. \n\n");
     } else {
       perror("Error: t3.txt does not exist within directory...\n");
     }

//--------------------------------------------------------------------------
/* Deletes tree.txt and path-info.txt */

    // Deletes tree.txt
    // if (!del_tree)
    //    printf("tree.txt has been deleted successfully! \n\n");
    // else
    //    printf("tree.txt was NOT deleted successfully \n");

    // Deletes path-info.txt
    if (!del_path)
       printf("path-info.txt has been deleted successfully! \n\n");
    else
       printf("path-info.txt was NOT deleted successfully \n");

    return 0;   
}


//---------------- main
int main(){
    char userInput[10];
    bool flag = true;

//tree list path exit
    while(flag){
        printf("Shell User$>>> ");
        scanf("%s",userInput);
        
        if(strcmp(userInput,"tree*") == 0){
            tree();
        
        } else if(strcmp(userInput,"list*") == 0){
            list();  
        
        } else if(strcmp(userInput,"path*") == 0){
            path();
        
        //} else if(strcmp(userInput,"exit*") == 0){
        //     flag= false;
        
        // } else{
            //printf("WRONG INPUT!!! \nPlease enter one of the following command (tree*, list*, path*, exit*)\n");
        }
    }  

  return 0;

  //  wait(shell);


}