 #include <stdio.h>  
#include <stdlib.h>  
#include "/lib/modules/2.6.35.13/build/arch/x86/include/asm/unistd_32.h"
int sema_close(int flag){  
  return syscall(__NR_sema_close,flag);  
}  
int main(int argc, char ** argv)  
{  
  int i;  
  if(argc != 2)  
    return -1;  
  i = sema_close(atoi(argv[1]));     
  if(i==1)
  	printf("Unregisted event %d\n",atoi(argv[1]));
  else
  	printf("Unregisted error.");  
  return 0 ;  
} 
