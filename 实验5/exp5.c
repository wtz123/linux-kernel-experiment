#include <stdio.h> 
#include <errno.h> 
#include <sys/time.h> 
#include <linux/unistd.h> //不同内核版本可能位置不同 
//系统调用入口汇编软中断宏， 
//2.6.X系统之前定义的方法 
//#define __NR_mysyscall 338
//_syscall2(long,mysyscall,struct timeval*,v_time,struct timespec*,s_time)
 
//2.6.X系统之后定义的方法 
//long my_syscall(struct timeval *v_time,struct timespec *s_time){  
//  return syscall(__NR_mysyscall,v_time,s_time);  
//}  

struct timeval v_time,now;
struct timespec s_time;
long start,end;
int main() 
{ 
   start=syscall(339);
   // mysyscall(&v_time,&s_time);
   syscall(338,&v_time,&s_time);
   gettimeofday(&now,NULL);
    printf("timeval.tv_sec=%ld\ntimeval.tv_usec=%ld\n",v_time.tv_sec,v_time.tv_usec);
    printf("timespec.tv_sec=%ld\ntimespec.tv_nsec=%ld\n",s_time.tv_sec,s_time.tv_nsec);
    printf("使用gettimeofday()得到的结果：tv_sec=%ld,tv_usec=%ld\n",now.tv_sec,now.tv_usec);
    int i;
    for(i=0;i<10000;i++);
    end=syscall(339);
    printf("缺页中断次数：strat=%ld,end=%ld,total=%d\n",start,end,end-start);
} 
