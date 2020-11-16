/* Part 5 - exit.c program*/

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdlib.h>

/* -------FUNCTION PROTOTYPES-------*/

bool exit_terminal();

/* --------MAIN FUNCTION-----------*/

int main(int argc, char *argv[])
{
    chdir("./FinalProject");
    int size = 0;

    for(int i=0; i<4; i++)
    {
        if(!argv[i])
            break;
        size++;
    }

    printf("-----------HISTORY------------\n");
    for(int i=size; i>0; i--)
    {
        printf("%s\n", argv[i-1]);
    }


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
        exit_terminal();
    }
}


/*-------HELPER FUNCTIONS--------*/


//print contents of current directory
void print_contents_of_current_dir()
{
    printf("---------Contents of the current directory----------\n");
    execlp("ls","ls", "-l", NULL);
}


//Exit Terminal
bool exit_terminal()
{
    printf("\n Press the 'RETURN' key to exit the terminal\n");
    char input;
    input = fget(stdin);
    if(input == "\n")
    {
        printf("Exiting...\n");
    }

    return false;

}