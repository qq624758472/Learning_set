# qemu基础知识、编译、安装以及虚拟机创建

## 0.qemu和kvm的区别

​	首先KVM（Kernel Virtual Machine）是Linux的一个内核驱动模块，它能够让Linux主机成为一个Hypervisor（虚拟机监控器）。

​	在支持VMX（Virtual Machine Extension）功能的x86处理器中，Linux在原有的用户模式和内核模式中新增加了客户模式，并且客户模式也拥有自己的内核模式和用户模式，虚拟机就是运行在客户模式中。

​	KVM模块的职责就是打开并初始化VMX功能，提供相应的接口以支持虚拟机的运行。

​	QEMU（quick emulator)本身并不包含或依赖KVM模块，而是一套由Fabrice Bellard编写的模拟计算机的自由软件。

​	QEMU虚拟机是一个纯软件的实现，可以在没有KVM模块的情况下独立运行，但是性能比较低。

​	QEMU有整套的虚拟机实现，包括处理器虚拟化、内存虚拟化以及I/O设备的虚拟化。

​	QEMU是一个用户空间的进程，需要通过特定的接口才能调用到KVM模块提供的功能。

​	从QEMU角度来看，虚拟机运行期间，QEMU通过KVM模块提供的系统调用接口进行内核设置，由KVM模块负责将虚拟机置于处理器的特殊模式运行。

​	QEMU使用了KVM模块的虚拟化功能，为自己的虚拟机提供硬件虚拟化加速以提高虚拟机的性能。

​	KVM只模拟CPU和内存，因此一个客户机操作系统可以在宿主机上跑起来，但是你看不到它，无法和它沟通。于是，有人修改了QEMU代码，把他模拟CPU、内存的代码换成KVM，而网卡、显示器等留着，因此QEMU+KVM就成了一个完整的虚拟化平台。

​	KVM只是内核模块，用户并没法直接跟内核模块交互，需要借助用户空间的管理工具，而这个工具就是QEMU。

​	KVM和QEMU相辅相成，QEMU通过KVM达到了硬件虚拟化的速度，而KVM则通过QEMU来模拟设备。

​	对于KVM来说，其匹配的用户空间工具并不仅仅只有QEMU，还有其他的，比如RedHat开发的libvirt、virsh、virt-manager等，QEMU并不是KVM的唯一选择。

​	所以简单直接的理解就是：QEMU是个计算机模拟器，而KVM为计算机的模拟提供加速功能。

## 1.编译与安装

```sh
$cd qemu-2.0.0 			#如果使用的是git下载的源码，执行cd qemu
$./configure --enable-kvm --enable-debug --enable-vnc --enable-werror  --target-list="x86_64-softmmu"
$make -j8
$sudo make install
```

- --enable-vnc：启用 VNC。 
- --enable-kvm：编译 KVM 模块，使 Qemu 可以利用 KVM 来访问硬件提供的虚拟化服务。 
- --enalbe-werror：编译时，将所有的警告当作错误处理。 
- --target-list：选择目标机器的架构。默认是将所有的架构都编译，但为了更快的完成编译，指定需要的架构即可



安装好之后，会生成如下应用程序：

- ivshmem-client/server：这是一个 guest 和 host 共享内存的应用程序，遵循 C/S 的架构。
- qemu-ga：这是一个不利用网络实现 guest 和 host 之间交互的应用程序（使用 virtio-serial），运行在 guest 中。
- qemu-io：这是一个执行 Qemu I/O 操作的命令行工具。
- qemu-system-x86_64：Qemu 的核心应用程序，虚拟机就由它创建的。
- qemu-img：创建虚拟机镜像文件的工具，下面有例子说明。
- qemu-nbd：磁盘挂载工具。





## 2.创建虚拟机

2.1使用qemu-img创建虚拟机镜像

​	虚拟机镜像用来模拟虚拟机的硬盘，在启动虚拟机之前需要创建镜像文件。

```sh
qemu-img create -f qcow2 test-vm-1.qcow2 10G
```

-f 选项用于指定镜像的格式，qcow2 格式是 Qemu 最常用的镜像格式，采用来写时复制技术来优化性能。

test-vm-1.qcow2 是镜像文件的名字，10G是镜像文件大小。



2.2使用 qemu-system-x86 来启动 x86 架构的虚拟机

```sh
qemu-system-x86_64 test-vm-1.qcow2
```

因为 test-vm-1.qcow2 中并未给虚拟机安装操作系统，所以会提示 “No bootable device”，无可启动设备。



- 启动 VM 安装操作系统镜像

```sh
qemu-system-x86_64 -m 2048 -enable-kvm test-vm-1.qcow2 -cdrom ./Centos-Desktop-x86_64-20-1.iso
```

-m 指定虚拟机内存大小，默认单位是 MB， 

-enable-kvm 使用 KVM 进行加速，

-cdrom 添加 fedora 的安装镜像。

可在弹出的窗口中操作虚拟机，安装操作系统，安装完成后重起虚拟机便会从硬盘 ( test-vm-1.qcow2 ) 启动。

- 之后再启动虚拟机只需要执行：

```sh
qemu-system-x86_64 -m 2048 -enable-kvm test-vm-1.qcow2
```

qemu-img 支持非常多种的文件格式，可以通过 qemu-img -h 查看 

其中 raw 和 qcow2 是比较常用的两种，raw 是 qemu-img 命令默认的，qcow2 是 qemu 目前推荐的镜像格式，是功能最多的格式。















# 给qemu社区提交代码

相关网站：

qemu官网：https://www.qemu.org/

qemu贡献：https://www.qemu.org/contribute/

qemu文档：https://qemu-project.gitlab.io/qemu/about/index.html

贡献步骤：

​	1.在我们的错误跟踪器中报告错误：[https](https://gitlab.com/qemu-project/qemu/-/issues) : [//gitlab.com/qemu-project/qemu/-/issues](https://gitlab.com/qemu-project/qemu/-/issues)

​	2.克隆（[或浏览](https://gitlab.com/qemu-project/qemu)）git 存储库：git clone https://gitlab.com/qemu-project/qemu.git

​	3.











# 源码解析

## arm架构的一处初始化代码

type_init(arm_cpu_register_types)解析：

```c
type_init(arm_cpu_register_types)
	#define type_init(function) module_init(function, MODULE_INIT_QOM)
		#define module_init(function, type)          
```

```c
#define module_init(function, type)                                         \
static void __attribute__((constructor)) do_qemu_init_ ## function(void)    \
{                                                                           \
    register_module_init(function, type);                                   \
}
```

函数在预编译时，解析为：

```
#define module_init(function, type)                                         
static void __attribute__((constructor)) do_qemu_init_arm_cpu_register_types(void)    
{                                                                           
    register_module_init(function, type);                                   
}
```

并在main()函数调用之前执行。



\_\_attribute\_\_((constructor))功能：这是gnu c编译器扩展，不属于c语言标准库功能。 加入后表示该函数在main函数调用之前执行。同理gcc中也有\_\_attribute__((destructor))，会在main函数之后执行。



register_module_init(function, type)解析：









# kvm子系统

## kvm和qemu的关系

​	首先KVM（Kernel Virtual Machine）是Linux的一个内核驱动模块，它能够让Linux主机成为一个Hypervisor（虚拟机监控器）。在支持VMX（Virtual Machine Extension）功能的x86处理器中，Linux在原有的用户模式和内核模式中新增加了客户模式，并且客户模式也拥有自己的内核模式和用户模式，虚拟机就是运行在客户模式中。KVM模块的职责就是打开并初始化VMX功能，提供相应的接口以支持虚拟机的运行。

​	QEMU（quick emulator)本身并不包含或依赖KVM模块，而是一套由Fabrice Bellard编写的模拟计算机的自由软件。QEMU虚拟机是一个纯软件的实现，可以在没有KVM模块的情况下独立运行，但是性能比较低。QEMU有整套的虚拟机实现，包括处理器虚拟化、内存虚拟化以及I/O设备的虚拟化。QEMU是一个用户空间的进程，需要通过特定的接口才能调用到KVM模块提供的功能。从QEMU角度来看，虚拟机运行期间，QEMU通过KVM模块提供的系统调用接口进行内核设置，由KVM模块负责将虚拟机置于处理器的特殊模式运行。QEMU使用了KVM模块的虚拟化功能，为自己的虚拟机提供硬件虚拟化加速以提高虚拟机的性能。

​	KVM只模拟CPU和内存，因此一个客户机操作系统可以在宿主机上跑起来，但是你看不到它，无法和它沟通。于是，有人修改了QEMU代码，把他模拟CPU、内存的代码换成KVM，而网卡、显示器等留着，因此QEMU+KVM就成了一个完整的虚拟化平台。

​	KVM只是内核模块，用户并没法直接跟内核模块交互，需要借助用户空间的管理工具，而这个工具就是QEMU。KVM和QEMU相辅相成，QEMU通过KVM达到了硬件虚拟化的速度，而KVM则通过QEMU来模拟设备。对于KVM来说，其匹配的用户空间工具并不仅仅只有QEMU，还有其他的，比如RedHat开发的libvirt、virsh、virt-manager等，QEMU并不是KVM的唯一选择。

​	所以简单直接的理解就是：QEMU是个计算机模拟器，而KVM为计算机的模拟提供加速功能。

