# go语言学习

## 1.基本语法

### 1.变量定义

1. 标准格式 

Go 量声明格式为： 

var  	变量名	   变量类型 

变量声明 关键字 var 开头，后置变量类型，行尾无须分号。

```go
 var a 
 var b string 
 var c [] float32 
 var d func() bool 
 var e struct{ 
 x int 
 }
```

2. 批量格式 

   ```go
   var ( 
   a int 
   b string 
   c [] float32 
   d func () bool 
   e struct { 
   x int 
   	}
   }
   ```


### 2.变量赋值
1. 标准格式
    var 变量名类型＝表达式
    例如：游戏中，玩家的血量初始值为 100 。可以这样写：
    var hp nt = 100

2. 编译器推导类型的格式
    var hp = 100 
    等号右边的部分在编译原理里被称做“右值”。
    下面是编译器根据右值推导变量类型完成初始化的例子

```go
 var attack = 40 
 
 var defence = 20 
 
 var damageRate float32 = 0 . 17 
 
 var damage= float32 (attack-defence) * damageRate 
 
 fmt.Println(damage)
```

3. 短变量声明并初始化

var 的变量声 明还有 种更为精简的写法，例如： 

hp := 100  

这是 Go 语言的推导声明写法，编译器会自动根据右值类型推断出左值的对应类型。

4. 匿名变量

在使用多重赋值时，如果不需要在左值中接收变量 可以使用匿名变量。 

```go
 func GetData() (int ，int) { 
 	return 100 , 200 
 } 
 
 a , _: = GetData() 
 
 _, b : = GetData() 
 
 fmt . Println(a , b)
```

### 3.数据类型

Go 语言中有丰富的数据类型，除了基本的整型、浮点型、布尔型 字符串外，还有 

切片、结构体 函数、 map 通道（ channel ）等。 Go 语言的基本类型和其他语言大同小异，切片类型有着指针的便利性，但 比指针更为安全 很多高级语言都配有切片进行安全和高效率的内 存操作。 

结构体是 Go 语言基 的复杂类型之一，后面会用单独的一章进行讲解。 

函数也是 Go 语言的 种数据类型，可 对函数类型 变量进行赋值和获取，函数特 

性较多，将被放在独立章节讲解 。

map和切片是开发中常见的数据容器类型。

通道与并发息息相关 读者会在第 章了解通道的细节。

1. 整型。
2. 浮点型。
3. 布尔
4. 字符串
5. 字符
6. 切片-能动态分配的空间

切片是一个拥有相同类型元素的可变长度的序列 切片的声明方式如下： T是切片元素类型，可以使整型，浮点，切片，map，函数等。

```go
var name [] T 
```

```go
 a := make([]int , 3) 
 
 a [O] = 1 
 a [1] = 2 
 a [3] = 3
```

解释：1.创建类型为Int，容量为3的切片。

2. 给3个切片赋值。 

### 4.数据类型转换

Go 语言使用类型前置加括号的方式进行类型转换 一般格式如下 ：

T（表达式）

代表要转换的类型。表达式包括变量、复杂算子和函数返回值等。

注意：类型转换 ，需要考虑两种类型的关系和范围，是否会发生数值截断等 。



### 5.指针

指针概念在 Go 语言中被拆分为两个核 概念 

1.类型指针，允许对这个指针类型的数据进行修改。传递数据使用指针，而无须拷贝数据。类型指针不能进行偏移和运算。 

2.切片，由指向起始元素的原始指针、元素数量和容量组成。

ptr : = &v   

v的类型为 T, 

其中V代表被取地址的变量，被取地址V使用ptr变量进行接收， prt的类型就为"*T"， 

称做T的指针类型,"*"代表指针。

#### 1.使用指针变量获取命令行的输入信息

#### 2.创建指针的另外一种方法new()

	Go 还提供了另外 种方法来创建指针变 ，格式如下： 

new(类型 )

一般这样写： 

str := new(string)  

*str = "ninja"  

fmt.Println(*str)  

new （）函数可以创建一个对应类型的指针，创建过程会分配内存。被创建的指针指向的值为默认值。



### 6.变量的声明周期

#### 1. 栈空间

#### 2.堆空间



### 7. 字符串

#### 1.保存格式

字符串一般utf-8保存， 一个汉字占用3个字符。

#### 2.遍历字符

以ascii打印字符。

```go
theme ：＝"犯击 start"
for i := O; i < len(theme) ; i++ { 
    fmt . Printf ("ascii %c %d\n ”， theme [i] , theme[i])
}
```

打印汉字的时候就乱码。

以unicode打印字符。 

```go
theme := "狙击 start"

for _,s := range theme{

	fmt.Printf("Unicode: %c %d\n",s,s)

} 
```

就会以正常汉字打印了。

#### 3.**总结：** 

**• ASCII 字符串遍历直接使用下标** 

**• Unicode 字符串遍历用 for range** 

#### 4.字符串拼接

```go
hammer "吃我一锤"
sickle "死吧”
//声明字节缓
var stringBuilder bytes.Buffer 
//把字符串写入缓冲
stringBuilder.WriteString(hammer) 
stringBuilder.WriteString(sickle) 
//将缓冲 字符串形式输出
fmt.Println(stringBuilder.String())
```

#### 5.格式化

预留

#### 6.base64 ini文件读取写入操作等

#### 7. 别名



## **2.容器：存储和组织数据的方式**

本章将以实用为目的，详细介绍数组、切片、映射，以及列表的增加、删除、修改和遍 

历的使用方法

### 1.数组-固定大小的连续空间

1.**声明:** 

*var 数组变量名［元素数量］T*

· 数组变量名 数组声明及使用时的变量名。 

· 元素数量 数组的元素数量。可以是一个表达式，但最终通过编译期计算的结果必须是整型数值。也就是说，元素数量不能含有到运行时才能确认大小的数值 

· T 可以是任意基本类型，包括 为数组本身。但类型为数组本身时，可以实现多维数组。

```go
var team [3]string  //将 team 明为包含3个元素的字符串数组。
```

2. **初始化**

定义时初始化：

```go
var team [3]string{"hammer","soldier","mum"}
```

遍历初始化：

```go
var team [3]string
team[0] = "hammer"
team[1] = "soldier" 
team[2] = "mum" 

for k,v := range team { 
	fmt.Println(k , v) 
}
```

### 2.切片(slice)-动态分配大小的连续空间

### 3.映射(map)-建立食物关联的容器（无序的）

1. 一般用法

```go
//使用时， 需要手动使用 make创建。如果不创建使用map类型，会触发岩机错误。
scene := make(map[string]int)  
//向map中加入映射关系，写法和数组一样，key可以是除函数外的任意类型。
scene ["route"] = 66 
//查找map中的值。
fmt.Println(scene["route"])
//查找一个不存在的值，会返回ValueType的默认值
v := scene["route2"] 
fmt.Println(v)
```

2. 判断值是否在map中有固定用法：

```go
v,ok := scene ["route"]
```

在默认获取键值的基础上，多取了一个变量 ok 可以判断键 route 是否存在于 map 中。

3. 声明时填充内容的方式：

```go
m := map[string]string{
	"W":"forward", 
	"A":"left", 
	"D":"right", 
	"S":"backward",
}
```

这种情况可以不是用make。 

4. 遍历map。  

可是使用for range 



5. 需要在多进程或者多线程中使用的map——sync.Map

### 4.列表（list)-可以快速增删的非连续性空间的容器

1. 双链表支持从队列前方或后方插入元素，分 对应的方法是 PushFront和PushBack。
2. 初始化列表（定义）

通过container/list 包的New方法初始化list 

```go
变量名 := list.New()
```

通过声明初始化list 

```go
var 变量名 list.List
```

3. 添加元素

```go
//创建一个列表示例
l := list.New()
//将fist字符串插入到列表尾部，此时列表是空的，插入后只有一个元素。
l.PushBack("fist")
//将67放入列表，67将被放在fist的前面。
l.PushFront(67)
```

4. 还有很多方法后边研究。

## 3.流程控制



## 4.函数

1. 声明形式

```go
func 函数名（参数列表）（返回参数列表）｛
	函数体
}

func foo ( a int, b string ) int
//参数类型简写， 返回int类型
func add (a,b int ）int { 
	return a + b 
}
```

2. 函数变量

Go 语言中， 函数也是一种类型，可以和其他类型一样被保存在变量中。下面的代码定义了一个函数变量f,并将一个个函数名 fire()赋给函数变量f，这样调用函数变量f时，实际调用的就是fire()函数，代码如下：

```go
package main 

import (
	"fmt"
)

func fire() { 
	fmt.Println("fire") 
} 

func main() { 
	var f func() 
	f = fire 
	f () 
}
```



