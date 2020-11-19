#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h> 
#include<fcntl.h> 
#include<stdlib.h> 

void tree(){
     struct stat status = {0};
     int fileDescriptor;

     // check if Directory exists
     if (stat("./Dir0", &status) == -1){
          //create directory Dir0, 0777 default mode with widest possible access
          if(mkdir("./Dir0", 0777) == -1){
               perror("Error while creating directory");
               return;
          }
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

     // check if Directory exists
     if(stat("./Dir1", &status) == -1){
          // Create directory Dir1
          if(mkdir("./Dir1", 0777) == -1){
               perror("Error while creating directory ./Dir1");
               return;
          }
     } 
}

int main(){
     tree();
} 