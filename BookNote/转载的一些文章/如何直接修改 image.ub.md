> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [blog.csdn.net](https://blog.csdn.net/weixin_43969075/article/details/117410890)

#### 文章目录

*   [前言](#_7)
*   [一、如何分解 image.ub？](#imageub_15)
*   *   [1. 查看原有 image.ub 里内容](#1imageub_21)
    *   [2. 提取 image.ub 里的镜像](#2imageub_77)
*   [二、修改 image.ub 里内容](#imageub_87)
*   *   [1. 修改设备树](#1_88)
*   [三、合成 image.ub](#imageub_96)
*   [总结](#_165)

前言
--

笔者以帮客户解决问题为生，各种奇怪的案子都会接，有一天一个客户说 zynqMP 下使用博通 phy 有个奇怪的问题，不跑系统 SDK 测试正常、uboot 下测试正常、Linux 系统下不能收包，让我解决，但是不提供 [vivado](https://so.csdn.net/so/search?q=vivado&spm=1001.2101.3001.7020) 工程，hdf 也不行，只提供 image.ub 文件，不能再多了，好吧，硬着头皮接下这个案子

一、如何分解 image.ub？
----------------

image.ub 是通过 mkimage 这个工具合成的，那么分解还是使用这个工具分解，mkimage 是 uboot 里带的工具，编译完 uboot 可以找到这个工具，ubuntu 下也可以通过 apt 安装

```
sudo apt-get install u-boot-tools -y
```

### 1. 查看原有 image.ub 里内容

通过运行命令 mkimage -l 命令查询原 image.ub 包含了哪些内容

```
mkimage -l image.ub
```

拿一个 petalinux 工程举例：

```
server@server:~/$ mkimage -l image.ub
FIT description: U-Boot fitImage for PetaLinux/4.14-xilinx-v2018.3+gitAUTOINC+eeab73d120/plnx-zynqmp
Created:         Fri May 28 15:26:34 2021
 Image 0 (kernel@1)
  Description:  Linux kernel
  Created:      Fri May 28 15:26:34 2021
  Type:         Kernel Image
  Compression:  gzip compressed
  Data Size:    7084583 Bytes = 6918.54 KiB = 6.76 MiB
  Architecture: AArch64
  OS:           Linux
  Load Address: 0x00080000
  Entry Point:  0x00080000
  Hash algo:    sha1
  Hash value:   821993b54a17d7278e01ecdd32d7af20941371fb
 Image 1 (fdt@system-top.dtb)
  Description:  Flattened Device Tree blob
  Created:      Fri May 28 15:26:34 2021
  Type:         Flat Device Tree
  Compression:  uncompressed
  Data Size:    31746 Bytes = 31.00 KiB = 0.03 MiB
  Architecture: AArch64
  Hash algo:    sha1
  Hash value:   741e90a6b586ae1848944393c5f07656f00dca9c
 Image 2 (ramdisk@1)
  Description:  petalinux-user-image
  Created:      Fri May 28 15:26:34 2021
  Type:         RAMDisk Image
  Compression:  gzip compressed
  Data Size:    6506366 Bytes = 6353.87 KiB = 6.20 MiB
  Architecture: AArch64
  OS:           Linux
  Load Address: unavailable
  Entry Point:  unavailable
  Hash algo:    sha1
  Hash value:   8e0f78ca2e6cd3d13cf3b554c164b72de6ab5fdd
 Default Configuration: 'conf@system-top.dtb'
 Configuration 0 (conf@system-top.dtb)
  Description:  1 Linux kernel, FDT blob, ramdisk
  Kernel:       kernel@1
  Init Ramdisk: ramdisk@1
  FDT:          fdt@system-top.dtb
  Hash algo:    sha1
  Hash value:   unavailable
```

例子用可以看到 image.ub 里有三个镜像，分别是  
Image 0 (kernel@1) 就是 Linux 内核  
Image 1 (fdt@system-top.dtb) 就是设备树  
Image 2 (ramdisk@1) 就是根文件系统

### 2. 提取 image.ub 里的镜像

运行命令，提取 Linux 内核，-p 0 指第 1 个镜像

```
dumpimage -T flat_dt -p 0 image.ub -o Image
```

运行命令，提取设备树，-p 1 指第 2 个镜像

```
dumpimage -T flat_dt -p 1 image.ub -o system.dtb
```

同样的方法提取根文件系统

二、修改 image.ub 里内容
-----------------

### 1. 修改设备树

可以反编译设备树文件，修改完成再编译回去

```
dtc -I dtb -O dts -o system.dts system.dtb
```

三、合成 image.ub
-------------

合成 image.ub 需要一个配置文件 its，这个配置文件在 petalinux 编译时会生成，笔者从 petalinux 提取并修改了一个

```
/dts-v1/;

/ {
        description = "U-Boot fitImage for PetaLinux";
        #address-cells = <1>;

        images {
                kernel@1 {
                        description = "Linux kernel";
                        data = /incbin/("Image");
                        type = "kernel";
                        arch = "arm64";
                        os = "linux";
                        compression = "none";
                        load = <0x80000>;
                        entry = <0x80000>;
                        hash@1 {
                                algo = "sha1";
                        };
                };
                fdt@system-top.dtb {
                        description = "Flattened Device Tree blob";
                        data = /incbin/("system-top.dtb");
                        type = "flat_dt";
                        arch = "arm64";
                        compression = "none";
                        
                        hash@1 {
                                algo = "sha1";
                        };
                };
                ramdisk@1 {
                        description = "petalinux-user-image";
                        data = /incbin/("rootfs.cpio.gz");
                        type = "ramdisk";
                        arch = "arm64";
                        os = "linux";
                        compression = "gzip";
                        
                        
                        hash@1 {
                                algo = "sha1";
                        };
                };
	};

        configurations {
                default = "conf@system-top.dtb";
                conf@system-top.dtb {
			description = "1 Linux kernel, FDT blob, ramdisk";
			kernel = "kernel@1";
			fdt = "fdt@system-top.dtb";
			ramdisk = "ramdisk@1";
			
                        hash@1 {
                                algo = "sha1";
                        };
                };
	};
};
```

通过下面命令合成：

```
mkimage -f fitimage.its image.ub
```

总结
--

通过直接修改 image.ub 免驱了再次编译 petalinux 的麻烦，特别是没有 hdf 或 xsa 的情况下。

** 本人擅长解决 FPGA、ZYNQ、ZYNQMP 中各种疑难杂症，欢迎交流！**