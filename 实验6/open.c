#include <stdio.h>
#include <stdlib.h>
#include "/lib/modules/2.6.35.13/build/arch/x86/include/asm/unistd_32.h"

int sema_open(int key,int number){ 
  return syscall(__NR_sema_open,key,number); 
} 
 
int main(int argc, char ** argv) 
{ 
  int i; 
  if(argc != 3) 
  { 
     printf("Usage: open 2|4|6|2i semaNum \n"); 
    return -1; 
  } 
  i = sema_open(atoi(argv[1]),atoi(argv[2])); 
  if(i) 
          printf("Registed event %d .\n",i); 
  else 
    printf("Registed event fail !\n"); 
  return 0 ; 
} 
