#include <linux/unistd.h>
#include <stdio.h>
#include <stdlib.h>
int buffer[2];
int ptr_buffer; 
 
int main(int argc, char ** argv)
{
    int i,ii,j,value=0;
    if(argc != 2)
    {
      printf("wrong args\n ");
      return -1;
    }
    i = (atoi(argv[1]));
    int jieshou=syscall(340,i,2);// the number of semaphore
    printf("the semaphore you declare is NO.%d\n",jieshou);
    printf ("the number of semaphore of all is %d\n",syscall(344));
    //if(i==1)//producer 
    {
      int j,xunhuan;
      for(j=0;j<=10;j++)
      {
      //printf("come here\n");
      if(jieshou!=2)
      return -1;
      int j=syscall(341,jieshou);//
      printf(" the process %d is in the buffer\n",getpid());
    
      for(xunhuan=0;xunhuan<10;xunhuan++)
        {
        sleep(1);
        printf("%d\n",xunhuan);
        }
          
        //sleep(10);
      if(jieshou!=2)
      return -1;
      //printf("come into signal\n");
      j=syscall(342,jieshou);
      }      
    }
    
    
    printf("%d\n",i);
    return 0 ;
}
