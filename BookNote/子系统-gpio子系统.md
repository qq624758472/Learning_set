# GPIO子系统

General-purpose input/output 

通用型之输入输出的简称.

​	gpio子系统帮助我们管理整个系统gpio的使用情况，同时通过sys文件系统导出了调试信息和应用层控制接口。它内部实现主要提供了两类接口，一类给bsp工程师，用于注册gpio_chip（也就是所谓的gpio控制器驱动），另一部分给驱动工程师使用，为驱动工程师屏蔽了不同gpio chip之间的区别，驱动工程师调用的api的最终操作流程会导向gpio对应的gpio chip的控制代码，也就是bsp的代码。

​	本文重点介绍gpio驱动.

​	功能类似8051的P0—P3，其接脚可以供[使用者](https://baike.baidu.com/item/使用者/1162412)由程控自由使用，PIN脚依现实考量可作为通用输入（*GPI*）或通用输出（*GPO*）或通用输入与输出（*GPIO*），如当clk generator, chip select等。

​	既然一个[引脚](https://baike.baidu.com/item/引脚/10879873)可以用于输入、输出或其他特殊功能，那么一定有[寄存器](https://baike.baidu.com/item/寄存器/187682)用来选择这些功能。

​	对于输入，一定可以通过读取某个寄存器来确定引脚电位的高低；

​	对于输出，一定可以通过写入某个寄存器来让这个引脚输出高电位或者低电位；

​	对于其他特殊功能，则有另外的寄存器来控制它们。

## 介绍

### 什么是GPIO？

​	(GPIO) 是一种灵活的软件控制数字信号。它们由多种芯片提供，对于使用嵌入式和定制硬件的 Linux 开发人员来说是熟悉的。每个 GPIO 代表一个连接到特定引脚或球栅阵列 (BGA) 封装上的“球”的位。电路板原理图显示哪些外部硬件连接到哪些 GPIO。可以通用地编写驱动程序，以便电路板设置代码将此类引脚配置数据传递给驱动程序。



片上系统（SOC）处理器严重依赖GPIO。

在某些情况下,非专用引脚可配置为GPIO；

大多数芯片至少有几十个。

可编程逻辑器件（如FPGA）可以轻松提供GPIO；

多功能芯片，如电源管理器和音频编解码器通常有几个这样的引脚来帮助解决SOC上引脚不足的问题；

还有“GPIO扩展器”芯片，使用I2C或SPI串行总线连接。

大多数PC机都有几十个支持GPIO的引脚（只有BIOS固件知道如何使用它们）。



GPIO 的确切功能因系统而异。常用选项：

> - 输出值是可写的（高=1，低=0）。一些芯片还可以选择如何驱动该值.
> - 输入值同样是可读的 (1, 0)。一些芯片支持配置为“输出”的引脚回读，这在这种“线或”情况下非常有用（以支持双向信号）。GPIO 控制器可能具有输入去毛刺/去抖动逻辑，有时带有软件控制。
> - 输入通常可以用作 IRQ 信号，通常是边沿触发，但有时是电平触发。此类 IRQ 可配置为系统唤醒事件，以将系统从低功率状态唤醒。
> - 通常，GPIO 可根据不同产品板的需要配置为输入或输出；单向的也存在。
> - 大多数 GPIO 可以在持有自旋锁时访问，但通过串行总线访问的那些通常不能。一些系统支持这两种类型。



​	在给定的板上，每个GPIO都用于一个特定的目的，如监视MMC/SD卡插入/拔出，检测卡写保护状态，驱动

一个LED，配置一个收发机，敲打一个串行总线，戳一个硬件看门狗，感应开关，等等。

### 高电平有效和低电平有效

假设GPIO 在其输出信号为 1（“高”）时为“活动”，而在为 0（“低”）时为非活动。

具体要看设备树怎么来设定.

这个根据具体的芯片和外设之间的电路链接.

![](/home/hao/work_my/Learning_set/BookNote/pic/gpio_1.jpg)

由图所示,cpu链接led和上拉电阻.  

则cpu的gpio口为低电平时,led亮,则表明该gpio口低电平有效.



例如:	该gpio控制的为led的亮灭.

gpiod_set_value() 设置的值是逻辑值而非电平值，1 表示使能，0 表示不使能

| Function                  | line property | physical line |
| ------------------------- | ------------- | ------------- |
| gpiod_set_value(desc, 0); | active high   | low           |
| gpiod_set_value(desc, 1); | active high   | high          |
| gpiod_set_value(desc, 0); | active low    | high          |
| gpiod_set_value(desc, 1); | active low    | low           |

表中所示,设备树中设置为high时有效,驱动调用gpiod_set_value(desc, 0)意思为该引脚不使能,则Led为灭.

### 优缺点

低功耗：GPIO具有更低的功率损耗(大约1μA，μC的工作电流则为100μA)。

集成IIC从机接口：GPIO内置IIC从机接口，即使在待机模式下也能够全速工作。

小封装：GPIO器件提供最小的封装尺寸 ― 3mm x 3mm QFN!

低成本：您不用为没有使用的功能买单。

快速上市：不需要编写额外的代码、文档，不需要任何维护工作。

灵活的灯光控制：内置多路高分辨率的PWM输出。

可预先确定响应时间：缩短或确定外部事件与中断之间的响应时间。

更好的灯光效果：匹配的电流输出确保均匀的显示亮度。

布线简单：仅需使用2条就可以组成IIC总线或3条组成SPI总线。

与ARM 的几组GPIO[引脚](https://baike.baidu.com/item/引脚)，功能相似，GPxCON 控制引脚功能，GPxDAT用于读写引脚数据。另外，GPxUP用于确定是否使用上拉电阻。 x为A,B,,H/J,

GPAUP 没有上拉电阻。


### x86平台gpio注意事项

调X86平台GPIO的时候，先看清是Super IO (SIO)的GPIO，还是南桥（PCH）的GPIO
它们可能重名，比如都叫GP20
 这些GPIO都是通过打开/dev/port设备来操作，只不过操作的地址不同
 原理图上，PCH(南桥)的GPIO一般表示为PCH_GPIOxxx
 南桥GPIO调时看文档看Intel的datasheet
 而SIO的芯片一般是winbond的，看这个文档调试


## gpio驱动和gpio chip驱动

![](/home/hao/work_my/Learning_set/BookNote/pic/gpio_2.png)

正常情况下，驱动工程师不需要了解 gpio chip driver 和 gpiolib：

- 驱动工程师负责编写 gpio drvier;
- 芯片厂商的 bsp 工程师负责编写 gpio chip driver;
- 开源社区里的人负责 gpiolib 的核心实现;





## Sysfs Interface for Userspace

用户空间的 Sysfs 接口.

gpio_operation 通过 /sys/ 文件接口操作 IO 端口 GPIO 到文件系统的映射。
控制 GPIO 的目录位于 /sys/class/gpio。
/sys/class/gpio/export 文件用于通知系统需要导出控制的 GPIO 引脚编号。
/sys/class/gpio/unexport 用于通知系统取消导出。
/sys/class/gpio/gpiochipX 目录保存系统中 GPIO 寄存器的信息，包括每个寄存器控制引脚的起始编号 base，寄存器名称，引脚总数


若要操作某个gpio,则需要参考数据手册.

以编号19为例:
1.向文件 /sys/class/gpio/export 写入引脚编号，即可激活引脚。
命令执行成功后，目录下，便会出现 gpio18 文件夹，如果没有出现，则表示引脚不可导出。

```shell
echo 19 > /sys/class/gpio/export
```

2.引脚导出成功后，即可通过写入 /sys/class/gpio/gpio19/direction 控制引脚 [输入] 或 [输出]。

```shell
echo "out" > direction

#in		引脚输入信号
#out		输出控制到引脚
#high 	输出一个高电平（`value`:`1`）
#low 	输出一个低电平（`value`:`0`）
```

3.输入输出的值，通过 gpioX 下的 value 文件控制

当为输入时，请 echo /sys/class/gpio/gpio18/value 读出值；
当为输出时，请 echo 1 > /sys/class/gpio/gpio18/value 输入值；

这里 value 的值对应着，上面的 high 和 low 时的 1 / 0

4.取消导出

```shell
echo 19 > /sys/class/gpio/unexport
```


## GPIO implementor’s framework 

GPIO 实现者的框架

要启用此框架，平台的 Kconfig 将“选择”GPIOLIB，否则由用户配置对 GPIO 的支持。

作用：

- 向下为 gpio chip driver 提供注册 struct gpio_chip 的接口：gpiochip_xxx();
- 向上为 gpio consumer 提供引用 gpio 的接口：gpiod_xxx();
- 实现字符设备的功能;
- 注册 sysfs;

目前 gpio subsystem 提供了 2 套接口:
**legacy API：**

​	integer-based GPIO interface，形式为 gpio_xxx()，例如 void gpio_set_value(unsigned gpio, int value);

**推荐 API:** 

​	descriptor-based GPIO interface，形式为 gpiod_xxx()，例如 void gpiod_set_value(struct gpio_desc *desc, int value)，新添加的驱动代码一律采用这套 API。

### Now GPIO Interfaces

全部接口非常多,在内核代码include/linux/gpio/consumer.h文件中.

常用api为:

```c
//获得/释放 一个或者一组 gpio：
[devm]_gpiod_get*()
[devm]_gpiod_put*()
//设置/查询 输入或者输出
gpiod_direction_input()
gpiod_direction_output()
gpiod_get_direction()
//读写一个 gpio
gpiod_get_value()
gpiod_set_value()
gpiod_get_value_cansleep()
gpiod_set_value_cansleep()
//读写一组 gpio
gpiod_get_array_value()
gpiod_set_array_value()
//获得 gpio 对应的中断号
gpiod_to_irq()
```



代码示例:

```c
static struct gpio_desc *red, *green, *btn1, *btn2;
static int irq;

static irqreturn_t btn1_pushed_irq_handler(int irq, void *dev_id)
{
    int state;

    /* read the button value and change the led state */
    state = gpiod_get_value(btn2);
    gpiod_set_value(red, state);
    gpiod_set_value(green, state);

    pr_info("btn1 interrupt: Interrupt! btn2 state is %d)\n", state);
    return IRQ_HANDLED;
}

static const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "gpio-descriptor-sample", },
};


static int my_pdrv_probe(struct platform_device *pdev)
{
    int retval;
    struct device *dev = &pdev->dev;

    // 获得 gpio descriptor 的同时也将其设置为 output，并且输出低电平
    red = gpiod_get_index(dev, "led", 0, GPIOD_OUT_LOW);
    green = gpiod_get_index(dev, "led", 1, GPIOD_OUT_LOW);
    
    btn1 = gpiod_get(dev, "btn1", GPIOD_IN);
    btn2 = gpiod_get(dev, "btn2", GPIOD_IN);

    // 获得中断号
    irq = gpiod_to_irq(btn1);

    // 申请中断
    retval = request_threaded_irq(irq, NULL,
                            btn1_pushed_irq_handler,
                            IRQF_TRIGGER_LOW | IRQF_ONESHOT,
                            "gpio-descriptor-sample", NULL);
    pr_info("Hello! device probed!\n");
    return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
    free_irq(irq, NULL);

    // 释放 gpio
    gpiod_put(red);
    gpiod_put(green);
    gpiod_put(btn1);
    gpiod_put(btn2);
    pr_info("good bye reader!\n");
    return 0;
}

static struct platform_driver mypdrv = {
    .probe      = my_pdrv_probe,
    .remove     = my_pdrv_remove,
    .driver     = {
        .name     = "gpio_descriptor_sample",
        .of_match_table = of_match_ptr(gpiod_dt_ids),  
        .owner    = THIS_MODULE,
    },
};
module_platform_driver(mypdrv);
```



### Legacy GPIO Interfaces

常用api:

```c
gpio_request(gpioNum,gpioName)
//申请一个空闲的GPIO

gpio_free(gpioNum)
//释放申请到的GPIO

gpio_direction_input(gpioNum)
//设置GPIO为输入模式

gpio_direction_output(gpioNum,value)
//设置GPIO为输出模式+值

gpio_get_value(gpioNum)
//获取GPIO的电平值

gpio_set_value(gpioNum,value)
//设置GPIO输出电平值

gpio_to_irq(gpioNum)
//返回GPIO对应的中断号

irq_to_gpio(irq)
//返回irq对应的GPIO号
```

代码示例:

```c
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
```


