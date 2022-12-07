#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
   if (argc != 2) {
      fprintf(stderr, "usage: vcpu <string>\n");
      exit(1);
   }

   char *str = argv[1];
   while (1) {
      sleep(1);
      printf("%s\n", str);
   }

   return 0;
}