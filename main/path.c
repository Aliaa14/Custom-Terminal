/*
path*: this path command will print the path of the current directory to the terminal and t2.txt, and change the name of text file to path-info.txt. Concatenate the content of tree.txt and path-info.txt into t3.txt and change the last to log.txt. Finally, delete tree and path text files
*/


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
   char cwd[PATH_MAX];dsd

   // Open two files to be merged
   FILE *fp1 = fopen("tree.txt", "r");
   FILE *fp2 = fopen("path-info.txt", "r");
   FILE *fp4 = fopen("t2.txt", "w");
   // Open file to store the result
   FILE *fp3 = fopen("t3.txt", "w");
   char c;

   // initiates variables to delete files at the end...
   int del_tree = remove("tree.txt");
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

   if (fp1 == NULL || fp2 == NULL || fp3 == NULL) {
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
    if (!del_tree)
       printf("tree.txt has been deleted successfully! \n\n");
    else
       printf("tree.txt was NOT deleted successfully \n");

    // Deletes path-info.txt
    if (!del_path)
       printf("path-info.txt has been deleted successfully! \n\n");
    else
       printf("path-info.txt was NOT deleted successfully \n");

   return 0;
}
