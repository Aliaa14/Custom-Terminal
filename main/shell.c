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


/*--------------------------------------------------------------------------------------------------------*/
int path()
{    
    chdir("./Dir0");
    char s[100];                            // Memory buffer 
    printf("Current Working Directory:\n\t%s\n", getcwd(s, 100));         // This prints the current working directory     
    // So this is essentially how the $ pwd command works in linux ... 
    
    FILE * fp;

    char old_name[] = "t2.txt";              // original file name 
    char new_name[] = "path-info.txt";       // new file name 

    fp = fopen(old_name, "w+");
    fprintf(fp, "This is testing for fprintf ... \n");
    fprintf(fp, "Current Working Directory:\n\t%s\n", getcwd(s, 100));    // This prints the current working directory into the "t2.txt" file 
    fclose(fp);                 // Close file 

    printf("\nCurrent Working Directory path added to file t2.txt\n");

    int value = rename(old_name, new_name);         // rename "t2.txt" to "path-info.txt" [FILE MUST BE CLOSED!] 

    // Standard Exception Handling 
    if(!value) { 
        printf("%s", "-- File name changed successfully from t2.txt to pathinfo.txt!\n"); 
    } else { 
        perror("\nError File name change unsuccess!\n"); 
    } 

/* ---------------------------------[  Thought Process:  ]-------------------------------------
Open 2 files in read mode, store all of its content and paste into a 3rd file 't3.txt', 
after closing the file, rename 't3.txt' into 'log.txt' and then delete the first 2 files.
----------------------------------------------------------------------------------------------- */

    FILE *fs1, *fs2, *logFile;

    char ch, file1[100], file2[100], file3[200];

    fs1 = fopen("tree.txt", "r");     // Open file 1 in read mode 
    fs2 = fopen("path-info.txt", "r");     // Open file 2 in read mode 

    // Standard Exception Handling for tree.txt, and path.txt
    if( fs1 == NULL || fs2 == NULL ) {
		perror("Error ");
		printf("Press any key to exit...\n");
		exit(EXIT_FAILURE);
	}

    logFile = fopen("t3.txt", "w");    // Open log file write mode
    
    // Standard Exception Handling for t3.txt
    if( logFile == NULL ) {
		perror("Error ");
		printf("Press any key to exit...\n");
		exit(EXIT_FAILURE);
	}

    while( ( ch = fgetc(fs1) ) != EOF )
	      fputc(ch,logFile);
	while( ( ch = fgetc(fs2) ) != EOF )
	      fputc(ch,logFile);

    char filename[] = "t3.txt";              // original file name 
    char new_filename[] = "log.txt";         // new file name 

    printf("Two files were sucessfully merged into %s file successfully!\n", filename);

    fclose(fs1);            // close file 1 
	fclose(fs2);            // close file 2 
    fclose(logFile);        // close logFile 

    int result = rename(filename, new_filename);        // rename "t3.txt" to "log.txt" [FILE MUST BE CLOSED!] 

    // Standard Exception Handling 
    if(!result) { 
        printf("%s", " -- File name changed successfully from t3.txt to log.txt !\n"); 
    } else { 
        perror("\nError File name change unsuccess!\n"); 
    } 

    printf("\n");       // Just for formatting purposes


    // Now all that's left is to delete the remaining 2 files! 

    remove("tree.txt");     // delete "tree.txt"
    printf("tree.txt file removed successfully ...\n");
    remove("path-info.txt");     // delete "path.txt"
    printf("path-info.txt file removed successfully ...\n");

    printf("\nAll tasks done!\n\nProgram Execution Complete!\n");

    return 0; 

} 



/*---------------------------------------------------------------------*/

// int path(){
//     char cwd[PATH_MAX];

//    chdir("Dir0");
//    // Open two files to be merged
//    FILE *fp1 = fopen("tree.txt", "r");
//    FILE *fp2 = fopen("path-info.txt", "r");
//    FILE *fp4 = fopen("t2.txt", "w");
//    // Open file to store the result
//    FILE *fp3 = fopen("t3.txt", "w");
//    char c;

//    // initiates variables to delete files at the end...
//    //int del_tree = remove("tree.txt");
//    int del_path = remove("path-info.txt");

// //--------------------------------------------------------------------------
//    // original File_name (t2.txt --> path-info.txt)
//    char old_name[] = "t2.txt";
//    // new File_name (t2.txt --> path-info.txt)
//    char new_name[] = "path-info.txt";
//    int value;

//    // original File_name (t3.txt --> log.txt)
//    char old_name_2[] = "t3.txt";
//    // new File_name (t3.txt --> log.txt)
//    char new_name_2[] = "log.txt";
//    int value_2;

// //--------------------------------------------------------------------------

//    // Prints current directory. Otherwise, generates error.
//    if (getcwd(cwd, sizeof(cwd)) != NULL) {
//        printf("Current working directory is: %s\n\n", cwd);
//        fprintf(fp4, "Current working directory is: %s\n\n", cwd);
//    } else {
//        perror("getcwd() error");
//        return 1;
//    }

// //--------------------------------------------------------------------------
// /* Changing the filename of t2.txt to path-info.txt */

//    // File name is changed from t2.txt --> path-info.txt
//    value = rename(old_name, new_name);

//    // Prints whether or not the File_name was successfully changed
//    if(!value) {
//        printf("%s", "File name t2.txt to path-info.txt changed successfully. \n\n");
//      } else {
//        perror("Error: t2.txt does not exist within directory...\n");
//      }

// //--------------------------------------------------------------------------
// /* Merge tree.txt with path-info.txt into t3.txt */

// printf("----------FPPPP111111-------------------\n");
//     if(fp1){
//         while ((c = getc(fp1)) != EOF)
//         putchar(c);
//     }

//  printf("--------------FP22222222---------------\n");

//     if(fp2){
//         while ((c = getc(fp2)) != EOF)
//         putchar(c);
//     }
//  printf("------------FPPPP33333-----------------\n");
//  if(fp3){
//         while ((c = getc(fp3)) != EOF)
//         putchar(c);
//     }

//  printf("------------FPPPP444444-----------------\n");
//  if(fp4){
//         while ((c = getc(fp4)) != EOF)
//         putchar(c);
//     }

//    if (fp2 == NULL) {
//      puts("Could not open files");
//      exit(0);
//    }

//    // Copy contents of first file to file3.txt
//    while ((c = fgetc(fp1)) != EOF){
//      fputc(c, fp3);
//    }

//     // Copy contents of second file to file3.txt
//     while ((c = fgetc(fp2)) != EOF){
//       fputc(c, fp3);
//     }

//     printf("Merged tree.txt and path-info.txt into t3.txt \n\n");

//     fclose(fp1);
//     fclose(fp2);
//     fclose(fp3);

// //--------------------------------------------------------------------------
// /* Changing the filename of t3.txt to log.txt */

//    // File name is changed from t3.txt --> log.txt
//    value = rename(old_name_2, new_name_2);

//    // Prints whether or not the File_name was successfully changed
//    if(!value) {
//        printf("%s", "File name t3.txt to log.txt changed successfully. \n\n");
//      } else {
//        perror("Error: t3.txt does not exist within directory...\n");
//      }

// //--------------------------------------------------------------------------
// /* Deletes tree.txt and path-info.txt */

//     // Deletes tree.txt
//     // if (!del_tree)
//     //    printf("tree.txt has been deleted successfully! \n\n");
//     // else
//     //    printf("tree.txt was NOT deleted successfully \n");

//     // Deletes path-info.txt
//     if (!del_path)
//        printf("path-info.txt has been deleted successfully! \n\n");
//     else
//        printf("path-info.txt was NOT deleted successfully \n");

//     return 0;   
// }


/*---------------------Declare variables for exit* command---------------------*/
#define COUNT_HISTORY 4
char store_history[COUNT_HISTORY][20];
int current_history = 0;


/*---------------------Function to store history---------------------*/
void append_to_history(char store_history[COUNT_HISTORY][20], char *command, int *count)
{
	if(*count < COUNT_HISTORY)
	{
		strcpy(store_history[*count],command);
		(*count)++;
	}
	else
	{
		for(int i=0;i<COUNT_HISTORY-1;i++){
            strcpy(store_history[*count],store_history[*count+1]);
        }
		strcpy(store_history[COUNT_HISTORY-1],command);
	}
}

/*---------------------Function to display history---------------------*/
void display_history(char store_history[COUNT_HISTORY][20], int count)
{
	if(count == 0)
		printf("\nERROR: *NO HISTORY TO DISPLAY*\n");
	else
	{
		printf("\n---HISTORY---\n\n");
		for(int i=0;i<count;i++)
        {
            printf("%d   %s\n",i, store_history[i]);
        }
			
	}
}


/*---------------------Function to print contents of the current directory---------------------*/
void print_contents_of_current_dir()
{
    printf("---------Contents of the current directory----------\n");
    execlp("ls","ls", "-l", NULL);
}


// bool exit_terminal()
// {
//     printf("\n Press the 'RETURN' key to exit the terminal\n");
//     char input;
//     input = fget(stdin);
//     if(input == "\n")
//     {
//         printf("Exiting...\n");
//     }

//     return false;

// }
/*-------------------------------------------------------------------------------*/

//---------------- main
int main(){
    char userInput[10];
    bool flag = true;

//tree list path exit
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
			
			// change the current working directory to the main folder 
			char s[100]; 
			// printf("%s\n", getcwd(s, 100)); 
			chdir("..");
			
			int PID = fork();
			if(PID < 0)
			{
				printf("Error : Failed to create a child process\n");
				return -1;
			}

			else if (PID == 0)
			{
				print_contents_of_current_dir();   
			}

			else
			{
				wait(NULL);
				printf("\n\nPress 'Return' to exit the terminal\n"); 
				while(1){
					char c = getchar();
					if (strcmp(&c,"\n") == 0)
						break;
						flag = false;
				}
				
			}

        } 
		// else if(strcmp(userInput,"\n") == 0){
		// 	flag = false;
		// 	break;
		// }
		else{
            printf("WRONG INPUT!!! \nPlease enter one of the following command (tree*, list*, path*, exit*)\n");
        }
    }  

  return 0;

  //  wait(shell);


}