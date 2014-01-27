#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
main(int argc,char *argv[])
{
	int fd ;
	char num;
	int jieshou;
	jieshou=atoi(argv[1]);
	//int ptr_write=0;
	//int ptr_read;
	if(jieshou==1)
	{
	        fd= open("/dev/scullpipe",O_RDWR);
	        int jishu=0;
		while(1)//send data
		{
			//printf("1\n"); 
			if (fd != -1 )
			{
			        if(read(fd, &num, sizeof(char)))
			          printf("The scull is %c\n", num);
			        /**int i=0;
			        for(i=0;i<=10;i++)
			        {
			        //sleep(0);
			        //printf("%d\n",i);
			        }	*/		
			}
			else
			{
				printf("Device open failure\n");
			}
		}
		close(fd);
	}
	else 
	{
	        fd = open("/dev/scullpipe",O_RDWR|O_APPEND);
	        int jishu=0;
	        while(1)//receive data
	        {                
			if (fd != -1 )
			{
			        //read(fd, &num, sizeof(char));
				//printf("The scull is %d\n", num);
				//printf("Please input the num written to scull\n");
				scanf("%c", &num);
				write(fd, &num, sizeof(char));
        			//num=0;
		 		//read(fd, &num, sizeof(char));
				//printf("The scull is %d\n", num);				
			}
			else
			{
				printf("Device open failure\n");
			}
	        }
	        close(fd);	
	}
}
