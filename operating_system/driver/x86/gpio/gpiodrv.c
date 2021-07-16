#include <linux/init.h>  
#include <linux/module.h>  
#include <asm/uaccess.h>  
#include <linux/fs.h>  
#include <linux/errno.h>  
#include <linux/gpio.h>  
//#include <linux/sys_config.h>  
#include <linux/miscdevice.h>  
  
  
#define IGPIO_DEVICE_NAME       "igpioDrv"  
#define IGPIO_IOCTL_MAGIC       0XF12  
#define IGPIO_IOCTL_RESET       _IO(IGPIO_IOCTL_MAGIC, 0x0)  
#define IGPIO_IOCTL_SET         _IO(IGPIO_IOCTL_MAGIC, 0x1)  
static DEFINE_MUTEX(igpio_ioctlmutex);          /* 声明并初始化互斥锁 */  
static int gpioNumer = 0;                       /*用以保存申请到的io*/  
  
static int igpio_open(struct inode *node, struct file *filp)  
{  
    return 0;  
}  
  
static long igpio_ioctl(struct file *filp, unsigned int cmd, unsigned long gpioNum)  
{  
    int nRet = 0;  
  
    printk(KERN_EMERG "cmd= %d,gpioNum= %d\n",cmd,gpioNum);  
    mutex_lock(&igpio_ioctlmutex);  
    if(gpioNumer != gpioNum)  
    {     
        nRet = gpio_request(gpioNum, NULL);  
        if(nRet){  
            printk(KERN_EMERG "%d gpio_request failed\n", gpioNum);  
            nRet = -EINVAL;  
            goto iExit;  
        }  
  
        gpio_free(gpioNumer); //如果申请了新的那就释放掉旧的  
        gpioNumer = gpioNum;  
    }  
      
    switch(cmd){  
        case IGPIO_IOCTL_RESET:  
            gpio_direction_output(gpioNumer,0x00);  
            break;  
        case IGPIO_IOCTL_SET:  
            gpio_direction_output(gpioNumer,0x01);  
            break;  
        default:  
        nRet = -EINVAL;  
    }  
      
iExit:  
  
    mutex_unlock(&igpio_ioctlmutex);  
    return nRet;  
}  
  
static const struct file_operations gpio_fops ={  
    .owner  = THIS_MODULE,  
    .open   = igpio_open,  
    .unlocked_ioctl = igpio_ioctl,  
};  
  
static struct miscdevice gpioMiscDevSt ={  
    .minor = MISC_DYNAMIC_MINOR,  
    .name  = IGPIO_DEVICE_NAME,  
    .fops  = &gpio_fops,  
};  

static int __init gpioDrv_init(void)  
{  
    int nRet = 0;  
    /*注册设备*/  
    nRet = misc_register(&gpioMiscDevSt);  
    printk(KERN_EMERG "%s misc_deregister nRet=%d \n",__FUNCTION__,nRet);
  
    return nRet;  
}  
  
static void __exit gpioDrv_exit(void)  
{  
    int nRet = -1;  
  
    gpio_free(gpioNumer);
    //nRet = misc_deregister(&gpioMiscDevSt);  
    // if(nRet<0)  
    //     printk(KERN_EMERG "%s misc_deregister failed\n",__FUNCTION__);  
    misc_deregister(&gpioMiscDevSt);  
}  
  
module_init(gpioDrv_init);  
module_exit(gpioDrv_exit);  
MODULE_LICENSE("GPL");  