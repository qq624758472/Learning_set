# 内核中c语言用法



## \_\_attribute__

​	GNU C 的一大特色就是\_\_attribute__ 机制。\_\_attribute__ 可以设置函数属性（Function Attribute ）、变量属性（Variable Attribute ）和类型属性（Type Attribute ）。

```sh
#书写特征是：前后都有两个下划线，并切后面会紧跟一对原括弧，括弧里面是相应的__attribute__ 参数。
	__attribute__ 

#语法格式为：
	__attribute__ ((attribute-list))
```





示例代码：

https://github.com/qwe624758472/Learning_set/tree/master/Language/c%2B%2B/c

```c
static void __attribute__((constructor)) do_qemu_init_ ## function(void)    \
{                                                                           \
    register_module_init(function, type);                                   \
}
```

​	如果函数被设定为constructor属性，则该函数会在main（）函数执行之前被自动的执行；

​	若函数被设定为destructor属性，则该函数会在main（）函数执行之后或者exit（）被调用后被自动的执行。

解析：

​	1.从







## __read_mostly的使用

定义：

```c
#define __read_mostly  __attribute__((__section__(".data..read_mostly")))
```

使用：

```c
static unsigned int d_hash_shift __read_mostly;
```

也就是说被__read_mostly 形容的数据别放入到了段. data.read_mostly 中

那这个段的作用是什么呢？

定义到这个段的数据，再 linux 内核被加载时，数据会自动被存放到 cache 中，以提高整个系统的执行效率。另一方面，如果所在的平台没有 cache，或者有 cache，但是并没有提供存放数据的接口 (也就是不允许人工放置数据在 cache 中)，这样定义为__ead_mostly 类型的数据将不能存放在 linux 内核中，甚至也不能被加载到系统内存去执行，将造成 linux 内核启动失败。

**当不能存放数据时，解决方法:**



1，修改 inclue/asm/cache.h 中的__read_mostly 定义为空的，如:__

```c
__define __read_mostly 
```

2，修改 arch/mips/kernel/vmlinux.s，将. data.read_mostly 段的位置定义到实际内存空间中去。









# 内核中数据结构

## 哈希表（散列表）

1.概述

2.功能

3.构造方法

要计算简单， 地址分布均匀(散列)



4.处理冲突的方法

* 开放定址法：一旦发生冲突，寻找下一个空的散列地址。
  * 线性探测法：
  * 随机探测法：





## 红黑树













































