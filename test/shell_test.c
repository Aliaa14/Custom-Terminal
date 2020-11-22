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

/*------------------------------------------------------- Tree Function -------------------------------------------------------*/
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

        if(chdir("Dir0") != 0){      		     // changing directory to Dir0
            perror("chdir() directory change to Dir0 failed");
           return;
        }
          
        // Creating the txt files now as follows inside Dir0, when done close the files
        fileDescriptor = open("t1.txt", O_CREAT | O_RDWR, S_IRWXU);

        if(fileDescriptor < 0){
           perror("Error creating file t1.txt");
           return;
        }
        close(fileDescriptor); //close the file
    
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

        if(stat("./Dir1", &status) == -1){          // check if Directory /Dir1 doesnt already exist

            if(mkdir("./Dir1", 0777) == -1){   	    // Create directory /Dir1
            	perror("Error while creating directory ./Dir1");
                return;
            }
        }
    }
}

/*------------------------------------------------------- List Function -------------------------------------------------------*/
int list(){
	pid_t child_reset = fork();

	if(child_reset < 0){				       	  // error check for fork
		perror("ERROR: list*: child_reset: fork: ");
		return 1;
	
	} else if(child_reset == 0){			      // child_reset code
		
		char *args[] = {"reset", NULL};		      // clear terminal screen
		execvp("reset", args);	
			
	} else{									      // parent code
		wait(NULL);  							  // wait for child_reset to terminate		
		pid_t child_list = fork();

		if(child_list < 0){			  		      // error check for fork
			perror("ERROR: list*: child_list: fork: ");
			return 1;

		} else if(child_list == 0){			      // child_list code
			// print a detailed list of all the content in current directory
			char *args[] = {"ls", "-l", "-a", "-h", NULL};
			execvp("ls", args);	

		} else{								      // parent code
			wait(NULL);			   	    	      // wait for child_list to terminate			
			pid_t child_text = fork();
		
			if(child_text < 0){					 // error check in forking
				perror("list*: child_text: ");
				return -1;

			} else if(child_text == 0) {			// child_text code
                chdir("Dir0");
				int fd = open("t1.txt", O_CREAT | O_RDWR, S_IRWXU);    // open file

				if (fd < 0) {							// error check for open
					perror("ERROR: list*: fd: open: ");
					exit(1);				
				}	
				dup2(fd, 1);			    			// make stdout go to file		
				close(fd);				
				
				// print a detailed list of all the content in currect directory to file
				char *args[] = {"ls", "-l", "-a", "-h", NULL};
				execvp("ls", args);			
	
			} else{					// parent code
				wait(NULL); 					// wait for child_text to terminate				
				pid_t child_rename = fork();
				
				if(child_rename < 0){					// error check for fork
					perror("ERROR: list*: child_rename: fork: ");
					return 1;

				} else if (child_rename == 0){			// child_remain code
                    chdir("Dir0");
                   char *args[] = {"mv", "t1.txt", "tree.txt", NULL}; 	// change name of file
					execvp("mv", args);	
		
				} else {								// parent code

					wait(NULL);		 					// wait for child_rename to terminate
					printf("-------------------- All contents are listed--------------------\n");
				}
			}
		}
	}
return 0;
}

/*------------------------------------------------------- Path Function -------------------------------------------------------*/
int path(){    
    chdir("./Dir0");
    char s[100];                           								   // buffer for memory  
    printf("Current Directory:%s ", getcwd(s, 100));        		   // print the current directory         
    FILE * fp;

    char fileOldName[] = "t2.txt";            					 
    char fileNewName[] = "path-info.txt";       

    fp = fopen(fileOldName, "w+");										   //open t2.txt file
    fprintf(fp, " >> Current Working Directory:\n\t%s\n", getcwd(s, 100));     // copy the current directory to t2.txt
    fclose(fp);                											   // Close file 
    printf("\n >> Current directory path successfully added to t2.txt\n");
    int value = rename(fileOldName, fileNewName);         				   // rename t2.txt to path-info.txt

    if(!value) { 														  
        printf("File2 name (t2.txt) changed successfully path-info.txt\n"); 
    } else{ 
        perror("\nERROR!! Couldn't change file name!\n"); 
    } 

    FILE *fp1, *fp2, *logFile;
    char c, f1[100], f2[100], f3[200];

    fp1 = fopen("tree.txt", "r");     							    	  // Open file 1 in read mode 
    fp2 = fopen("path-info.txt", "r");   								  // Open file 2 in read mode 

    if( fp1 == NULL || fp2 == NULL ){   				    			  //check if files are empty
		perror("Error ");
		printf("Press any key to exit\n");
		exit(0);
	}
    logFile = fopen("t3.txt", "w");   								      // Open log file in write mode
    
    if( logFile == NULL ) {												  //check if log file is empty
		perror("Error ");
		printf("Press any key to exit\n");
		exit(0);
	}

    while((c = fgetc(fp1)) != EOF)								//read through file1 and copy its contents to log file
	    fputc(c,logFile);
	while(( c = fgetc(fp2)) != EOF)							    //read through file2 and copy its contents to log file
	    fputc(c,logFile);

    char filename[] = "t3.txt";              
    char new_filename[] = "log.txt";   
	    
    printf("Files merged sucessfully to t3.txt\n");
    fclose(fp1);            
	fclose(fp2);            
    fclose(logFile);        				 

    int result = rename(filename, new_filename);                 //rename t3.txt to log.txt

    if(!result) { 
        printf("File3 name changed successfully to log.txt\n"); 
    } else { 
        perror("\nERROR!! Couldn't change file name!\n"); 
    } 

    remove("tree.txt");    										 // delete "tree.txt"
    printf("successfully removed tree.txt\n");
    remove("path-info.txt");   									  // delete "path.txt"
    printf("successfully removed  path-info.txt\n");
    printf("-------------------- All tasks are done!--------------------\n");
    return 0; 
} 

/*------------------------------------------------------- Exit Function  -------------------------------------------------------*/
#define COUNT_HISTORY 4
char store_history[COUNT_HISTORY][20];
int current_history = 0;

//This function stores history
void append_to_history(char store_history[COUNT_HISTORY][20], char *command, int *count){
	if(*count < COUNT_HISTORY){
		strcpy(store_history[*count],command);
		(*count)++;
	
	} else{
		for(int i=0;i<COUNT_HISTORY-1;i++){
            strcpy(store_history[*count],store_history[*count+1]);
        }
		strcpy(store_history[COUNT_HISTORY-1],command);
	}
}

//This function displays history
void display_history(char store_history[COUNT_HISTORY][20], int count){
	if(count == 0)
		printf("\nERROR: NO HISTORY TO DISPLAY\n");
	
	else{
		printf("\n------------------------- HISTORY ---------------------------\n\n");
		for(int i=0;i<count;i++){
            printf("%d   %s\n",i, store_history[i]);
        }	
	}
}

//This function prints contents of the current directory
void print_contents_of_current_dir(){
    printf("---------Contents of the current directory----------\n");
    execlp("ls","ls", "-l", NULL);
}

/*------------------------------------------------------- Main Function -------------------------------------------------------*/
int main(){
    char userInput[10];
    bool flag = true;

    while(flag){
        printf("Terminal > ");
        scanf("%s",userInput);
        
        if(strcmp(userInput,"tree*") == 0){
            tree();
			append_to_history(store_history,userInput,&current_history);
        
        } else if(strcmp(userInput,"list*") == 0){
            list();  
			append_to_history(store_history,userInput,&current_history);
        
        } else if(strcmp(userInput,"path*") == 0){
            path();
			append_to_history(store_history,userInput,&current_history);

        } else if(strcmp(userInput,"exit*") == 0){
			append_to_history(store_history,userInput,&current_history);
			display_history(store_history,current_history);
			
			char s[100]; 			// change the current working directory to the main folder 
			chdir("..");			// printf("%s\n", getcwd(s, 100)); 

			int PID = fork();
			if(PID < 0){
				printf("Error : Failed to create a child process\n");
				return -1;
			
			} else if(PID == 0){
				print_contents_of_current_dir();   
			
			} else{
				wait(NULL);
				printf("\n\nPress 'Return' to exit the terminal\n"); 
				while(1){
					char c = getchar();
					if (strcmp(&c,"\n") == 0 || strcmp(&c,"\r") == 0)
						break;
						flag = false;
				}
			}
        
		} else{
            printf("WRONG INPUT!!! \nPlease enter one of the following command (tree*, list*, path*, exit*)\n");
        }
    }  
  return 0;
}