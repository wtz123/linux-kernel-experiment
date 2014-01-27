/***********************************************************
File      : exp3b.c
copyright :(C) 2012 by wangtianzhi
Function  : 内核的定时机制实验 - 问题B
			使用问题A实现的gettimeofday()实现一个精确到微秒级的“壁钟”
************************************************************/
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>

static void sighandle(int);
//static int second = 0;
struct timeval now; 
int today;
int main(){
	struct itimerval v;
	signal(SIGALRM,sighandle);
	v.it_interval.tv_sec = 0;
	v.it_interval.tv_usec = 1;
	v.it_value.tv_sec = 0;
	v.it_value.tv_usec = 1;
	setitimer(ITIMER_REAL, &v, NULL);
	for(;;);
}
static void sighandle(int s){
	//second++;
	gettimeofday(&now, NULL);
	today = now.tv_sec%(3600*24);
	//printf("%d\r",today);
		printf("%02d:%02d:%02d:%ld\r",today/3600,(today%3600)/60,(today%3600)%60,now.tv_usec);
//	printf("%s\r",ctime(&(now.tv_sec)));
    //printf("%li\r",now.tv_sec);
	//printf("%li\r",now.tv_usec);
	//printf("%d\r",second);
	fflush(stdout);
}
