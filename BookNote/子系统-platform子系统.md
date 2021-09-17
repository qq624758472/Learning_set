# platform子系统

## 什么是platform总线？

platform总线是一种虚拟、抽象出来的总线，实际中并不存在这样的总线。

是Linux设备驱动模型为了保持设备驱动的统一性而虚拟出来的总线。

因为对于usb设备、i2c设备、pci设备、spi设备等等，他们与cpu的通信都是直接挂在相应的总线下面与我们的cpu进行数据交互的，但是在我们的嵌入式系统当中，并不是所有的设备都能够归属于这些常见的总线，在嵌入式系统里面，SoC系统中集成的独立的外设控制器、挂接在SoC内存空间的外设却不依附与此类总线。所以Linux驱动模型为了保持完整性，将这些设备挂在一条虚拟的总线上（platform总线），而不至于使得有些设备挂在总线上，另一些设备没有挂在总线上。



## platform device

平台设备是通常在系统中显示为实体的设备.

这包括传统的基于端口的设备和到外围总线的主桥，以及集成到片上系统平台的大多数控制器。它们通常的共同点是从 CPU 总线直接寻址。极少情况下，platform_device将通过某种其他类型的总线连接；但它的寄存器仍然可以直接寻址。

为平台设备指定一个名称（用于驱动程序绑定）和一个资源列表（如地址和IRQ）：

用如下结构表示一个设备：

```c
struct platform_device {
      const char      *name;// 平台总线下设备的名字
      u32             id;
      struct device   dev;	// 所有设备通用的属性部分
      u32             num_resources; // 设备使用到的resource的个数
      struct resource *resource;// 设备使用到的资源数组的首地址
};

//设备资源信息
struct resource {
    resource_size_t start; //资源起始地址
    resource_size_t end;   //资源结束地址
    const char *name;
    unsigned long flags;    //资源类型
    struct resource *parent, *sibling, *child;
};
```





## platform driver

​	平台驱动程序遵循标准驱动程序模型约定。

​	我们只需要关注总线，设备和驱动这三个实体，总线将设备和驱动绑定。

​	系统每注册一个设备的时候，会寻找与之匹配的驱动；相反，系统每注册一个驱动的时候，会寻找与之匹配的设备，而匹配则由总线完成。

```c
struct platform_driver {
      int (*probe)(struct platform_device *);
      int (*remove)(struct platform_device *);
      void (*shutdown)(struct platform_device *);
      int (*suspend)(struct platform_device *, pm_message_t state);
      int (*suspend_late)(struct platform_device *, pm_message_t state);
      int (*resume_early)(struct platform_device *);
      int (*resume)(struct platform_device *);
      struct device_driver driver;
};
```

一般来说，probe（）应该验证指定的设备硬件是否实际存在。



## driver和device的匹配过程

第一步：系统启动时在bus系统中注册platform
第二步：提供platform_device
第三步：提供platform_driver
第四步：platform的match函数发现driver和device匹配后，调用driver的probe函数来完成驱动的初始化和安装，然后设备就工作起来了



## 常用接口

```c
//platform driver注册
int platform_driver_register(struct platform_driver *drv);

//在已知设备不可热插拔的常见情况下，probe（）例程可以存在于init部分中，以减少驱动程序的运行时内存占用
int platform_driver_probe(struct platform_driver *drv,
                  int (*probe)(struct platform_device *));
    
//内核模块可以由多个平台驱动程序组成。平台核心提供了用于注册和注销一组驱动程序的接口
//如果其中一个driver未能注册，则在此之前注册的所有driver将以相反的顺序取消注册。
int __platform_register_drivers(struct platform_driver * const *drivers,
                              unsigned int count, struct module *owner);
void platform_unregister_drivers(struct platform_driver * const *drivers,
                                 unsigned int count);


//platform device注册
int platform_device_register(struct platform_device *pdev);

int platform_add_devices(struct platform_device **pdevs, int ndev);
```





## 驱动架构

```c
int hello_probe(struct platform_device *pdev)
{  
     //当platform_device 和platform_driver 匹配成功之后，才会调用probe，
     1. 申请设备号
     2. cdev
     3. class
     4. ioremap
     5. beep初始化
}

int hello_remove(struct platform_device *pdev)
{
     释放资源
}

struct platform_driver pdrv={
    .probe = hello_probe,
    .remove = hello_remove,
    .driver.name = "chuanpu",
};

static int hello_init(void)
{   
    return platform_driver_register(&pdrv);
}

static void hello_exit(void)
{
    platform_driver_unregister(&pdrv);
   
}
```





