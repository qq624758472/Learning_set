<!--
 * @Description: 此目录为c语言链接脚本功能
 * @Version: 2.0
 * @Autor: lsh
 * @Date: 2022-02-25 19:28:06
 * @LastEditors: lsh
 * @LastEditTime: 2022-02-25 20:09:57
-->
# 链接脚本介绍
具体教程在../../../BookNote/转载的一些文章/Linux下的lds链接脚本详解.pdf

1.通过命令”ld --verbose”获取默认链接脚本：
2.我们截取分割线”=====“之间的链接脚本保存为：ldscript.lds
3.在.bss的数据段前添加了自定义的数据段：
```c

_init_start = .;

.myinit : { *(.myinit) }

_init_end = .;
```


4.”_init_start“和”_init_end“是我们用于识别数据段开始和结束的在链接脚本中定义的变量，而.myinit则是数据段的名称，其中：
.myinit : { *(.myinit) }：表示.o中的.myinit数据段（输入段）保存到bin中的.myinit数据段（输出段）中

5.
执行：gcc -c section.c -o section.o 编译应用源码。
执行：readelf -S section.o 查看段信息.
可以看到，段[6]是我们自定义的数据段.

6.
执行：gcc -T ldscript.lds section.o -o section 链接成可执行的bin文件
执行：readelf -S section 查看bin文件的段分布情况
在我链接成的可执行bin中，在[25]段中存在我们自定义的段
