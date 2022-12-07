#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

int main(void)
{
   char commandline[100], **argv;
   int i, j, k, n, p, output_path_valid, input_path_valid, fd;
   char output_path[100], input_path[100];
   while (1) {
      output_path_valid=0;
      input_path_valid=0;
      printf("> ");     // Prompt for a command
      j=0;
      i=0;
      k=0;
      gets(commandline);     // Read command line
      if (!strcmp(commandline, "exit")) break;

      // Calculate the number of command line arguments
      while (commandline[i] != '\0') {
         while(commandline[i] == ' ') i++;
         i++;
         if (commandline[i] == ' ') { j++; i++; }
      }

      // Allocate argument list array (one plus the needed number)
      argv = (char**)malloc((j+2)*sizeof(char*));
      for (n=0; n<j+1; n++) {
         argv[n] = (char*)malloc(20*sizeof(char));
      }

      // Copy command line to argument list
      j=0;
      i=0;
      k=0;
      while (commandline[i] != '\0') {
         while(commandline[i] == ' ') i++;
         if ((commandline[i] != '>') && (commandline[i] != '<')) {
            argv[j][k] = commandline[i];
            k++;
            i++;
            if (commandline[i] == ' ') {
               argv[j][k] = '\0';
               j++;
               k=0;
               i++;
            }
         }
         else if (commandline[i] == '>') {
            i++;
            while(commandline[i] == ' ') i++;
            p=0;
            output_path_valid = 1;
            while ((commandline[i] != '\0') && (commandline[i] != ' ')) {
               output_path[p] = commandline[i];
               p++;
               i++;
            }
            output_path[p] = '\0';
         }
         else if (commandline[i] == '<') {
            i++;
            while(commandline[i] == ' ') i++;
            p=0;
            input_path_valid = 1;
            while ((commandline[i] != '\0') && (commandline[i] != ' ')) {
               input_path[p] = commandline[i];
               p++;
               i++;
            }
            input_path[p] = '\0';
         }
      }
      if (k == 0) argv[j] = NULL;   // The last argument is null
      else {
         argv[j][k] = '\0';
         argv[j+1] = NULL;   // The last argument is null
      }

      if (access(argv[0], F_OK) != 0) {
         printf("%s: command not found.\n", argv[0]);
         continue;
      }

      // Fork child to execute command
      if (fork()==0) {
         if (output_path_valid) {
            close(STDOUT_FILENO);
            fd = open(output_path, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR);
            assert(fd==1);
         }
         if (input_path_valid) {
            close(STDIN_FILENO);
            fd = open(input_path, O_RDONLY);
            assert(fd==0);
         }
         execv(argv[0],argv);
      }
      else wait(NULL);
   }
   return 0;
}