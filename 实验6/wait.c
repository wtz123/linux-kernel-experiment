#include <stdio.h>
#include <stdlib.h>
#include "/lib/modules/2.6.35.13/build/arch/x86/include/asm/unistd_32.h"

int sema_wait(int num){ 
  return syscall(__NR_sema_wait,num); 
} 

int main(int argc, char ** argv) 
{ 
  int i; 
  if(argc != 2) 
  { 
     printf("Usage: wait 2 | 4 | 6 | 2i ...\n"); 
    return -1; 
  } 
  printf("Event %d will sleeping !\n",atoi(argv[1])); 
  i = sema_wait(atoi(argv[1])); 
  printf("Event %d wakeup !\n",i); 
  return 0 ; 
  
}
