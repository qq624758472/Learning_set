# UOS上使用KVM安装一个linux系统

## 1.KVM安装虚拟机的使用

### 一、安装

安装命令：

```shell
sudo apt -y install libvirt0 libvirt-daemon qemu virt-manager bridge-utils libvirt-clients python-libvirt qemu-efi uml-utilities virtinst qemu-system
```

### 二、打开

![](/home/hao/work_my/Learning_set/tools_Lib/all_picture/UOS上KVM的使用/1.png)

### 三、网络设置

编辑->连接详情->虚拟网络

![](/home/hao/work_my/Learning_set/tools_Lib/all_picture/UOS上KVM的使用/2.png)default 是KVM安装时默认创建的虚拟网络

![](/home/hao/work_my/Learning_set/tools_Lib/all_picture/UOS上KVM的使用/3.png)

![4](/home/hao/work_my/Learning_set/tools_Lib/all_picture/UOS上KVM的使用/4.png)

![5](/home/hao/work_my/Learning_set/tools_Lib/all_picture/UOS上KVM的使用/5.png)

![6](/home/hao/work_my/Learning_set/tools_Lib/all_picture/UOS上KVM的使用/6.png)

![7](/home/hao/work_my/Learning_set/tools_Lib/all_picture/UOS上KVM的使用/7.png)

![8](/home/hao/work_my/Learning_set/tools_Lib/all_picture/UOS上KVM的使用/8.png)

在步骤4中的网络模式：

* 隔离的虚拟网络->Host Only：

* 转发到物理网络：
  * 目的：默认即可，通过本主机网卡转发
  * 模式：NAT、路由、Open

* SR-IOV ：技术是一种基于硬件的虚拟化解决方案，可提高性能和可伸缩性

### 四、安装虚拟机

新建->加载iso镜像文件->分配资源。 等，和vm差不多，不过多介绍。



# 