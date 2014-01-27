/****************************************************************
File:exp3d.c
Copyright : (C)2012 by wangtianzhi
Function  : 编写一个定时报告中断次数的程序，它可以连续报告三种定时
			类型每1.25毫秒的时间间隔内，系统发生了多少次中断。
  ****************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

//中断处理函数
static void sig_real(void);
static void sig_virtual(void);
static void sig_prof(void);

unsigned long real_stime,virtual_stime,prof_stime;//开始时中断数
unsigned long real_otime,virtual_otime,prof_otime;//结束时中断数

//记录3种定时的毫秒秒数的结构变量
static struct itimerval realt,virtt,proft;
FILE *thisProcFile1,*thisProcFile2,*thisProcFile3;
char str_real[502],str_virtual[502],str_prof[502];
char sreal[20],svirtual[20],sprof[20];
int main(){
	thisProcFile1 = fopen("/proc/stat","r");
	while(fgets(str_real,502,thisProcFile1)!=NULL){
		sscanf(str_real,"%s%ld",sreal,&real_stime);
		if(strcmp(sreal,"intr")==0){
			close(thisProcFile1);
		//	rewind(thisProcFile1);//将内部文件指针移到文件开头
			break;
		}
	}
//	close(thisProcFile);
	virtual_stime=real_stime;
	prof_stime=real_stime;
	//设置3种定时处理入口 		  
	signal(SIGALRM,sig_real); 			 
	signal(SIGVTALRM,sig_virtual); 				
    signal(SIGPROF,sig_prof);

	//初始化3种时间定时器 				   
	realt.it_interval.tv_sec = 0; 					  
	realt.it_interval.tv_usec = 1250; 						 
    realt.it_value.tv_sec = 0; 							
	realt.it_value.tv_usec = 1250;

	virtt.it_interval.tv_sec = 0; 					  
	virtt.it_interval.tv_usec = 1250; 						 
	virtt.it_value.tv_sec = 0; 							
    virtt.it_value.tv_usec = 1250;

	proft.it_interval.tv_sec = 0; 					  
	proft.it_interval.tv_usec = 1250; 						 
	proft.it_value.tv_sec = 0; 							
    proft.it_value.tv_usec = 1250;

	setitimer(ITIMER_REAL,&realt,NULL);
	setitimer(ITIMER_VIRTUAL,&virtt,NULL);
	setitimer(ITIMER_PROF,&proft,NULL);
	for(;;);
	return 0;
}

static void sig_real(){
	thisProcFile1 = fopen("/proc/stat","r");
	while(fgets(str_real,502,thisProcFile1)!=NULL){
		sscanf(str_real,"%s%ld",sreal,&real_otime);
		if(strcmp(sreal,"intr")==0){
	        close(thisProcFile1);
	        //rewind(thisProcFile1);
			break;
		}
	}
//	printf("real_otime%ld\n",real_otime);
	printf("Real interrupt time:%ld\n",real_otime-real_stime);
	real_stime = real_otime;

}
static void sig_virtual(){
	thisProcFile2 = fopen("/proc/stat","r");
	while(fgets(str_virtual,502,thisProcFile2)!=NULL){
		sscanf(str_virtual,"%s%ld",svirtual,&virtual_otime);
		if(strcmp(svirtual,"intr")==0){
	       // rewind(thisProcFile2);
			close(thisProcFile2);
			break;
		}
	}
//	close(thisProcFile);
//	printf("virtual_otime:%ld\n",virtual_otime);
	printf("Virtual interrupt time:%ld\n",virtual_otime-virtual_stime);
	virtual_stime = virtual_otime;
}
static void sig_prof(){
	thisProcFile3 = fopen("/proc/stat","r");
	while(fgets(str_prof,502,thisProcFile3)!=NULL){
		sscanf(str_prof,"%s%ld",sprof,&prof_otime);
		if(strcmp(sprof,"intr")==0){
	      	close(thisProcFile3);
			// rewind(thisProcFile3);
			break;
		}
	}
//	close(thisProcFile);
//	printf("prof_otime:%ld\n",prof_otime);
	printf("Proc interrupt time:%ld\n",prof_otime - prof_stime);
	prof_stime = prof_otime;
}
