/*******************************************************
File :exp4a.c
Copyright :(C)2012 by wangtianzhi
Function  : 分析实验以上模块，编写一个测试该模块的用户程序。比较该模块读取的时间和用gettimeofday()读取的时间的精度。

********************************************************/
#include <stdio.h>
#include <sys/time.h>

#define HZ 250 
FILE *thisProcFile;
struct timeval now; 
char str[20],c1,c2,c3,c4,c5,c6;
unsigned long jiffies;
int main(){
	gettimeofday(&now, NULL);
	thisProcFile = fopen("/proc/mydir/myfile","r");
	fgets(str,20,thisProcFile);
	sscanf(str,"%c%c%c%c%c%c%ld",&c1,&c2,&c3,&c4,&c5,&c6,&jiffies);
	printf("gettimeofday=%ld\n",now.tv_sec);
//	float stime = ((float)jiffies)/HZ;
//	printf("%f\n",stime);
	printf("jiffies=%ld\n",jiffies);
	close(thisProcFile);
	return 0;
}
