<!--
 * @Description: 
 * @Version: 2.0
 * @Autor: lsh
 * @Date: 2021-04-26 11:18:59
 * @LastEditors: lsh
 * @LastEditTime: 2021-11-19 16:44:24
-->
# Linux-0.12
Linux0.12内核代码中文注释 + 在高版本GCC 5.4.0 + Ubuntu 16.04

编译结果如下:

![image](https://github.com/sky-big/Linux-0.12/blob/master/compile_picture.jpg)


运行结果如下:

![image](https://github.com/sky-big/Linux-0.12/blob/master/run_result.jpg)

运行环境下载地址:
http://oldlinux.org/Linux.old/bochs/linux-0.12-080324.zip

gcc版本必须低于4.0

运行命令:
qemu-system-i386 -fda xxx.img   
表示文件当做镜像
qemu-system-i386 -hda xxx.img
表示一个软盘镜像


编译之前安装了nasm编译器，还有其他啥东西，忘了，如果make时报啥错，就安装哪个编译器吧。