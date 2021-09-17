## GO实现HTTP服务器

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

![](/home/hao/work_my/Learning_set/tools_Lib/all_picture/内核笔记/57.jpg)

![](/home/hao/work_my/Learning_set/tools_Lib/all_picture/内核笔记/58.png)

### 5.HTTP响应格式

![](/home/hao/work_my/Learning_set/tools_Lib/all_picture/内核笔记/59.jpg)

![60](/home/hao/work_my/Learning_set/tools_Lib/all_picture/内核笔记/60.png)

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

![](/home/hao/work_my/Learning_set/tools_Lib/all_picture/内核笔记/61.jpg)

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



