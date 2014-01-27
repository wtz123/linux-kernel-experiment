/****************************************************************
  						mytimer.c  -  description 
                          ------------------
copyright            : (C) 2012 by wangtianzhi 
Function             : 测试并发进程执行中的各种时间。
                       给定3个菲波纳奇项数值可选在36-45之间
*****************************************************************/
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <signal.h> 
#include <sys/time.h> 

static void psig_real(void);//父进程的3个定时中断处理函数原型 
static void psig_virtual(void); 
static void psig_prof(void); 

static void c1sig_real(void);//子进程 1的 3个定时中断处理函数原型 
static void c1sig_virtual(void); 
static void c1sig_prof(void); 

static void c2sig_real(void);//子进程 2的 3个定时中断处理函数原型 
static void c2sig_virtual(void); 
static void c2sig_prof(void); 

long unsigned int fibonacci(unsigned int n); 

//记录3种定时的秒数的变量 
static long p_real_secs=0,c1_real_secs=0,c2_real_secs=0; 
static long p_virtual_secs=0,c1_virtual_secs=0,c2_virtual_secs=0; 
static long p_prof_secs=0,c1_prof_secs=0,c2_prof_secs=0; 

//记录3种定时的毫秒秒数的结构变量 
static struct itimerval p_realt,c1_realt,c2_realt; 
static struct itimerval p_virtt,c1_virtt,c2_virtt; 
static struct itimerval p_proft,c1_proft,c2_proft; 

int main(int argc,char **argv) 
{ 
   long unsigned fib=0; 
   int pid1,pid2; 
   unsigned int fibarg;
   int status; 
   int i; 
   if(argc < 4) { 		  
	   printf("Usage: testsig arg1 arg2 arg3\n"); 		   
	   return 1; 				
   } 		  
  
   //父进程设置3种定时处理入口 		  
   signal(SIGALRM,psig_real); 			 
   signal(SIGVTALRM,psig_virtual); 				
   signal(SIGPROF,psig_prof); 
				   				   
   //初始化父进程3种时间定时器 				   
   p_realt.it_interval.tv_sec = 9; 					  
   p_realt.it_interval.tv_usec = 999999; 						 
   p_realt.it_value.tv_sec = 9; 							
   p_realt.it_value.tv_usec = 999999; 
   setitimer(ITIMER_REAL,&p_realt,NULL); 								   
   
   p_virtt.it_interval.tv_sec = 9; 					  
   p_virtt.it_interval.tv_usec = 999999; 						 
   p_virtt.it_value.tv_sec = 9; 							
   p_virtt.it_value.tv_usec = 999999; 
   setitimer(ITIMER_VIRTUAL,&p_virtt,NULL);
   
   p_proft.it_interval.tv_sec = 9; 					  
   p_proft.it_interval.tv_usec = 999999; 						 
   p_proft.it_value.tv_sec = 9; 							
   p_proft.it_value.tv_usec = 999999; 
   setitimer(ITIMER_PROF,&p_proft,NULL);
   
   pid1 = fork(); 
   if(pid1==0){
	   //子进程1设置3种定时处理入口 
	   signal(SIGALRM,c1sig_real); 			 
	   signal(SIGVTALRM,c1sig_virtual); 				
       signal(SIGPROF,c1sig_prof); 
				   				   
       //初始化子进程1的3种时间定时器 				   
       c1_realt.it_interval.tv_sec = 9; 					  
       c1_realt.it_interval.tv_usec = 999999; 						 
       c1_realt.it_value.tv_sec = 9; 							
       c1_realt.it_value.tv_usec = 999999; 
       setitimer(ITIMER_REAL,&c1_realt,NULL); 								   
       c1_virtt.it_interval.tv_sec = 9; 					  
       c1_virtt.it_interval.tv_usec = 999999; 						 
       c1_virtt.it_value.tv_sec = 9; 							
       c1_virtt.it_value.tv_usec = 999999; 
       setitimer(ITIMER_VIRTUAL,&c1_virtt,NULL);
   
       c1_proft.it_interval.tv_sec = 9; 					  
       c1_proft.it_interval.tv_usec = 999999; 						 
       c1_proft.it_value.tv_sec = 9; 							
       c1_proft.it_value.tv_usec = 999999; 
       setitimer(ITIMER_PROF,&c1_proft,NULL);  
										
       //子进程1开始计算 fib 									
	   fib = fibonacci(atoi(argv[1]));	
	   //打印子进程1所花费的 3种时间值
	   getitimer(ITIMER_REAL,&c1_realt);
	   printf("Child1 fib=%ld\nChild1 Real Time=%ldSec:%ldusec\n",fib,c1_real_secs+ 10 -c1_realt.it_value.tv_sec,1000000 - c1_realt.it_value.tv_usec);
	   getitimer(ITIMER_VIRTUAL,&c1_virtt);
	   printf("Child1 Virtual Time=%ldSec:%ldusec\n",c1_virtual_secs+ 10 -c1_virtt.it_value.tv_sec,1000000 - c1_virtt.it_value.tv_usec);
	   getitimer(ITIMER_PROF,&c1_proft);
	   printf("Child1 Prof Time=%ldSec:%ldusec\n",c1_prof_secs+ 10 -c1_proft.it_value.tv_sec,1000000 - c1_proft.it_value.tv_usec);
   }
   else if((pid2=fork()) == 0){ 
	   //子进程2设置3种定时中段入口 
   	   signal(SIGALRM,c2sig_real); 			 
	   signal(SIGVTALRM,c2sig_virtual); 				
       signal(SIGPROF,c2sig_prof); 
				   				   
       //初始化子进程2的3种时间定时器 				   
       c2_realt.it_interval.tv_sec = 9; 					  
       c2_realt.it_interval.tv_usec = 999999; 						 
       c2_realt.it_value.tv_sec = 9; 							
       c2_realt.it_value.tv_usec = 999999; 
       setitimer(ITIMER_REAL,&c2_realt,NULL); 								   
       c2_virtt.it_interval.tv_sec = 9; 					  
       c2_virtt.it_interval.tv_usec = 999999; 						 
       c2_virtt.it_value.tv_sec = 9; 							
       c2_virtt.it_value.tv_usec = 999999; 
       setitimer(ITIMER_VIRTUAL,&c2_virtt,NULL);
   
       c2_proft.it_interval.tv_sec = 9; 					  
       c2_proft.it_interval.tv_usec = 999999; 						 
       c2_proft.it_value.tv_sec = 9; 							
       c2_proft.it_value.tv_usec = 999999; 
       setitimer(ITIMER_PROF,&c2_proft,NULL);  
										
       //子进程2开始计算 fib 									
	   fib = fibonacci(atoi(argv[2]));	
	   //打印子进程2所花费的 3种时间值
	   getitimer(ITIMER_REAL,&c2_realt);
	   printf("Child2 fib=%ld\nChild2 Real Time=%ldSec:%ldusec\n",fib,c2_real_secs+ 10 -c2_realt.it_value.tv_sec,1000000 - c2_realt.it_value.tv_usec);
	   getitimer(ITIMER_VIRTUAL,&c2_virtt);
	   printf("Child2 Virtual Time=%ldSec:%ldusec\n",c2_virtual_secs+ 10 -c2_virtt.it_value.tv_sec,1000000 - c2_virtt.it_value.tv_usec);
	   getitimer(ITIMER_PROF,&c2_proft);
	   printf("Child2 Prof Time=%ldSec:%ldusec\n",c2_prof_secs+ 10 -c2_proft.it_value.tv_sec,1000000 - c2_proft.it_value.tv_usec);
   } 
   else  { 
	   //父进程开始计算fib
       fib = fibonacci(atoi(argv[3]));	
	   //打印父进程所花费的 3种时间值
	   getitimer(ITIMER_REAL,&p_realt);
	   printf("Parent fib=%ld\nParent Real Time=%ldSec:%ldusec\n",fib,p_real_secs+ 10 -p_realt.it_value.tv_sec,1000000 - p_realt.it_value.tv_usec);
	   getitimer(ITIMER_VIRTUAL,&p_virtt);
	   printf("Parent Virtual Time=%ldSec:%ldusec\n",p_virtual_secs+ 10 -p_virtt.it_value.tv_sec,1000000 - p_virtt.it_value.tv_usec);
	   getitimer(ITIMER_PROF,&p_proft);
	   printf("Parent Prof Time=%ldSec:%ldusec\n",p_prof_secs+ 10 -p_proft.it_value.tv_sec,1000000 - p_proft.it_value.tv_usec);

       //等待子进程结束 
	   //第一个参数：
	   //         	pid<-1 等待进程组识别码为 pid 绝对值的任何子进程。
	   //		　　pid=-1 等待任何子进程,相当于 wait()。
	   //		　　pid=0 等待进程组识别码与目前进程相同的任何子进程。
	   //		　　pid>0 等待任何子进程识别码为 pid 的子进程。
       //第二个参数存放子进程的状态结束符
	   waitpid(0,&status,0); 
	   waitpid(0,&status,0); 
   } 
} 

//父进程的3个定时中断处理函数 
static void psig_real(){ 
   p_real_secs += 10; 
   //p_realt.it_interval.tv_sec = 9; 
   //p_realt.it_interval.tv_usec = 999999; 
   //p_realt.it_value.tv_sec = 9; 
   //p_realt.it_value.tv_usec = 999999; 
   //setitimer(ITIMER_REAL,&p_realt,NULL); 
} 
static void psig_virtual(){
	p_virtual_secs += 10;
} 
static void psig_prof(){
	p_prof_secs += 10;
} 

//子进程1的3个定时中断处理函数
static void c1sig_real(){
	c1_real_secs += 10;
}
static void c1sig_virtual(){
	c1_virtual_secs += 10;
} 
static void c1sig_prof(){
	c1_prof_secs += 10;
} 

//子进程2的3个定时中断处理函数原型
static void c2sig_real(){
	c2_real_secs += 10;
}
static void c2sig_virtual(){
	c2_virtual_secs += 10;
} 
static void c2sig_prof(){
	c2_prof_secs += 10;
}

//fib的递归计算函数 
long unsigned int fibonacci(unsigned int n) 
{  
	if(n == 0)return 0;
	else if(n == 1 || n == 2)return 1;
	else return fibonacci(n-1)+fibonacci(n-2);
} 

 
/************************END*****************************************/
