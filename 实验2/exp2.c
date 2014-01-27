#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#define MAXARG 20
#define LS 80
#define SHELL_NAME "myshell$:"
#define PROMPT_ENVIRONMENT_VARIABLE "PROMPT"
char *prompt,**fullpath;
int k=0;
struct command_struct{
	char *name;		
	int argc;//参数个数
	char *argv[];//存放参数（argv[0]为命令名）
}*command,*command2;

void splitCmd(char buffer[], struct command_struct* command) {
	 int l = 0;
	 command->argc=0;
        char *p;
	command->argv[l]=malloc(LS);
	command->argv[l++]=strtok(buffer," ");
	while(p=strtok(NULL," ")){		
		command->argv[l]=malloc(LS);
		sprintf(command->argv[l++],"%s",p);	
		command->argc++;				
	}
	command->name=command->argv[0];
	command->argv[l]=NULL;
	//printf("argc=%d\n",command->argc);	
}

/**
   查找命令所在路径
   fullPathFile：存放命令所在的路径
   command：要查找的路径
**/
void getfullPath(char fullPathFile[], struct command_struct* command)
{
	int  c;
	for (c = 0; c < k; c++) {
		sprintf(fullPathFile, "%s/%s", fullpath[c], command->name);	
		
		//access()  checks  whether the calling process can access the file path‐
                //name.  If pathname is a symbolic link, it is dereferenced.
		// F_OK tests  for  the  existence  of  the file.
		if (access(fullPathFile,F_OK)==0){//文件存在access函数返回0	
			break;	
		}else{
			continue;}	
	}
	if(c==k){//可执行文件，路径为当前目录
		sprintf(fullPathFile, "%s", command->name);
	//	printf("executed:%s\n",fullPathFile);
	}
	
}

int main(){	
	char *infile,*outfile,*lineBuf,*lineBuf2,*tempBuf,*path,fullpathfilename[64],
	fullpathfilename2[64],*lineBuf3,**lines;
	int pid1,pid2;
	int i=0,j;
	int f_in,f_out;
	int filein,fileout,ispipe,isbkg,mypipe[2];
	lineBuf=(char *)malloc(LS+1);
	lineBuf2=(char *)malloc(LS+1);
	lineBuf3=(char *)malloc(LS+1);
	lines=(char *)malloc(sizeof(lines));
	int t;
	for(t=0;t<5;t++)
		lines[t]=malloc(LS);
	
	command=(struct command_struct *)malloc(command);
	command2=(struct command_struct *)malloc(command2);
	
	
	fullpath=malloc(sizeof(fullpath));
	path=getenv("PATH");
	printf("path:%s\n",path);
	fullpath[k]=malloc(LS);
	fullpath[k++]=strtok(path,":");	
	while(tempBuf=strtok(NULL,":")){//以“:”分割字符串
	//	printf("ok:");
	//	printf(tempBuf);			
		fullpath[k]=malloc(LS);
		sprintf(fullpath[k++],"%s",tempBuf);					
	}	

	if((prompt=getenv(PROMPT_ENVIRONMENT_VARIABLE))==NULL)
		prompt=SHELL_NAME;


	while(1){
		t=0;
		j=0;
		filein=0;
		fileout=0;
		ispipe=0;
		isbkg=0;
		bzero(fullpathfilename,64);//置字节字符串fullpathfile为零
		bzero(lineBuf,LS+1);
		printf(prompt);	
		gets(lineBuf3);
		if(strlen(lineBuf3)==0)
			continue;
		if(strstr(lineBuf3,"&")){//有后台执行符“&”			
			isbkg=1;			
			lineBuf3=strtok(lineBuf3,"&");			
			}
			lines[t++]=strtok(lineBuf3,";");
			printf("lines[0]:%s\n",lines[0]);
			while(lines[t]=strtok(NULL,";"))
				t++;
		int q;
		
		for(q=0;q<t;q++){
			lineBuf=lines[q];
		
		if(strstr(lineBuf," > ")){//输出重定向
			filein=1;
			lineBuf=strtok(lineBuf," > ");
			infile=strtok(NULL," > ");			
		}
		if(strstr(lineBuf," < ")){//输入重定向
			fileout=1;
			lineBuf=strtok(lineBuf," < ");
			outfile=strtok(NULL," < ");				
		}
		if(strstr(lineBuf,"|")){//管道
			ispipe=1;
			lineBuf=strtok(lineBuf,"|");
			lineBuf2=strtok(NULL,"|");	
			//printf("lineBuf2%s\n",lineBuf2);
			if (pipe(mypipe) < 0) {//创建管道，mypipe[0]用来读，mypipe[1]用来写
				/*mypipe[0] Read
				 mypipe[1] Write*/
				printf("pipe create error\n");				
				}
		
		}	
		//printf("lineBuf=%s\n",lineBuf);
		splitCmd(lineBuf,command);
		getfullPath(fullpathfilename,command);	
		
	
		if(strcasecmp(command->name,"exit")==0)
			exit(0);
		if((pid1=fork())==0){//child process

		//printf("fullpathfilename%s\n",fullpathfilename);
			if (filein) {//输出重定向
			        //只写打开（若不存在自动创建）
			        //参数mode（第三个参数）只有在建立新文件时才会生效
			        //S_IRWXU 00700 权限，代表该文件所有者具有可读、可写及可执行的权限。
			        //S_IRWXG 00070权限，代表该文件用户组具有可读、可写及可执行的权限。
			        //S_IRWXO 00007权限，代表其他用户具有可读、可写及可执行的权限。
				f_in = open(infile, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG
						| S_IRWXO);
				close(1);//1为标准输出设备
				dup(f_in);//These system calls create a copy of the file descriptor oldfd.
				close(f_in);
			}
			if (fileout) {//输入重定向
				f_out = open(outfile, O_RDONLY | O_CREAT, S_IRWXU | S_IRWXG
						| S_IRWXO);
				close(0);//0为标准输入设备
				//由dup返回的新文件描述符一定是当前可用文件描述中的最小数值
				dup(f_out);
				close(f_out);
			}
			if(ispipe){//管道
				close(mypipe[0]); //关闭read
				close(1);
				dup2(mypipe[1], 1); //int dup2(int oldfd, int newfd) makes newfd be the copy of 
				                    //oldfd，closing newfd first if  neces‐sary
				close(mypipe[1]); 
			}
			execvp(fullpathfilename,command->argv);			
			printf("EXEC %s failed:\n",command->name);			
			exit(1);
		}
		else {//parent process

			if(ispipe){	
				close(mypipe[1]); //关闭“write”一端				
				if ((pid2 = fork())==0) {//second child process			
					splitCmd(lineBuf2,command2);
					getfullPath(fullpathfilename2,command2);		
				
					close(mypipe[1]); //关闭“write”的的一端
					close(0);
					dup2(mypipe[0], 0); //在执行系统调用exec后，会释放0,1,2之外由父进程打开的文件，
					close(mypipe[0]); //所以要把myPipe[0]复制到stdin对应的文件句柄0					//printf("fullpathfilename2:%s\n",fullpathfilename2);
					
					execvp(fullpathfilename2,command2->argv);			
					printf("EXEC %s failed:%s\n",command2->name);
					exit(1);		
				}//end of second child process
		
			}//end of if(ispipe)
			
			if(!isbkg){//没有后台执行
				wait(&pid1);
				if(ispipe){
					close(mypipe[0]);
					wait(&pid2);
				}
			}
		
		/*if(WIFEXITED(pid1)){
			if(WEXITSTATUS(pid1)){
				fprintf(stderr,"child exited with status %d.\n",WEXITSTATUS(pid1));
		}
		}else {
		fprintf(stderr,"child exited unexpectedly\n");	
		}*/
		} }
	}
	int rel;
	for(rel=1;rel<=command->argc;rel++){				
				free(command->argv[rel]);
				command->argv[rel] == NULL;
			}
	free(command);
	free(fullpath);
	free(lineBuf);
	free(command2);
	free(lineBuf2);
}
