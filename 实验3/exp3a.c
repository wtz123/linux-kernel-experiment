/***********************************************************
copyright :(C) 2012 by wangtianzhi
Function  : 内核的定时机制实验 - 问题A
			使用ITIMER_REAL型定时器实现一个gettimeofday(),将它设置为每秒产生一个信号，并计算已经经过的秒数。
************************************************************/
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>

static void sighandle(int);
static int second = 0;

int main(){
	struct itimerval v;
	signal(SIGALRM,sighandle);
	v.it_interval.tv_sec = 1;
	v.it_interval.tv_usec = 0;
	v.it_value.tv_sec = 1;
	v.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &v, NULL);
	for(;;);
}
static void sighandle(int s){
	second++;
	printf("%d\r",second);
	fflush(stdout);
}
