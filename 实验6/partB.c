#include <stdio.h>
#include <stdlib.h>
#include "/lib/modules/2.6.35.13/build/arch/x86/include/asm/unistd_32.h"

int sema_open(int key,int number){ 
  return syscall(__NR_sema_open,key,number); 
} 
int sema_close(int flag){  
  return syscall(__NR_sema_close,flag);  
}  
int sema_signal(int num){ 
  return syscall(__NR_sema_signal,num); 
} 
int sema_wait(int num){ 
  return syscall(__NR_sema_wait,num); 
} 
long interrupt_num(){  
  return syscall(__NR_mysyscall1); 
}
int main(int argc, char ** argv) 
{ 
  int i; 
  int pid[5];
  i=sema_open(0,1);
  printf("register %d\n",i);
  int m;
  for(m=0;m<5;m++){
  	if((pid[m]=fork())==0){//вс╫ЬЁл
  		while(1){
  			printf("process %d is waiting \n",getpid());
  			int j= sema_wait(i);
  			//printf("wait over %d\n",j);
  			sleep(2);
  			long intr = interrupt_num();
  			printf("process %d interrupt time:%ld\n",getpid(),intr);
  			int k= sema_signal(i);
  			printf("signal %d\n",i);
  		}
  		//return 0;
  	}else	
		continue;  	
  }
 //int m= sema_close(i);
// printf("wait over %d\n",m);
  return 0 ; 
  
}
