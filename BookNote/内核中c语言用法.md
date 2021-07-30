# 内核中c语言用法

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