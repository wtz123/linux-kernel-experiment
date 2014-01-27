/***********************************************************************
    main.c  -  description 
    begin                :4月/10日/2012年  
    copyright            : (C) 2012 by wangtianzhi 
    Function             : 观察linux内核行为 
***************************************************************************/
#include <stdio.h> 
#include <sys/time.h> 
#define LB_SIZE 80 
enum TYPE{STANDARD,SHORT,LONG};
FILE *thisProcFile; 
  //Proc打开文件指针 
struct timeval now; 
  //系统时间日期 
enum TYPE reportType; 
 //观察报告类型 
char repTypeName[16]; 
char *lineBuf;    //proc文件读出行缓冲 
int interval;     //系统负荷监测时间间隔 
int duration;    //系统负荷监测时段 
int iteration; 
char c1,c2;     //字符处理单元 
void sampleLoadAvg(){ //观察系统负荷 
	 float m;
	 iteration=0;
     printf("一分钟平均负载:\n"); 
	 while(iteration<duration){
		 thisProcFile=fopen("/proc/loadavg","r");
		 sleep(interval);
		 fgets(lineBuf,LB_SIZE+1,thisProcFile);
		 rewind(thisProcFile);//将文件内部的位置指针重新指向一个流（数据流/文件）的开头
		 sscanf(lineBuf,"%f",&m);
		 printf("%f\n",m);
		 fclose(thisProcFile);
		 iteration+=interval;
	 }
   //  fclose(thisProcFile); 

} 

void sampleTime(){//观察系统启动时间 
     int uptime,idletime; 
     int  day,hour,minute,second; 
     int  i,j; 
     char temp[80]; 
     i=j=0; 
     //打开计时文件  
	 thisProcFile=fopen("/proc/uptime","r");
	 if(thisProcFile==NULL){
		 printf("/proc/uptime file open failure");
		 exit(1);
	 }
      
	 fscanf(thisProcFile,"%d%d",&uptime,&idletime);
    //转换成日时钟秒 
	 day=hour=minute=second=0;
	 day=uptime/86400;
	 i=uptime%86400;
	 hour=i/3600;
	 i=i%3600;
	 minute=i/60;
	 second=i%60;
    //打印处理好的信息内容 
	 printf("启动以来经历的时间：%02d:%02d:%02d:%02d\n",day,hour,minute,second);
	 fclose(thisProcFile);
} 

int main(int argc,char *argv[]) 
{ 
      lineBuf = (char *)malloc(LB_SIZE+1); 
     // printf("%d",sizeof(struct time_t));
     // now = malloc(80);
      reportType = STANDARD; 
      strcpy(repTypeName,"Standard"); 
 
      if(argc >1)  
         sscanf(argv[1],"%c%c",&c1,&c2);//取命令行选择符
      if(c1!='-'){     //提示本程序命令参数的用法 
         exit(1); 
      } 
      if(c2 == 'b'){//观察部分B 
          printf("******PART B **********\n"); 
          //打开内存信息文件 
       
          //读出文件全部的内容 

          //处理并用方便阅读的格式显示 

//          fclose(thisProcFile);       

          //观察系统启动时间 
          sampleTime(); 
      } 
      else if(c2=='c'){//观察部分C 
          printf("******PART C**********\n"); 
          //打开系统状态信息文件 
		  thisProcFile = fopen("/proc/stat","r");
          //读出文件全部的内容 
		  char c[512],d[30];
		  int usrtime,m,systime,idletime;
		  fscanf(thisProcFile,"%s%d%d%d%d",c,&usrtime,&m,&systime,&idletime);
		  printf("用户态时间:%d(0.01秒) 系统态时间:%d(0.01秒) 空闲态时间:%d(0.01秒)\n",usrtime,systime,idletime);
          fclose(thisProcFile);

		  //打开磁盘状态信息文件
		  thisProcFile = fopen("/proc/diskstats","r");
		  int r,w;
		  while(fgets(c,512,thisProcFile)!=NULL)
		  {
			  sscanf(c,"%d %d %s %d %d %d %d %d",&m,&m,d,&r,&m,&m,&m,&w);
			  if(d[0]=='s'){
				  printf("系统接收到的磁盘请求 %s：读%d次，写%d次，总共%d次\n",d,r,w,r+w);
				  break;
			  }
		  }
		  fclose(thisProcFile);
		  //打开文件，读取上下文切换和进程信息
		  thisProcFile = fopen("/proc/stat","r");
		  while(fgets(c,512,thisProcFile)!=NULL){
			  sscanf(c,"%s %d",&d,&m);
			  if(strcmp(d,"ctxt")==0)
				  printf("上下文转换的次数:%d\n",m);
			  if(strcmp(d,"processes")==0)
				  printf("从系统启动开始创建的进程数:%d\n",m);
		  }
		  fclose(thisProcFile);
      }         
      else if(c2 == 'd'){ //观察部分 D 
          printf("******PART D **********\n"); 
          if(argc<4){ 
             printf("usage:observer [-b] [-c][-d int dur]\n"); 
             exit(1); 
          } 
		  interval = atoi(argv[2]);//将string转换成int
		  duration = atoi(argv[3]);
          reportType = LONG; 
          strcpy(repTypeName,"Long"); 
		  int i;
		  char d[30];
		  //打开内存信息文件
		  thisProcFile = fopen("/proc/meminfo","r");
		  while(fgets(lineBuf,LB_SIZE+1,thisProcFile)!=NULL){
			  sscanf(lineBuf,"%s %d",&d,&i);
			  if(strcmp(d,"MemTotal:")==0)
				  printf("计算机配置的内存大小:%d\n",i);
			  if(strcmp(d,"MemFree:")==0)
				  printf("当前可用的内存大小:%d\n",i);
		  }
		  fclose(thisProcFile);
          //用命令行参数指定的时间段和时间间隔连续的 
          //读出系统负荷文件的内容用方便阅读的格式显示 
		  sampleLoadAvg();
      } 
      else{//观察部分 A 
          int stoday;
	  printf("******PART A **********\n"); 
          reportType = SHORT; 
          strcpy(repTypeName,"Short"); 
          //取出并显示系统当前时间 
		  gettimeofday(&now, NULL );
            //  now=   time(NULL);
               // printf();
		stoday = now.tv_sec%(3600*24);
		 printf("Status report type %s at %02d:%02d:%02d\n",repTypeName,stoday/3600,(stoday%3600)/60,
			(stoday%3600)%60);
          //读出并显示机器名 
		  thisProcFile = fopen("/proc/sys/kernel/hostname","r");
		  if(thisProcFile==NULL){
			  printf("Source file can't open");
			  exit(1);
		  }
		  fgets(lineBuf,LB_SIZE+1,thisProcFile);
		  printf( "Machine hostname: %s",lineBuf);
          fclose(thisProcFile); 
          //读出并显示全部CPU信息 
		  thisProcFile=fopen("/proc/cpuinfo","r");//打开cpuinfo文件
		  if(thisProcFile==NULL){
			  printf("Source file can't open");
		      exit(1);
		  }

		  //获取需要显示的信息
		  int i;
		  for(i=0;i<5;i++)
			  fgets(lineBuf,LB_SIZE+1,thisProcFile);
		  printf("CPU型号: %s",lineBuf+13);
		  fclose(thisProcFile); 

          //读出并显示系统版本信息
		  thisProcFile=fopen("/proc/version","r");//打开version文件
		  if(thisProcFile==NULL){
			  printf("Source file can't open'");
			  exit(0);
		  }
		  fgets(lineBuf,LB_SIZE+1,thisProcFile);
		  printf("内核版本：%s",lineBuf);

          fclose(thisProcFile); 

       }
	  return 0;
} 
