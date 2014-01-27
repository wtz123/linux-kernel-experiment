/*************************************************************************** 
                          main.c   -   description 
     begin                 : 一  4月   4 21:01:11 CST 2003 
     copyright             : (C) 2003 by 张鸿烈 
     Function              : 编写创建proc文件系统的模块,该程序创建在/proc目录下 
                          : 创建 mydir目录,在mydir 目录下创建保存当前系统时间   
             ：jiffies 值的文件 myfile, 
***************************************************************************/ 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/proc_fs.h> 
#include <linux/jiffies.h> 
#include <asm/uaccess.h> 
#include <linux/moduleparam.h> 
#define MODULE_NAME "Myproc" 
#define MYDATA_LEN   16 
 
//放用户空间传入的数据 
struct my_proc_data{ 
   char value[MYDATA_LEN]; 
}; 
struct my_proc_data mydata;   
//proc结构变量 
static struct proc_dir_entry *example_dir; 
static struct proc_dir_entry *date_file; 
static int   param; 
module_param(param, int , 0644); 
 
//读文件 myfile的读驱动函数 
static int proc_read(char *page, char **start,off_t off,int count,int *eof,void *data ){ 
    int len=0 ; 
    struct my_proc_data *mydatap = (struct my_proc_data *) data; 
    len+=sprintf(page,"%s%ld\n",mydatap->value,jiffies); 
    return len;    
}    
//写文件 myfile 的写驱动函数 
static int proc_write(struct file *file,const char *buffer,unsigned long count,void *data){ 
    int len ; 
    struct my_proc_data *mydatap = (struct my_proc_data *) data; 
    if(count>MYDATA_LEN) 
       len = MYDATA_LEN; 
    else   
       len = count; 
    if(copy_from_user(mydatap->value,buffer,len)){ 
       return -EFAULT; 
    } 
    mydatap->value[len-1] = '\0'; 
    return len;    
}    
//装入模块 
int init_module(void) 
{ 
   //创建 proc/myfile目录 
   example_dir = (struct proc_dir_entry * )proc_mkdir("mydir",0); 
   if(example_dir == 0){
   	  printk("mkdir fail\n"); 
      return -1; 
   } 
   //创建/proc/mydir/myfile 文件 
   date_file = (struct proc_dir_entry * )create_proc_entry("myfile",0666,example_dir); 
   if(date_file == 0){ 
      remove_proc_entry("myfile",0); 
      printk("mkfile fail\n"); 
      return -ENOMEM; 
   }      
   strcpy(mydata.value,"Ticks="); 
   date_file->data=&mydata; 
   date_file->read_proc=&proc_read; 
   date_file->write_proc=&proc_write; 
   //date_file->owner=THIS_MODULE; 
   return 0; 
} 
//卸载模块 
void cleanup_module(void) 
{ 
   remove_proc_entry("myfile",example_dir); 
   remove_proc_entry("mydir",NULL); 
   printk("Goodbye.\n"); 
} 
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("Test"); 
MODULE_AUTHOR("xxx"); 
