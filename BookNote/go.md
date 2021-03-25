# 菜鸟教程GO语言

## 1.基本语法(基本数据)

### 1.1变量定义

#### 1.1.1 标准格式 

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

#### 1.1.2 批量格式 

```go
var ( 
a int 
b string 
c [] float32 
d func () bool 
e struct { 
x int 
}
)
```


### 1.2变量赋值

#### 1.2.1 标准格式

var 变量名类型＝表达式
例如：游戏中，玩家的血量初始值为 100 。可以这样写：

```go
var hp int = 100
```

#### 1.2.2 编译器推导类型的格式

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

#### 1.2.3 短变量声明并初始化

var 的变量声 明还有 种更为精简的写法，例如： 

hp := 100  

这是 Go 语言的推导声明写法，编译器会自动根据右值类型推断出左值的对应类型。

#### 1.2.4 匿名变量

在使用多重赋值时，如果不需要在左值中接收变量 可以使用匿名变量。 

```go
 func GetData() (int ，int) { 
 	return 100 , 200 
 } 
 
 a , _: = GetData() 
 
 _, b : = GetData() 
 
 fmt . Println(a , b)
```

### 1.3数据类型

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

### 1.4数据类型转换

Go 语言使用类型前置加括号的方式进行类型转换 一般格式如下 ：

T（表达式）

代表要转换的类型。表达式包括变量、复杂算子和函数返回值等。

注意：类型转换 ，需要考虑两种类型的关系和范围，是否会发生数值截断等 。



### 1.5指针

指针概念在 Go 语言中被拆分为两个核 概念 

1.类型指针，允许对这个指针类型的数据进行修改。传递数据使用指针，而无须拷贝数据。类型指针不能进行偏移和运算。 

2.切片，由指向起始元素的原始指针、元素数量和容量组成。

ptr : = &v   

v的类型为 T, 

其中V代表被取地址的变量，被取地址V使用ptr变量进行接收， prt的类型就为"*T"， 

称做T的指针类型,"*"代表指针。

#### 1.5.1使用指针变量获取命令行的输入信息

#### 1.5.2创建指针的另外一种方法new()

	Go 还提供了另外 种方法来创建指针变 ，格式如下： 

new(类型 )

一般这样写： 

str := new(string)  

*str = "ninja"  

fmt.Println(*str)  

new （）函数可以创建一个对应类型的指针，创建过程会分配内存。被创建的指针指向的值为默认值。



### 1.6变量的声明周期

#### 1.6.1 栈空间

#### 1.6.2 堆空间



### 1.7 字符串

#### 1.7.1 保存格式

字符串一般utf-8保存， 一个汉字占用3个字符。

#### 1.7.2 遍历字符

以ascii打印字符。

```go
theme ：＝"狙击 start"
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

#### 1.7.3 **总结：** 

**• ASCII 字符串遍历直接使用下标** 

**• Unicode 字符串遍历用 for range** 

#### 1.7.4 字符串拼接

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

#### 1.7.5 格式化

预留

#### 1.7.6 base64 ini文件读取写入操作等

#### 1.7.7 别名



## **2.容器：存储和组织数据的方式**

本章将以实用为目的，详细介绍数组、切片、映射，以及列表的增加、删除、修改和遍 

历的使用方法

### 2.1 数组-固定大小的连续空间

#### 2.1.1 声明: 

*var 数组变量名［元素数量］T*

· 数组变量名 数组声明及使用时的变量名。 

· 元素数量 数组的元素数量。可以是一个表达式，但最终通过编译期计算的结果必须是整型数值。也就是说，元素数量不能含有到运行时才能确认大小的数值 

· T 可以是任意基本类型，包括 为数组本身。但类型为数组本身时，可以实现多维数组。

```go
var team [3]string  //将 team 明为包含3个元素的字符串数组。
```

#### 2.1.2 **初始化**

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

### 2.2 切片(slice)-动态分配大小的连续空间

### 2.3 映射(map)-建立食物关联的容器（无序的）

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

### 2.4 列表（list)-可以快速增删的非连续性空间的容器

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

### 4.1 声明形式

```go
func 函数名（参数列表）（返回参数列表）｛
	函数体
}

func foo ( a int, b string ) int
//参数类型简写， 返回int类型
func add (a,b int ）int { 
	return a + b 
}

//返回多个值
func swap(x, y string) (string, string) {
   return y, x
}        
```

### 4.2 函数变量

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

### 4.3 匿名函数

定义: 匿名函数就是没有名字的普通函数定义. 

```go
func (参数列表) (返回参数列表){
	函数体
}
```

4.3.1 在定义时直接调用匿名函数

```go
func(data init) { 
	fmt.Println (”hello”, data) 
} (100)
```

4.3.2 将匿名函数赋值给变量

```go
//将匿名函数体保存到f()中
f := func(data int){
    fmt.Println("hello",data)
}
//使用f()调用
f(100)
```

4.3.3 匿名函数作为回调函数

```go
//对切片进行遍历操作,遍历中访问每个元素的操作使用匿名函数来实现

//遍历切片的每个元素,通过给定函数进行元素访问
func visit(list []int, f func(int)){
    for _,v := range list{
        f(v)
    }
}

func main() {
    //使用匿名函数打印切片内容
    visit([]int{1,2,3,4}, func(v int){
        fmt.Println(v)
    })
}
```

4.3.4 使用匿名函数实现操作封装



### 5.函数类型实现接口



### 6.闭包

闭包＝函数＋引用环境

### 7.可变参数

```go
//v 为可变参数变量, 类型为[]T, 也就是拥有多个T元素的T类型切片.
//T为可变参数类型,当T为interface{}时,传入的可以是任意类型.
func 函数名 (固定参数列表, v ... T) (返回参数列表){
    函数体
}
```

7.1 所有参数都是可变参数:

```go
//打印函数定义如下:
func Println(a ... interface{}) (n int, err error){
    //略
}

//调用时,参数可以是任意类型.
fmt.Println(5,"hello". &struct{a int}{1}, true)
```

7.2 部分参数为可变参数

```go
func Printf(format string, a ...interface{}) (n int, err error){
    //略
}

//调用时,第一个参数必须为string
fmt.Printf("pure string\n")
fmt.Printf("value: %v %f\n",true, math.Pi)
```



### 8.延迟执行语句

​	Go 言的 defe 语句会将其后面跟随的语句进行延迟处理。在 defer 归属的函数即将返回时，将延迟处理的语句按 defer 的逆序进行执行，也就是说，先被 defer 的语句最后被执行，最后被 defer 的语旬，最先被执行。

（内心独白：好鸡肋的作用，按c语言编程习惯，这个就是个无用的东西．）

8.1使用延迟语句在函数退出时释放资源.

### 9.处理运行时发生的错误

### 10.宕机(panic)

10.1 手动触发宕机

```go
func main(){
    panic("crash")
}
//代码运行崩溃并输出如下:
panci: crash
goroutine 1 [running]:
main.main()
main.go:5 +0x6b
```

10.2 在运行依赖的必备资源时主动触发宕机

10.3 在宕机时触发延迟执行语句

```go
func main(){
    defer fmt.Println("宕机后要做的事情1")
    defer fmt.Println("宕机后要做的事情2")
    panic("宕机")
}
```

### 11.宕机恢复







## 5.结构体

定义: 一个复合类型.  没有类等面向对象的概念.

```go
type 类型名 struct{
    字段1   字段1的类型
    字段2   字段2的类型
    ...
}
```

#### 5.1 实例化结构体

```go
var test T
```

T为结构体类型.  test为结构体的实例.

5.1.1 创建指针类型的结构体



## 6.HTTP服务器

### 1.HTTP协议简介

超文本传输协议（英文：**H**yper**T**ext **T**ransfer **P**rotocol，缩写：HTTP）是一种用于分布式、协作式和超媒体信息系统的应用层协议。

就是常用的浏览器和服务器通信协议. 将tcp进行封装后的一种应用层协议.

### 2.HTTP工作原理

HTTP协议定义Web客户端如何从Web服务器请求Web页面，以及服务器如何把Web页面传送给客户端。HTTP协议采用了请求/响应模型。客户端向服务器发送一个请求报文，请求报文包含请求的方法、URL、协议版本、请求头部和请求数据。服务器以一个状态行作为响应，响应的内容包括协议的版本、成功或者错误代码、服务器信息、响应头部和响应数据。

以下是 HTTP 请求/响应的步骤：

1. 客户端连接到Web服务器
   一个HTTP客户端，通常是浏览器，与Web服务器的HTTP端口（默认为80）建立一个TCP套接字连接。例如，[http://www.baidu.com](http://www.baidu.com/)。  默认80端口是不显示的, 否则需要在域名后加入端口号.

2. 发送HTTP请求
   通过TCP套接字，客户端向Web服务器发送一个文本的请求报文，一个请求报文由请求行、请求头部、空行和请求数据4部分组成。

3. 服务器接受请求并返回HTTP响应
   Web服务器解析请求，定位请求资源。服务器将资源复本写到TCP套接字，由客户端读取。一个响应由状态行、响应头部、空行和响应数据4部分组成。

4. 释放连接TCP连接
   若connection 模式为close，则服务器主动关闭TCP连接，客户端被动关闭连接，释放TCP连接;若connection 模式为keepalive，则该连接会保持一段时间，在该时间内可以继续接收请求;

5. 客户端浏览器解析HTML内容
   客户端浏览器首先解析状态行，查看表明请求是否成功的状态代码。然后解析每一个响应头，响应头告知以下为若干字节的HTML文档和文档的字符集。客户端浏览器读取响应数据HTML，根据HTML的语法对其进行格式化，并在浏览器窗口中显示。

例如：在浏览器地址栏键入URL，按下回车之后会经历以下流程：

1. 浏览器向 DNS 服务器请求解析该 URL 中的域名所对应的 IP 地址;
2. 解析出 IP 地址后，根据该 IP 地址和默认端口 80，和服务器建立TCP连接;
3. 浏览器发出读取文件(URL 中域名后面部分对应的文件)的HTTP 请求，该请求报文作为 TCP 三次握手的第三个报文的数据发送给服务器;
4. 服务器对浏览器请求作出响应，并把对应的 html 文本发送给浏览器;
5. 释放 TCP连接;
6. 浏览器将该 html 文本解析并显示对应格式的内容; 

### 3.HTTP请求方法

HTTP/1.1协议中共定义了八种方法来以不同方式操作指定的资源：

GET

向指定的资源发出“显示”请求。使用GET方法应该只用在读取数据，而不应当被用于产生“副作用”的操作中，例如在Web Application中。其中一个原因是GET可能会被网络蜘蛛等随意访问。

HEAD

与GET方法一样，都是向服务器发出指定资源的请求。只不过服务器将不传回资源的本文部分。它的好处在于，使用这个方法可以在不必传输全部内容的情况下，就可以获取其中“关于该资源的信息”（元信息或称元数据）。

POST

向指定资源提交数据，请求服务器进行处理（例如提交表单或者上传文件）。数据被包含在请求本文中。这个请求可能会创建新的资源或修改现有资源，或二者皆有。

PUT

向指定资源位置上传其最新内容。

DELETE

请求服务器删除Request-URI所标识的资源。

TRACE

回显服务器收到的请求，主要用于测试或诊断。

OPTIONS

这个方法可使服务器传回该资源所支持的所有HTTP请求方法。用'*'来代替资源名称，向Web服务器发送OPTIONS请求，可以测试服务器功能是否正常运作。

CONNECT

HTTP/1.1协议中预留给能够将连接改为管道方式的代理服务器。通常用于SSL加密服务器的链接（经由非加密的HTTP代理服务器）。

### 4.HTTP请求格式

![](../tools_Lib/all_picture/内核笔记/57.jpg)

![](../tools_Lib/all_picture/内核笔记/58.png)

### 5.HTTP响应格式

![](../tools_Lib/all_picture/内核笔记/59.jpg)

![60](../tools_Lib/all_picture/内核笔记/60.png)

### 6.HTTP路由转发

路由是处理url和函数关系的程序,配置一系列的url访问规则,提供对应url访问的模板.在go的原生net/http包中,浏览器发起http请求(post,get等请求),到服务器中,服务器根据发送的url查找对应的出来程序,此查找程序为路程转发程序.

### 7.HTTP状态码

所有HTTP响应的第一行都是状态行，依次是当前HTTP版本号，3位数字组成的状态代码，以及描述状态的短语，彼此由空格分隔。

状态代码的第一个数字代表当前响应的类型：

- 1xx消息——请求已被服务器接收，继续处理
- 2xx成功——请求已成功被服务器接收、理解、并接受
- 3xx重定向——需要后续操作才能完成这一请求
- 4xx请求错误——请求含有词法错误或者无法被执行
- 5xx服务器错误——服务器在处理某个正确请求时发生错误

有些是已经定义好的,比如200, 404等常见状态码,开发者也可以自行定义.

### 8.一个简单的HTTP服务器

```go
package main
 
import (
    "fmt"
    "net/http"
)
 
func IndexHandler(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintln(w, "hello world")
}
 
func main() {
    http.HandleFunc("/", IndexHandler)
    http.ListenAndServe("127.0.0.0:8000", nil)
}
```

 上述代码中分析请求格式如下图所示:

![](../tools_Lib/all_picture/内核笔记/61.jpg)

可以看到上图,请求头,响应头,请求方法为GET, 响应状态码为200.

Content-Type: 传输的媒体类型.

Content-Length: HTTP消息长度.

等字段内容庞大,后续专门开一贴介绍.

### 9.HTTP和MYSQL开发登录功能

一个简单的登录页面,用户登录时填写的账号密码从数据库中读出,并做对比

index.html:  主页显示内容

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>index</title>
</head>
<body>
欢迎访问golang搭建的HttpServer
</body>
</html>
```

login.html: 登录页面

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>用户登录</title>
</head>
<body>
<form action="/loginForm" method="post">
    <span>{{.}}</span>
    username:<input type="text" name="username"/><br/>
    password:<input type="password" name="password"/><br/>
    <input type="submit" value="登录"/>&nbsp;&nbsp;
    <input type="reset" value="重置"/>
</form>
</body>
</html>
```

success.html: 登录成功页面

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>首页</title>
</head>
<body>
欢迎访问golang搭建的HttpServer
</body>
</html>
```

main.go: 服务器主程序

```go
package main

import (
"database/sql"
"encoding/json"
"fmt"
_ "github.com/go-sql-driver/mysql"
"html/template"
"io/ioutil"
"net/http"
"strconv"
)

//数据库操作可以面向接口来设计，我这里只是简单写一下哈
var Db *sql.DB

func init() { //初始化db实例
	var err error
	dsn := "root:000000@tcp(127.0.0.1:3306)/test"
	Db, err = sql.Open("mysql", dsn)
	if err != nil {
		fmt.Printf("init db instance falied:%v\n", err)
		return
	}
}

type User struct {
	Id       int    `db:"id",json:"id"`
	Username string `db:"username",json:"username"` //sql.NullString
	Password string `db:"password",json:"password"` //sql.NullString
	Age      int    `db:"age",json:"age"`           //sql.NullInt64
	Sex      string `db:"sex",json:"sex"`           //sql.NullString
	Phone    string `db:"phone",json:"phone"`       //sql.NullString
}

/**
 *添加用户
 */
func addUser(user *User) int64 {
	sqlStr := "INSERT INTO user_tb(username,password,age,sex,phone) VALUES(?,?,?,?,?)"
	stmt, err := Db.Prepare(sqlStr)
	if err != nil {
		fmt.Printf("Prepare insert sql failed:%v\n", err)
		return -1
	}
	defer closeStmt(stmt)
	result, err := stmt.Exec(user.Username, user.Password, user.Age, user.Sex, user.Phone)
	if err != nil {
		fmt.Printf("exec sql failed:%v\n", err)
		return -1
	}
	lastId, err := result.LastInsertId()
	if err != nil {
		fmt.Printf("get lastId failed:%v\n", err)
		return -1
	}
	return lastId
}

//更新用户
func updateUser(u User) int64 {
	updateSql := "UPDATE user_tb SET username=?,password=?,age=?,sex=?,phone=? where id=?"
	stmt, err := Db.Prepare(updateSql)
	if err != nil {
		fmt.Printf("Prepare update sql failed:%v\n", err)
		return -1
	}
	defer closeStmt(stmt)
	result, err := stmt.Exec(u.Username, u.Password, u.Age, u.Sex, u.Phone, u.Id)
	if err != nil {
		fmt.Printf("exec sql failed:%v\n", err)
		return -1
	}
	affectedId, err := result.RowsAffected()
	if err != nil {
		fmt.Printf("get affected id failed:%v\n", err)
		return -1
	}
	return affectedId
}

//根据id删除用户对象
func delUser(id int) int64 {
	delSql := "DELETE FROM user_tb WHERE id=?"
	result, err := Db.Exec(delSql, id)
	if err != nil {
		fmt.Printf("exec del failed:%v\n", err)
		return -1
	}
	affectedId, err := result.RowsAffected()
	if err != nil {
		fmt.Printf("get affected id failed:%v\n", err)
		return -1
	}
	return affectedId
}

//根据用户名查询用户对象
func findUserByUsername(username string) (err error, user User) {
	sqlStr := "SELECT * FROM user_tb WHERE username=?"
	row := Db.QueryRow(sqlStr, username)
	err = row.Scan(&user.Id, &user.Username, &user.Password, &user.Age, &user.Sex, &user.Phone)
	if err != nil {
		return
	}
	return
}

//根据ID查询用户对象
func findUserById(id int) (err error, user User) {
	sqlStr := "SELECT * FROM user_tb WHERE id=?"
	stmt, err := Db.Prepare(sqlStr)
	defer closeStmt(stmt)
	row := stmt.QueryRow(id)
	err = row.Scan(&user.Id, &user.Username, &user.Password, &user.Age, &user.Sex, &user.Phone)
	if err != nil {
		return
	}
	return
}

//根据用户名和密码查询用户对象
func findUserByUsernameAndPassword(username string, password string) (err error, user User) {
	sqlStr := "SELECT * FROM user_tb WHERE username=? AND password=?"
	row := Db.QueryRow(sqlStr, username, password)
	err = row.Scan(&user.Id, &user.Username, &user.Password, &user.Age, &user.Sex, &user.Phone)
	if err != nil {
		return
	}
	return
}

//获取所有用户对象集
func findAllUsers() (err error, users []*User) {
	sqlStr := "SELECT * FROM user_tb"
	rows, err := Db.Query(sqlStr)
	if err != nil {
		fmt.Print("mysql is nul!\n")
		return
	}
	defer closeRow(rows)  //延迟调用
	for rows.Next() {
		var u User
		err = rows.Scan(&u.Id, &u.Username, &u.Password, &u.Age, &u.Sex, &u.Phone)
		fmt.Println(u.Id)
		fmt.Println(u.Username)
		//err = rows.Scan(&u.Id, &u.Username, &u.Password)
		if err != nil {
			fmt.Printf("err is nil\n")
			return
		}
		users = append(users, &u)
	}
	return
}

//关闭stmt连接
func closeStmt(stmt *sql.Stmt) {
	if stmt != nil {
		fmt.Printf("close stmt connection\n")
		stmt.Close()
	}
}

//关闭row连接
func closeRow(rows *sql.Rows) {
	if rows != nil {
		fmt.Printf("close rows connection\n")
		rows.Close()
	}
}

//处理http请求方法
var contentPath = "./static/"

func indexPage(w http.ResponseWriter, r *http.Request) {
	t, err := template.ParseFiles(contentPath + "index.html")
	if err != nil {
		fmt.Fprintf(w, "load index.html failed:%v\n", err)
		return
	}
	t.Execute(w, nil)
}

func login(w http.ResponseWriter, r *http.Request) {
	t, err := template.ParseFiles(contentPath + "login.html")
	if err != nil {
		fmt.Fprintf(w, "load login.html failed:%v\n", err)
		return
	}
	t.Execute(w, nil)
}
//处理登录请求
func loginForm(w http.ResponseWriter, r *http.Request) {
	method := r.Method
	if method == "POST" {
		r.ParseForm()
		//获取表单的中数据
		usernmae := r.FormValue("username")
		password := r.FormValue("password")
		err, user := findUserByUsernameAndPassword(usernmae, password)
		if err != nil {
			fmt.Printf("find user failed:%v\n", err)
			t, err := template.ParseFiles(contentPath + "login.html")
			if err != nil {
				fmt.Fprintf(w, "load login.html failed:%v", err)
				return
			}
			t.Execute(w, "用户名或密码不正确,请确认")
			return
		}
		t, err := template.ParseFiles(contentPath + "success.html")
		if err != nil {
			fmt.Fprintf(w, "load success.html failed:%v", err)
			return
		}
		t.Execute(w, user)

	}
}

//处理更新请求
func update(w http.ResponseWriter, r *http.Request) {
	r.Header.Add("Content-Type", "application/json")
	boday, err := ioutil.ReadAll(r.Body)
	if err != nil {
		fmt.Fprintf(w, "read body failed:%v", err)
		return
	}
	var user User
	err = json.Unmarshal(boday,&user)
	if err != nil {
		fmt.Fprintf(w,"{\"error\":\"%v\"}",err)
		return
	}
	fmt.Printf("%#v\n", user)
	rowId := updateUser(user)
	fmt.Fprintf(w, "{\"status\":\"ok\",\"affectedId\":\"%d\"}",rowId)
}

//处理添加用户请求
func add(w http.ResponseWriter, r *http.Request) {
	r.Header.Add("Content-Type", "application/json")
	boday, err := ioutil.ReadAll(r.Body) //读取body消息体
	if err != nil {
		fmt.Fprintf(w, "read body failed:%v", err)
		return
	}
	var user User
	err = json.Unmarshal(boday,&user) //反序列化成user对象
	if err != nil {
		fmt.Fprintf(w,"{\"error\":\"%v\"}",err)
		return
	}
	fmt.Printf("%#v\n", user)
	rowId := addUser(&user)
	fmt.Fprintf(w, "{\"status\":\"ok\",\"rowId\":\"%d\"}",rowId)
}

//处理查询请求
func findById(w http.ResponseWriter, r *http.Request){

	idStr  := r.URL.Query().Get("id")
	id,err := strconv.Atoi(idStr)
	if err != nil {
		fmt.Fprintf(w,"parse id string to int failed:%v",err)
		return
	}
	err,user := findUserById(id)
	if err != nil {
		fmt.Fprintf(w,"get user by %d failed:%v",id,err)
		return
	}

	data,err := json.Marshal(&user) //将user struct序列化成字节数组
	if err != nil {
		fmt.Fprintf(w,"parse user data struct failed:%v",err)
		return
	}
	fmt.Fprintf(w,string(data))
}

//处理查询请求
func findAll(w http.ResponseWriter, r *http.Request){

	err,user := findAllUsers()
	if err != nil {
		fmt.Fprintf(w,"get user failed:%v",err)
		return
	}
	data,err := json.Marshal(&user) //将user struct序列化成字节数组
	if err != nil {
		fmt.Fprintf(w,"parse user data struct failed:%v",err)
		return
	}
	fmt.Fprintf(w,string(data))
}

func main() {
	//添加路由
	http.HandleFunc("/", indexPage)
	http.HandleFunc("/index", indexPage)
	http.HandleFunc("/login", login)
	http.HandleFunc("/loginForm", loginForm)
	http.HandleFunc("/add", add)
	http.HandleFunc("/update", update)
	http.HandleFunc("/findById",findById)
	http.HandleFunc("/findAll",findAll)
	//启动服务端口监听
	err := http.ListenAndServe(":9090", nil)
	if err != nil {
		fmt.Printf("Listen 9090 port failed:%v\n", err)
		return
	}
}
```

# ===================

# Golang从入门到进阶实战

## 1.基本语法(基本数据)

### 1.1变量定义

#### 1.1.1 标准格式 

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

#### 1.1.2 批量格式 

```go
var ( 
a int 
b string 
c [] float32 
d func () bool 
e struct { 
x int 
}
)
```


### 1.2变量赋值

#### 1.2.1 标准格式

var 变量名类型＝表达式
例如：游戏中，玩家的血量初始值为 100 。可以这样写：

```go
var hp int = 100
```

#### 1.2.2 编译器推导类型的格式

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

#### 1.2.3 短变量声明并初始化

var 的变量声 明还有 种更为精简的写法，例如： 

hp := 100  

这是 Go 语言的推导声明写法，编译器会自动根据右值类型推断出左值的对应类型。

#### 1.2.4 匿名变量

在使用多重赋值时，如果不需要在左值中接收变量 可以使用匿名变量。 

```go
 func GetData() (int ，int) { 
 	return 100 , 200 
 } 
 
 a , _: = GetData() 
 
 _, b : = GetData() 
 
 fmt . Println(a , b)
```

### 1.3数据类型

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

### 1.4数据类型转换

Go 语言使用类型前置加括号的方式进行类型转换 一般格式如下 ：

T（表达式）

代表要转换的类型。表达式包括变量、复杂算子和函数返回值等。

注意：类型转换 ，需要考虑两种类型的关系和范围，是否会发生数值截断等 。



### 1.5指针

指针概念在 Go 语言中被拆分为两个核 概念 

1.类型指针，允许对这个指针类型的数据进行修改。传递数据使用指针，而无须拷贝数据。类型指针不能进行偏移和运算。 

2.切片，由指向起始元素的原始指针、元素数量和容量组成。

ptr : = &v   

v的类型为 T, 

其中V代表被取地址的变量，被取地址V使用ptr变量进行接收， prt的类型就为"*T"， 

称做T的指针类型,"*"代表指针。

#### 1.5.1使用指针变量获取命令行的输入信息

#### 1.5.2创建指针的另外一种方法new()

	Go 还提供了另外 种方法来创建指针变 ，格式如下： 

new(类型 )

一般这样写： 

str := new(string)  

*str = "ninja"  

fmt.Println(*str)  

new （）函数可以创建一个对应类型的指针，创建过程会分配内存。被创建的指针指向的值为默认值。



### 1.6变量的声明周期

#### 1.6.1 栈空间

#### 1.6.2 堆空间



### 1.7 字符串

#### 1.7.1 保存格式

字符串一般utf-8保存， 一个汉字占用3个字符。

#### 1.7.2 遍历字符

以ascii打印字符。

```go
theme ：＝"狙击 start"
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

#### 1.7.3 **总结：** 

**• ASCII 字符串遍历直接使用下标** 

**• Unicode 字符串遍历用 for range** 

#### 1.7.4 字符串拼接

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

#### 1.7.5 格式化

预留

#### 1.7.6 base64 ini文件读取写入操作等

#### 1.7.7 别名



## **2.容器：存储和组织数据的方式**

本章将以实用为目的，详细介绍数组、切片、映射，以及列表的增加、删除、修改和遍 

历的使用方法

### 2.1 数组-固定大小的连续空间

#### 2.1.1 声明: 

*var 数组变量名［元素数量］T*

· 数组变量名 数组声明及使用时的变量名。 

· 元素数量 数组的元素数量。可以是一个表达式，但最终通过编译期计算的结果必须是整型数值。也就是说，元素数量不能含有到运行时才能确认大小的数值 

· T 可以是任意基本类型，包括 为数组本身。但类型为数组本身时，可以实现多维数组。

```go
var team [3]string  //将 team 明为包含3个元素的字符串数组。
```

#### 2.1.2 **初始化**

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

### 2.2 切片(slice)-动态分配大小的连续空间

### 2.3 映射(map)-建立食物关联的容器（无序的）

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

### 2.4 列表（list)-可以快速增删的非连续性空间的容器

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

### 4.1 声明形式

```go
func 函数名（参数列表）（返回参数列表）｛
	函数体
}

func foo ( a int, b string ) int
//参数类型简写， 返回int类型
func add (a,b int ）int { 
	return a + b 
}

//返回多个值
func swap(x, y string) (string, string) {
   return y, x
}        
```

### 4.2 函数变量

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

### 4.3 匿名函数

定义: 匿名函数就是没有名字的普通函数定义. 

```go
func (参数列表) (返回参数列表){
	函数体
}
```

4.3.1 在定义时直接调用匿名函数

```go
func(data init) { 
	fmt.Println (”hello”, data) 
} (100)
```

4.3.2 将匿名函数赋值给变量

```go
//将匿名函数体保存到f()中
f := func(data int){
    fmt.Println("hello",data)
}
//使用f()调用
f(100)
```

4.3.3 匿名函数作为回调函数

```go
//对切片进行遍历操作,遍历中访问每个元素的操作使用匿名函数来实现

//遍历切片的每个元素,通过给定函数进行元素访问
func visit(list []int, f func(int)){
    for _,v := range list{
        f(v)
    }
}

func main() {
    //使用匿名函数打印切片内容
    visit([]int{1,2,3,4}, func(v int){
        fmt.Println(v)
    })
}
```

4.3.4 使用匿名函数实现操作封装



### 5.函数类型实现接口



### 6.闭包

闭包＝函数＋引用环境

### 7.可变参数

```go
//v 为可变参数变量, 类型为[]T, 也就是拥有多个T元素的T类型切片.
//T为可变参数类型,当T为interface{}时,传入的可以是任意类型.
func 函数名 (固定参数列表, v ... T) (返回参数列表){
    函数体
}
```

7.1 所有参数都是可变参数:

```go
//打印函数定义如下:
func Println(a ... interface{}) (n int, err error){
    //略
}

//调用时,参数可以是任意类型.
fmt.Println(5,"hello". &struct{a int}{1}, true)
```

7.2 部分参数为可变参数

```go
func Printf(format string, a ...interface{}) (n int, err error){
    //略
}

//调用时,第一个参数必须为string
fmt.Printf("pure string\n")
fmt.Printf("value: %v %f\n",true, math.Pi)
```



### 8.延迟执行语句

​	Go 言的 defe 语句会将其后面跟随的语句进行延迟处理。在 defer 归属的函数即将返回时，将延迟处理的语句按 defer 的逆序进行执行，也就是说，先被 defer 的语句最后被执行，最后被 defer 的语旬，最先被执行。

（内心独白：好鸡肋的作用，按c语言编程习惯，这个就是个无用的东西．）

8.1使用延迟语句在函数退出时释放资源.

### 9.处理运行时发生的错误

### 10.宕机(panic)

10.1 手动触发宕机

```go
func main(){
    panic("crash")
}
//代码运行崩溃并输出如下:
panci: crash
goroutine 1 [running]:
main.main()
main.go:5 +0x6b
```

10.2 在运行依赖的必备资源时主动触发宕机

10.3 在宕机时触发延迟执行语句

```go
func main(){
    defer fmt.Println("宕机后要做的事情1")
    defer fmt.Println("宕机后要做的事情2")
    panic("宕机")
}
```

### 11.宕机恢复







## 5.结构体

定义: 一个复合类型.  没有类等面向对象的概念.

```go
type 类型名 struct{
    字段1   字段1的类型
    字段2   字段2的类型
    ...
}
```

#### 5.1 实例化结构体

```go
var test T
```

T为结构体类型.  test为结构体的实例.

5.1.1 创建指针类型的结构体