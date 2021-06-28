> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [mp.weixin.qq.com](https://mp.weixin.qq.com/s/dp3RyBpdMsof8enrb8XdVQ)

击上方 “**果果小师弟**”，选择 “**置顶 / 星标公众号**”

干货福利，第一时间送达！


================

![](https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPiaJQXWGyC9wrUzIicibgXayrgibTYarT3A1yzttbtaO0JlV21wMqroGYT3QtPq2C7HMYsvicSB2p7dTBg/640?wx_fmt=gif)
===========================================================================================================================================

![](https://mmbiz.qpic.cn/mmbiz_jpg/f80N5WPNTbz5moibwljZDkPpL01afsmnibAIArId5A5ibcrsx7mZ8XMPh5tfUR89vqUcYwTicjG1AMJQAyYVJmiaYxg/640?wx_fmt=jpeg)

**摘要**：俗话说万事开头难，刚开始的时候，你是否根本就不知如何开始，上网查资料被一堆堆新名词搞的找不到北，去图书馆看书也是找不到方向? 又是 arm，又是 linux，又是 uboot 头都大了，不知道自己究竟从哪里开始？

下面就跟着我学习设计嵌入式工程师的修炼日记吧！

简单说，从 arm 基础知识到裸机编程，从 uCOSII 到 linux，从 linux 基础的命令到 shell 编程，从 u-boot 到文件系统，最后到现在的 linux 驱动程序。从简单到复杂，从基础到高级基本是按照这个来的。

0x01 嵌入式硬件基础以及裸机程序开发
====================

1. 知识点
------

（1） arm 处理器基础知识，工作模式，寄存器，中断与异常，寻址方式，汇编指令集

（2） 熟悉 ADS 集成开发环境为裸机程序开发做准备

（3）裸机程序开发：LED，串口，LCD，触摸屏，NANDFLASH，IIC，IIS，DMA 等

2. 过程以及目的
---------

这一阶段的学习主要是熟悉嵌入式的硬件，为以后的 linux 驱动打基础。因为嵌入式主打处理器架构就是 arm, 所以针对 arm 来学习。首先学习基础知识，然后通过在开发板上的练习来掌握。这一阶段所用的资料主要有：

（1） 《ARM 嵌入式基础教程》 这本书主要看 arm 基础的知识。这本书是讲 arm7 的，但是 arm 的基础的指令集以及模型是一样的。

（2） 《嵌入式 linux 应用开发完全手册》 这本书主要看介绍 s3c2410 接口模块的部分，实验裸机的时候需要硬件的知识。

3. 所用时间
-------

大约两个月。

0x02 linux 基础以及 shell 编程
========================

1. 知识点
------

（1） linux 基础知识

（2） linux 安装与使用

（3） linux 常用命令

（4） vi 使用

（5） 简单 shell 编程

2. 过程以及目的
---------

这一阶段的学习主要是熟悉 linux，会安装 linux，了解 Linux 的基础知识，会简单的操作。刚开始安装 linux 还是在虚拟机上安装比较好, 安装开发板推荐的 Fedora9，按照开发板配套的说明一步一步的安装，然后在上面练习各种命令。

这一阶段主要要看《鸟哥的 linux 私房菜基础学习篇》，这个书有电子版，也不用全看，主要是看前面对 Linux 基础知识的描述，以及基本的命令。命令只是了解，那么多记也记不下来，以后用时再查阅。这一阶段主要目的是对 linux 有一个整体的了解，最起码要知道基础的一些概念，还有会一些基本的操作。

3. 时间
-----

大约一个月。

0x03 linux 应用程序基础开发
===================

1. 知识点
------

（1） GCC，GDB，Make 基础

（2） 文件，时间编程

（3） 多进程，多线程程序设计

（4） 进程间通信

2. 过程以及目的
---------

这一阶段的学习主要是了解 linux 应用程序的编写调试，也都是基础知识，主要目的还是熟悉 Linux。熟悉其中应用程序有关的一些概念，为以后打基础。这一阶段主要看了《嵌入式 Linux 应用程序开发标准教程》，这本书对 linux 上应用程序编写讲的很详细，而且不是很难，那本最经典的《UNIX 高级环境编程》讲的比较深入，刚开始看有点难，万事开头难还是从简单的开始。学习应用编程以后也不一定搞应用，从事嵌入式 linux 行业的人，这是必须的基本技能。

3. 时间
-----

大约一个月

0x04 uCOSII 操作系统学习
==================

1. 知识点
------

（1）嵌入式实时操作系统的概念以及基础知识

（2）uCOSII 操作系统源代码阅读

（3）移植 uCOSII

（4）uCOSII 操作系统应用编程

2. 过程以及目的
---------

这一阶段的学习主要是了解嵌入式实时操作系统的基础，因为 uCOSII 是开源的，可以研究他的代码。对操作系统的任务调度以及资源管理有直观的认识。并且阅读他的源码能够提高自己的编程能力，规范自己的编程习惯。虽然提供了 uCOSII 的移植版本，但是自己实际移植一次能够有更深刻的认识。

这个操作系统比较简单，所以应用程序的编写就很重要，在上面实现几个功能还是有助于对系统的理解。这一阶段主要看的就是邵贝贝翻译的 uCOSII 作者所著的《嵌入式实时操作系统 uCOS-II》, 里面对操作系统的源码有详细的分析。看透这本书后就会对 uCOSII 有一个深入的了解。

这一阶段的主要目的是了解操作系统的知识，比如任务调度，并发与竞态。还有一个目的就是锻炼自己的编程能力。因为如果看介绍操作系统原理的书会比较枯燥，我感觉阅读一个操作系统的源码比看一千本介绍原理的书来的实惠的多。

3. 时间
-----

大约一个月

0x05 bootloader 学习
==================

1. 知识点
------

（1）bootloader 基础知识 

（2）u-boot 基础与命令 

（3）u-boot 移植

2. 过程以及目的
---------

bootloader 是一个 linux 嵌入式系统的不可缺少的部分。这一阶段的学习主要是了解 bootloader 的作用，以及通用 bootloader---u-boot 的知识，刚开始学会用 u-boot 的各种命令。然后分析 u-boot 的启动流程和 Makefile 以及 u-boot 加载内核过程。

然后按照别人的步骤一步一步的移植内核，多做几遍以熟悉这个过程。然后在熟悉整个 u-boot 的基础上，独自完成 u-boot 的移植，并添加自己修改。这一阶段主要参考资料就是光盘自带的 u-boot 移植参考手册和一些网络上的文章。个人感觉 u-boot 移植这方面的网络资源相当的丰富。

3. 时间
-----

大约一个月

0x06 文件系统学习
===========

1. 知识点
------

（1）linux 文件系统的种类与作用

（2）熟悉 busybox 的使用，以及 linux 系统启动过程

（3）构建自己的根文件系统

2. 过程以及目的
---------

文件系统也是一个 linux 嵌入式系统不可缺少的部分，这一阶段的学习主要是熟悉根文件系统的作用以及基础知识，并且自己构建自己的根文件系统。这一阶段所看的书有《构建嵌入式 linux 系统》，这本书详细讲解了嵌入式 linux 的所有元素，包括文件系统。

这里主要看文件系统这块。我还参考了网上的一些文章。这里首先要熟悉基本的一些 Linux 文件系统, 然后学习 busybox 的使用以及 linux 的启动过程，最后参考别人的一步一步来构建自己的文件系统。

3. 时间
-----

大约半个月

0x07 嵌入式图形界面 GUI
================

1. 知识点
------

（1）嵌入式图形界面 GUI 的种类与特点

（2）minigui 基础知识

（3）minigui 移植与程序编写

2. 过程以及目的
---------

嵌入式图形界面 GUI 作为嵌入式上层系统的东西也是需要了解的，这里我只学习了 minigui，因为 QT 是需要 C++ 的知识，而我没有学过 C++，所以只看了 minigui 的相关知识。这个阶段的目的主要是熟悉一下嵌入式图形接界面的开发流程，因为以后也不打算搞应用。

只是熟悉一下嵌入式组成的一些方面。这里只要将 minigui 移植到板子上能跑 hello 程序就可以了。其他的也不用深入了解。通过移植 minigui 对 linux 上运行的软件以及函数库，程序运行原理，编译器都会有一个新的认识。

3 时间
----

大约半个月

0x08 linux 驱动程序基础知识
===================

1. 知识点
------

（1）看完《LINUX 设备驱动程序》除了网络驱动程序，tty 驱动程序，块驱动程序这几章不看，其他的都要看完。

（2）实验书中所有例程，要仔细分析结果，理解相关知识点。

（3）就算是例子，程序编程习惯一定要统一，文档一定要规范。

2. 过程以及目的
---------

这一阶段的目的就是为了 linux 驱动打基础，《LINUX 设备驱动程序》（第三版）也就是大家所说的 ldd3，是最经典的一本讲 linux 驱动的书，一定要看而且多看几遍。第一次看很难看进去，感觉讲的晦涩难懂。但是多看几遍就会由豁然开朗的感觉。看完了书，然后就是实验书中的例子。

3. 时间
-----

大约两个月

0x09 linux 驱动实战
===============

1. 知识点
------

（1）通过 led 驱动理解杂项字符设备的注册与实现，并掌握 MINI2440 的 IO 操作过程。

（2）通过 button 驱动程序掌握基本的中断实现方法。

（3）掌握 PWM 驱动程序。

（4）通过触摸屏驱动程序掌握 input 设备模型的实现。

（5）掌握 ADC 驱动程序的设计

（6）掌握看门狗, rtc 驱动程序的设计

（7）通过 LCD 驱动程序分析 platform 设备模型的实现。

2. 过程以及目的
---------

这一阶段的学习主要是掌握实际的简单的驱动程序的实现与 ldd3 中的差别，并且分析一些 linux 子系统。通过这一阶段的学习，能够对 linux 驱动程序有更深入的认识。并且锻炼自己阅读海量代码的能力。这一阶段主要工作就是看代码，实验，看代码，实验，通过现象学习知识。

3. 时间
-----

大约一个月

0x0A 深入理解 linux 内核与 USB 子系统
===========================

前九个阶段完成了，自己感觉嵌入式 Linux 应该应该算是入门了。所以要开始计划第十个阶段了。这一阶段基本上要确定自己未来的发展方向了。因为现在嵌入式系统的标准接口都有 USB，所以学会 USB 的相关知识是很有竞争力的。搞驱动的人对内核应该有一定的了解，所以《深入理解 Linux 内核》还是必须的。

这一阶段主要是学习 USB 协议，阅读 linux USB 子系统全部的代码，通过阅读代码加深对 USB 的理解，随后就是熟悉各种 USB 类协议，编写和移植 USB 驱动了。USB 是我确定的方向，因为我想搞什么东西都要专一，什么都搞，什么也搞不好。所以我决定学习 USB。决定以后从事嵌入式有关 USB 方面的工作。这一阶段的学习正在进行中，现在正在 linux USB 子系统浩瀚的代码中漫游。

![](https://mmbiz.qpic.cn/mmbiz_png/YzxdmbhkVlJyKvnTK0WUeqb3dkB8CUTY8tzMX3SrteoTvjrPdC8bv7Fic0D8xBCDKL0RTQXjYqKibFq3Zia8rWKhQ/640?wx_fmt=png)

End

推荐好文  点击蓝色字体即可跳转

☞[【收藏】烂大街的 ESP82666 该咋玩](http://mp.weixin.qq.com/s?__biz=Mzg3MjU4NzI2NA==&mid=2247493480&idx=1&sn=8538c9d5a12041dee042bed2b8d960ea&chksm=ceefae07f9982711e4d1c92f30dbab86540945cf97c9f1bb8e164081b9c3f954a3c74033dc5c&scene=21#wechat_redirect)![](https://mmbiz.qpic.cn/mmbiz_gif/EhBibrVVlmvSUia2CdHcydLGJy7qHhLklSbyOhAibp1rUnETq6COIyxIt7ogtTkBI2ibGFJ2dfiaB33eZzBsia2n2cicw/640?wx_fmt=gif)[  
](http://mp.weixin.qq.com/s?__biz=Mzg3MjU4NzI2NA==&mid=2247493518&idx=1&sn=ce42066b37cbc4513a0d642caf82e353&chksm=ceefaee1f99827f7722e7640b2a778d0a05750d6043987ef8f90ec623c882df8587eaf29cf8d&scene=21#wechat_redirect)

[☞【鸡汤】一个普通人的大学四年](http://mp.weixin.qq.com/s?__biz=Mzg3MjU4NzI2NA==&mid=2247493523&idx=1&sn=11a0fc3816d2f3258aec50225e4d8d70&chksm=ceefaefcf99827eacd6de5f8dce50f60cd61d87866da174b444823e15503fbb1218c4a154350&scene=21#wechat_redirect)

[☞【干货】](http://mp.weixin.qq.com/s?__biz=Mzg3MjU4NzI2NA==&mid=2247493520&idx=1&sn=b83a4ef85638159f947e112003ff4e5c&chksm=ceefaefff99827e93bcd55d9a11560697d562f62cdfd04fffde523b713968b3ac8bfc704e03a&scene=21#wechat_redirect)[基于 STM32 的 PS2 遥控小车](http://mp.weixin.qq.com/s?__biz=Mzg3MjU4NzI2NA==&mid=2247493723&idx=1&sn=e3aa53a96871791020b3a331a6800627&chksm=ceefa134f9982822be4c16f1a56c35df50da45bc14f2b7f264bdea10e5cc7e6e4cdae0dfe219&scene=21#wechat_redirect)

[☞【干货】手把手教你写一个安卓 APP](http://mp.weixin.qq.com/s?__biz=Mzg3MjU4NzI2NA==&mid=2247495648&idx=1&sn=2ec0e12950f8b5690e1368fab58dac79&chksm=ceefa68ff9982f99bf8309a19c43cb07f68d492ea9d9ac4e38b8ca692fd62bd4a65bfac5489b&scene=21#wechat_redirect)

[☞【干货】手把手教你写单片机的指针](http://mp.weixin.qq.com/s?__biz=Mzg3MjU4NzI2NA==&mid=2247495813&idx=1&sn=b2b8c11274522184e6df2b9ca5bd8286&chksm=ceefb9eaf99830fc620d1e29ced669d20eb38e00ca9e807aff440a0133198f641f336b1c95cb&scene=21#wechat_redirect)

[☞【干货】手把手教你写单片机的结构体](http://mp.weixin.qq.com/s?__biz=Mzg3MjU4NzI2NA==&mid=2247495892&idx=1&sn=e2cd9f521a20e21663bb4ab0b08e0d78&chksm=ceefb9bbf99830adaa3e2fb2ec24846aa1bb8a8e6ba19398df02d3160e0690d62f968072f1c1&scene=21#wechat_redirect)

欢迎**转发、留言、点赞、分享**，感谢您的支持！  

公众号