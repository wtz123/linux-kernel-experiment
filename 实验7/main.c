//#include <linux/config.h> 
#include <linux/module.h> 
#include <linux/moduleparam.h> 
#include <linux/init.h> 
 
#include <linux/kernel.h>  /* printk() */ 
#include <linux/slab.h> /* kmalloc() */ 
#include <linux/fs.h>    /* everything... */ 
#include <linux/errno.h>  /* error codes */ 
#include <linux/types.h>  /* size_t */ 
#include <linux/proc_fs.h> 
#include <linux/seq_file.h> 
#include <linux/cdev.h> 
 
#include <asm/uaccess.h>  /* copy_*_user */ 
 
#include "scull.h"    /* local definitions */ 
 
/* 
 * Our parameters which can be set at load time. 
 */ 
 
int scull_major =   SCULL_MAJOR; 
int scull_minor =   0; 
int scull_nr_devs = SCULL_NR_DEVS;  /* number of bare scull devices */ 
module_param(scull_major, int, S_IRUGO); 
module_param(scull_minor, int, S_IRUGO); 
module_param(scull_nr_devs, int, S_IRUGO); 
 
MODULE_AUTHOR("Alessandro Rubini, Jonathan Corbet"); 
MODULE_LICENSE("Dual BSD/GPL"); 
 
struct scull_dev *scull_devices;  /* allocated in scull_init_module */ 
 
 
/* 
 * Empty out the scull device; must be called with the device 
 * semaphore held. 
 */ 
int scull_trim(struct scull_dev *dev) 
{ 
  memset(dev->data, 0, SCULL_DATA_SIZE); 
  dev->size = 0; 
  return 0; 
} 
 
/* 
 * Open and close 
 */ 
 
int scull_open(struct inode *inode, struct file *filp) 
{ 
  struct scull_dev *dev; /* device information */ 
 
  dev = container_of(inode->i_cdev, struct scull_dev, cdev);//根据一个结构体变量中的一个域成员变量的指针来获取指向整个结构体变量的指针 
  filp->private_data = dev; /* for other methods */ 
 
  /* now trim to 0 the length of the device if open was write-only */ 
  if ( (filp->f_flags & O_ACCMODE) == O_WRONLY) { 
    if (down_interruptible(&dev->sem)) 
      return -ERESTARTSYS; 
    scull_trim(dev); /* ignore errors */ 
    up(&dev->sem); 
      } 
  return 0;          /* success */ 
} 
 
int scull_release(struct inode *inode, struct file *filp) 
{ 
  return 0; 
} 
 
/* 
 * Data management: read and write 
 */ 
 
ssize_t scull_read(struct file *filp, char __user *buf, size_t count, 
                loff_t *f_pos) 
{ 
  struct scull_dev *dev = filp->private_data;  
  ssize_t retval = 0; 
 
  if (down_interruptible(&dev->sem)) 
    return -ERESTARTSYS; 
  if (*f_pos >= dev->size) 
    goto out; 
 
  if (*f_pos + count > dev->size) 
    count = dev->size - *f_pos; 
 
  if (copy_to_user(buf, dev->data + *f_pos, count)) { 
    retval = -EFAULT; 
    goto out; 
  } 
  *f_pos += count; 
  retval = count; 
 
out: 
  up(&dev->sem); 
  return retval; 
} 
ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, 
                loff_t *f_pos) 
{ 
  struct scull_dev *dev = filp->private_data; 
  ssize_t retval = -ENOMEM; /* value used in "goto out" statements */ 
 
  if (down_interruptible(&dev->sem)) 
    return -ERESTARTSYS; 
 
  if (*f_pos >= SCULL_DATA_SIZE) { 
    retval = -ENOSPC; 
    goto out; 
  } 
  if (*f_pos + count > SCULL_DATA_SIZE) 
    count = SCULL_DATA_SIZE - *f_pos; 
 
  if (copy_from_user(dev->data + *f_pos, buf, count)) { 
    retval = -EFAULT; 
    goto out; 
  } 
  *f_pos += count; 
  retval = count; 
  dev->size = *f_pos; 
 
  out: 
  up(&dev->sem); 
  return retval; 
} 
 
loff_t scull_llseek(struct file *filp, loff_t off, int whence) 
{ 
  struct scull_dev *dev = filp->private_data; 
  loff_t newpos; 
 
  switch(whence) { 
    case 0: /* SEEK_SET */ 
      newpos = off; 
            break; 
    case 1: /* SEEK_CUR */ 
      newpos = filp->f_pos + off; 
      break; 
    case 2: /* SEEK_END */ 
      newpos = dev->size + off; 
      break; 
    default: /* can't happen */ 
      return -EINVAL; 
  } 
  if (newpos < 0) 
    return -EINVAL; 
  filp->f_pos = newpos; 
  return newpos; 
} 
 
struct file_operations scull_fops = { 
  .owner =    THIS_MODULE, 
  .llseek =   scull_llseek, 
  .read =     scull_read, 
  .write =    scull_write, 
  .open =     scull_open, 
  .release =  scull_release, 
}; 
 
/* 
 * Finally, the module stuff 
 */ 
 
/* 
 * The cleanup function is used to handle initialization failures as well. 
 * Thefore, it must be careful to work correctly even if some of the items 
 * have not been initialized 
 */ 
void scull_cleanup_module(void) 
{ 
  int i; 
  dev_t devno = MKDEV(scull_major, scull_minor); 
   
  /* Get rid of our char dev entries */ 
  if (scull_devices) { 
    for (i = 0; i < scull_nr_devs; i++) { 
      scull_trim(scull_devices + i); 
      cdev_del(&scull_devices[i].cdev); 
    } 
    kfree(scull_devices); 
  } 
 
  /* cleanup_module is never called if registering failed */ 
  unregister_chrdev_region(devno, scull_nr_devs); 
} 
 
 
/* 
 * Set up the char_dev structure for this device. 
 */ 
static void scull_setup_cdev(struct scull_dev *dev, int index) 
{ 
  int err, devno = MKDEV(scull_major, scull_minor + index); 
     
  cdev_init(&dev->cdev, &scull_fops); 
  dev->cdev.owner = THIS_MODULE; 
  dev->cdev.ops = &scull_fops; 
  err = cdev_add (&dev->cdev, devno, 1); 
  /* Fail gracefully if need be */ 
  if (err) 
    printk(KERN_NOTICE "Error %d adding scull%d", err, index); 
} 
 
 
int scull_init_module(void) 
{ 
  int result, i; 
  dev_t dev = 0; 
 
/* 
 * Get a range of minor numbers to work with, asking for a dynamic 
 * major unless directed otherwise at load time. 
 */ 
  if (scull_major) { 
    dev = MKDEV(scull_major, scull_minor); 
    result = register_chrdev_region(dev, scull_nr_devs, "scull"); 
  } else { 
    result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, 
        "scull"); 
    scull_major = MAJOR(dev); 
  } 
  if (result < 0) { 
    printk(KERN_WARNING "scull: can't get major %d\n", scull_major); 
    return result; 
  } 
 
        /*  
   * allocate the devices -- we can't have them static, as the number 
   * can be specified at load time 
   */ 
  scull_devices  =  kmalloc(scull_nr_devs  *  sizeof(struct  scull_dev), 
GFP_KERNEL); 
  if (!scull_devices) { 
    result = -ENOMEM; 
    goto fail;  /* Make this more graceful */ 
  } 
  memset(scull_devices, 0, scull_nr_devs * sizeof(struct scull_dev)); 
 
        /* Initialize each device. */ 
  for (i = 0; i < scull_nr_devs; i++) { 
    init_MUTEX(&scull_devices[i].sem); 
    scull_setup_cdev(&scull_devices[i], i); 
  } 
 
  return 0; /* succeed */ 
 
  fail: 
  scull_cleanup_module(); 
    return result; 
} 
 
module_init(scull_init_module); 
module_exit(scull_cleanup_module); 
