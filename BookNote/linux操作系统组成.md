# X86平台

预留。。。



# ARM平台

## Bootloader

预留。。。



## 根文件系统

​	一般而言,在Linux系统有两种init程序: BSD init和 System V init。

​	BSD和 SysterV是两种版本的UNIX系统。这两种init程序各有优缺点,现在大多 Linux的发行版本使用 System V init。但是在嵌入式领域,通常使用 Busybox集成的init程序。

### Busybox

busybox制作过程查看《内核调试技术》中arm系统制作章节。

#### /etc/inittab文件详解:

[Linux系统](https://www.baidu.com/s?wd=Linux系统&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的启动过程为：

​	加电自检-->根据BIOS中的设置从指定的设备启动-->找到设备MBR中的[bootloader](https://www.baidu.com/s?wd=bootloader&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)引导启动系统-->启动kernel-->启动init进程

init进程启动后，根据/etc/inittab文件从不同的运行级别启动相应的进程或执行相应的操作。

 /etc/inittab这个文件中定义的登记项都是以：隔开的四个段，即：

  **id：runlevels：action：process**  

  **id**：它是每个登记项的标识符，用于唯一标识每个登记项，不能重复

  **runlevels**：系统的运行级别，表示process的action要在哪个级别下运行，该段中可以定义多个运行级别，各级别之间直接写不用分隔符；如果为空，表示在所有的运行级别运行。Linux的运行级别有：

```c
  0：表示关机
  1：表示单用户模式，在这个模式中，用户登录不需要密码，默认网卡驱动是不被加载，一些服务不能用。
  2：表示多用户模式，NFS服务不开启
  3，表示命令行模式
  4，这个模式保留未用
  5，表示图形用户模式
  6，表示重启系统 
```

  **action**：表示对应登记项的process在一定条件下所要执行的动作。

  具体动作有：

```sh
respawn：当process终止后马上启动一个新的

askfirst : 这个类似于上面的respawn，但是在运行process之前，它会打印"Please press Enter to activate this console."，然后等待用户按下Enter来启动该process。通常askfirst是用于启动终端设备的，如果不想看到这个提示，就像我那样使用respawn，而非askfirst.

wait：当进入指定的runlevels后process才会启动一次，并且到离开这个runlevels终止

initdefault：设定默认的运行级别，即我们开机之后默认进入的运行级别，不能是0,6，你懂的

sysinit：系统初始化，只有系统开机或重新启动的时候，这个process才会被执行一次

powerwait：当init接收到电源失败信号的时候执行相应的process，并且如果init有进程在运行，会等待这个进程完成之后，再执行相应的process

powerfail：当init接收到电源失败信号的时候执行相应的process，并且如果init有进程在运行，不会等待这个进程完成，它会直接执行相应的process

powerokwait：电源已经故障，但是在等待执行对应操作的时候突然来电了就执行对应的process

powerfailnow：当电源故障并且init被通知UPS电源已经快耗尽执行相对应的process

ctrlaltdel：当用户按下ctrl+alt+del这个组合键的时候执行对应的process

boot：只有在引导过程中，才执行该进程，但不等待该进程的结束；当该进程死亡时，也不重新启动该进程

bootwait：只有在引导过程中，才执行该进程，并等待进程的结束；当该进程死亡时，也不重新启动该进程

off：如果process正在运行，那么就发出一个警告信号，等待20秒后，再通过杀死信号强行终止该process。如果process并不存在那么就忽略该登记项

once：启动相应的进程，但不等待该进程结束便继续处理/etc/inittab文件中的下一个登记项；当该进程死亡时，init也不重新启动该进程
```

 

  **process**：表示启动哪个程序或脚本或执行哪个命令等

```sh
::sysinit:/bin/chmod 777 /etc/init.d/rcS
::sysinit:/bin/sh /etc/init.d/rcS
::respawn:-/bin/sh
::askfirst:-/bin/sh
::ctrlaltdel:/bin/umount -a -r
::shutdown:/bin/umount -a -r
```



#### /etc/init.d/rcS

一般使用init进程读取/etc/inittab文件时，第一个就是调用sh命令用来初始化系统。

```sh
#!/bin/sh
/bin/mkdir -p /proc
/bin/mkdir -p /tmp
/bin/mkdir -p /sys
/bin/mkdir -p /mnt
/bin/mount -a
/bin/mkdir -p /dev/pts
/bin/mount -t devpts devpts /dev/pts
/bin/echo /sbin/mdev > /proc/sys/kernel/hotplug
/sbin/mdev -s
```



#### /etc/fstab

用来将系统设备挂载到某个文件夹。

在/etc/init.d/rcS文件中调用mount -a时开始挂载。

```sh
proc /proc proc defaults 0 0	#挂载proc文件系统
tmpfs /tmp tmpfs defaults 0 0	#挂载tmpfs文件系统
sysfs /sys sysfs defaults 0 0	#挂载sysfs文件系统
tmpfs /dev tmpfs defaults 0 0
debugfs /sys/kernel/debug debugfs defaults 0 0
```





详情系统启动示例请查看《内核调试技术》arm文件系统制作步骤。



### 制作文件系统映像文件

预留。。。





















