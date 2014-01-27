#include <stdio.h>
#include <stdlib.h>
#include "/lib/modules/2.6.35.13/build/arch/x86/include/asm/unistd_32.h"

int sema_signal(int num){ 
  return syscall(__NR_sema_signal,num); 
} 
 
int main(int argc, char ** argv) 
{ 
  int i; 
  if(argc != 2) 
  { 
     printf("Usage: signal 2 | 4 | 6 | 2i ...\n"); 
    return -1; 
  } 
 
  i = sema_signal(atoi(argv[1])); 
  if(i) 
    printf("Wakeup event %d .\n",atoi(argv[1])); 
  else 
    printf("No event %d !\n",atoi(argv[1])); 
  return 0 ; 
} 
