# Linux常用命令
## 1. Linux系统IPC操作命令


# 进程管理 

#### 进程描述符和任务结构

1. 







# 进程间通信

## 1.管道：

### 1.有名管道

#### 特性

- 特点：1、可以用于不同进程间通信，不限于必须有亲缘关系。
#### 有名管道的使用
- 1、管道的创建 =====》mkfifo
  int mkfifo(const char * filename,mode_t mode); 
  
  功能：通过该函数可以创建一个有名管道的设备文件出来。 
  
  参数：filename 要创建的管道名称+ 路径 mode  创建的管道的文件权限。 
  
  返回值： 成功 0 失败  -1；


- 2、管道的打开 =====》open 

  int open(const char * filename,int flag); 

  参数： filename要打开的有名管道文件名称       

  flag == 》O_RDONLY  只读   O_WRONLY  只写 O_RDWR  不能用。

- 3、管道的读写  ====>read  write   

  读： ssize_t read(int fd, void * buff,size_t count ); 

  从fd中读count个字节的数据到buff中。   

  写： ssize_t  write(int fd, const void * buff,size_t count) 

  从buff中取count个字节的数据写入到fd文件中。

- 4、管道的关闭  ====>close   

  close（fd）；

- 5、管道的卸载  ====>unlink int unlink(const char * pathname); 

  功能：卸载或者删除已经创建的有名管道文件 

  参数： pathname 要卸载的管道文件名称+路径 

  返回值： 成功  0       失败 -1；

### 2.无名管道
#### 特性

  - 1、只能用于具有亲缘关系的进程间使用。  

  - 2、半双工通信，有固定的读端和写端。  

  - 3、特殊的系统文件，可以支持文件IO。  

  - 4、管道的数据存储方式类似队列，先进先出。 

  - 5、管道的默认容量大小是64k字节 ===》ulimit -a ====>4k 早期的大小  

  - 6、管道默认的读操作会阻塞等待，如果写操作满了的时候也会阻塞等待。  

  - 7、管道的读端存在时候，写管道才有意义，否则程序会退出.

#### 无名管道的使用

- 1、管道的创建与打开 int pipe(int fd[2]) ; 

     >  功能：通过该函数可以创建一个无名管道，同时打开该管道。 
     >
     >  参数： fd[2]  要操作的管道名称，有两个元素 fd[0]  管道的固定的读端 fd[1]  管道的固定的写端   
     >
     >  返回值： 成功 0 失败  -1   注意：在创建新的进程之前就应该先创建管道，之后之间的资源可以 通过管道传递。

- 2、管道的读写

     > 读： ssize_t read(int fd, void * buff,size_t count );
     >
     > 从fd中读count个字节的数据到buff中。
     >
     > 写： ssize_t  write(int fd, const void * buff,size_t count)
     >
     > 从buff中取count个字节的数据写入到fd文件中。

- 3、管道的关闭

     > close（fd[0]）；
     > close(fd[1]);


## 2.信号：

### 1.信号的发送   

kill 函数 =====》支持的信号列表 kill -l ====>所有当前系统支持的默认信号   

头文件： signal.h   sys/types.h  



```c
 int  kill(pid_t pid,int sig); 
```

功能：给指定的pid进程发送sig信号。 

参数：pid要接收信号的进程id sig 要发送的信号编号 ====》kill -l 中的信号 kill -l 中前32 属于系统原始信号，也叫不稳定信号  后32个属于稳定信号。 

返回值： 成功 0  失败 -1；    



```c
int raise(int sig); 
```

功能：进程可以自己通过该函数给自己发送信号。 

参数：sig 要发的信号的编号 

返回值：成功 0 失败 -1；     



**闹钟函数和暂停函数**  

暂停函数 

```c
int pause(void ) ====>while(1){sleep(1);} 
```

执行该函数后程序暂停。   



闹钟函数

定时时间到了发送 SIGALRM 信号给自己。  

```c
unsigned int alarm(unisgned int sec);
```

指定间隔sec秒之后给自己发送信号。 

默认的SIGALRM 信号会使程序终止运行。    

### 2.信号的接收

**三种方式： 默认处理  忽略处理  自定义处理**    



1、捕获信号和处理   

```c
void () (int);======>void fun(int arg);   
void (signal(int signum,void (handler)(int)))(int); 
signal(int signum , test);===> test == void(handler)(int);   typedef void (SIGNAL)(int);  ===> void (signal(int signum,void (handler)(int)))(int); == SIGNAL signal(int singnum,SIGNAL handler);   
```

简化成：

```c
 #include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
```

参数： 

sig 要处理的信号 

fun  信号的处理函数。

如果其是：

​	SIG_IGN 表示该程序对所有的信号做忽略处理 。    

​	SIG_DFL 表示该程序对所有的信号做默认处理 ,系统对这些信号是怎么处理的，程序就会对该信号怎么处理。

​	fun     表示改程序有一个回调函数用来自定义处理 。



函数可以接收信号，并对信号做出处理， 三种处理方式。 

1. 忽略。 
2. 默认处理： 
3. 自定义处理：接收到该信号之后，会调用自定义的回调函数。

**注意**：在所有系统预制的信号列表中，9号SIGKILL 和 19 号的SIGSTOP信号不能被忽略处理。      

例子：

```c
//SIGABRT  进程停止运行 6
//SIGINT      终端中断  2
//SIGKILL     停止进程(此信号不能被忽略或捕获)
//SIGQUIT   终端退出   3

void handler(int num) 
{
	printf("recv num:%d \n", num);	
	if (num == SIGQUIT)
	{
		//exit(0);
	}
}
int main()
{
    if (signal(SIGINT, handler) == SIG_ERR)
    {
        perror("signal err"); //errno
        exit(0);
    } 
}
```

### 3.信号在进程间通信中的缺陷：  

1、不能发送大量数据，包括字符串。  

2、发送的信号必须是双方约定好的。  

3、发送的信号必须是系统预制的范围内的。  

4、发送的信号在接收方必须有自定义处理，一般用SIGUSR1 SIGUSER2.    其他信号有系统含义 建议不要使用。        



### 4.信号和中断的区别

信号是在软件层次上对中断的一种模拟，所以通常把它称为是软中断

信号与中断的相似点：

（1）采用了相同的异步通信方式；

（2）当检测出有信号或中断请求时，都暂停正在执行的程序而转去执行相应的处理程序；

（3）都在处理完毕后返回到原来的断点；

（4）对信号或中断都可进行屏蔽。

 信号与中断的区别：

（1）中断有优先级，而信号没有优先级，所有的信号都是平等的；

（2）信号处理程序是在用户态下运行的，而中断处理程序是在核心态下运行；

（3）中断响应是及时的，而信号响应通常都有较大的时间延迟。





## 3.IPC对象

### 0.基本概念

IPC对象包含了：共享内存、信号量、消息队列

1、ipc对象查看命令

  	ipcs -a   ====> 查看对象信息

​	ipcs -q   ====>只查看消息队列的对象信息 

​	ipcs -m  ====>只查看共享内存的对象信息 

​	ipcs -s    ====>只查看信号量的对象信息 

2、ipc对象操作命令

​	ipcs -l   ipcs -lq   ipcs -lm    ipcs -ls    ===>查看对象的默认上限值  

​	ipcrm ===>删除命令  ===》ipcrm -q msgid ====>删除消息队列中队列id是msgid的对象 

​	ipcrm -m shmid ====>共享内存对象删除 

​	ipcrm -s  semid ====>信号量集对象删除

3、基本操作流程：   

​	key  ====>申请或者创建IPC对象====>读写数据 ====>卸载对象 

### 1.key 的获取

1、私有key =====> IPC_PRIVATE === 0X00000000  

2、测试key =====> ftok() ====>指定路径+字符  

3、自定义key ====> 0X12345678       

```c
key_t ftok(const char * pathname ,int pro_id);  
```

**功能**：通过该函数可以以指定的路径为基本生成一个唯一键值。  

**参数**：pathname 任意指定一个不可卸载的目录同时要求改目录不能被删除重建。        

**pro_id**  一个数字，或者字符，表示将该值与参数1做运算之后的值作为键值。    

返回值：成功  返回唯一键值，       失败 -1； 

### 2.消息队列

#### 0、头文件： 

```c++
#include<sys/types.h >
#include<sys/ipc.h >
#include<sys/msg.h >
```

#### 1、申请消息队列;

```c
int msgget(key_t key ,int flag);
//功能：向内核提出申请一个消息队列对象。
//参数： 
//key  用户空间的键值      
//flag  消息对象的访问权限，但是如果是第一次向内核提出申请，则需要添加IPC_CREAT 和 IPC_EXCL 
//返回值：成功 返回消息对象id失败 -1
```

#### 2、消息对象的操作：

##### 1.发送消息：

```c
int msgsnd(int msgid,void * msgp,size_t size ,int flag); 
```

参数：msgid 要发送到的消息对象id
msgp ====>要发送的消息结构体===》

```c
struct msgbuf{
	long mtype;   ////消息的类型
	char mtext[N];////消息的正文，N 自定义的数据大小
}；
```

size :要发送的消息正文的长度，单位是字节。
flag :   = 0  表示阻塞发送     
	    = IPC_NOWAIT 非阻塞方式发送
返回值：成功  0 ,  失败  -1；

##### 2.接收消息：

```c
int msgrcv(int msgid,void * msgp,size_t size,long type,int flag);
```

参数： msgid 要接收到的消息对象id 

msgp ====>要接收的消息结构体变量，必须事先定义一个空变量，用来存储数据。 

size  ====》要接收的数据长度，一般是 sizeof(msgp); 

type  ====>要接收的消息类型 

flag  ====》接收消息的方式，0 表示阻塞接收  IPC_NOWAIT 非阻塞接收   

返回值：成功  0 失败  -1；



##### 3.消息队列对象的卸载：

```c
int msgctl(int msgid,int cmd, struct msgid_ds * buff);
```

功能：调整消息队列的属性，很多时候用来删除消息队列。 

参数： msgid 要操作的消息队列对象id 

cmd  ==>IPC_RMID ====>删除消息队列的宏 

IPC_SET  ====>设置属性 

IPC_STAT ====》获取属性 

buff ====》属性结构体 

返回值：成功 0 失败 -1

### 3.共享内存

#### 0、头文件

```c
#include <sys/shm.h>
```

#### 1、key值得创建

同上。

#### 2、shmget 向内核申请共享内存对象

```c
int shmget(key_t key ,int size ,int flag);
```

参数： key  用户空间的唯一键值 

size 要申请的共享内存大小 

flag  申请的共享内存访问权限，如果是第一次申请，则需要 IPC_CREAT IPC_EXCL; 

返回值： 成功 shmid 失败 -1；

#### 3、shmat  将内核申请成功的共享内存映射到本地

```c
void * shmat(int shmid,const void * shmaddr,int flag);
```

参数： shmid  申请好的共享内存id 

shmaddr   ===》NULL 表示由系统自动查找合适的内存映射。 

flag  ====》对于挂载之后的内存的操作权限。0  表示可直接读写 SHM_RDONLY   表示只读权限 

返回值：成功 返回映射后的可以使用的地址 失败 NULL;

####  4.读写共享内存

读写共享内存 ====》类似操作堆区的方式

#### 5.shmdt  断开本地与共享内存的映射

```c
int shmdt(void * shmaddr);
```

参数： shmaddr  要断开的已经映射的地址，就是shmat的返回值。 

返回值： 成功 0  失败 -1；

#### 6.shmctl  删除共享内存操作对象

```c
int shmctl(int shmid,int cmd ,struct shmid_ds *buff);
```

参数： shmid 要删除的共享内存对象 

cmd 要操作的宏：

IPC_RMID 表示删除对象 

IPC_STAT 表示获取对象属性 

IPC_SET  表示设置对象属性 

buff 属性结构体对象 

返回值：成功 0 失败-1

### 4.信号量与信号量集

信号量集就是将多个信号量放入数组，统一管理，IPC使用的是信号量的数组(信号量集)，而不是单一的信号量。  **信号量集其实就是一个计数器数组，只能控制进程数量而不能互发数据。**

#### 1.头文件 sys/sem.h

#### 2.key 唯一键值

#### 3.semget 向内核申请一个信号量集

```c
int semget（key_t key ,int nsems, int flag）;
```

参数：key  用户空间的唯一键值      

nsems 要申请的信号量个数      

flag  申请信号量的权限，如果是第一次申请则需IPC_CRATE IPC_EXCL; 

返回值：成功 0 失败 -1

#### 4.semop 操作信号量

semop 操作信号量 ===》PV 操作 ====》信号量的加一 减一

```c
int semop(int semid,struct sembuf * buff,size_t opts);
```

参数： semid  要操作的信号量集id 

buff要操作的方式方法===》操作结构体 sembuf

```c
struct sembuf
{
    short sem_num;    //要操作的信号量的编号，一般都是从0 编号。
    short sem_op;     //要给该信号量进行的操作，1  V操作 释放资源  -1 P 操作 申请资源   0  阻塞等待
    short sem_flg;    //0  表示阻塞方式执行信号量   IPC_NOWAIT 表示非阻塞  SEM_UNDO   表示操作结束后返回原值
 }；
```

opts：要操作的信号量的个数。 

返回值： 成功 0  失败 -1；

#### 5.semctl 删除信号量

```c
int semctl(int semid, int semnum,int cmd ...);
```

功能：通过该函数可以调整信号量集中指定的信号量的属性。 

参数：

semid 要调整的信号量集的id      

semnum 要调整的信号量编号      

cmd  要执行的动作  

IPC_RMID  表示要删除该对象 

SETVAL   表示设置信号量的值 

GETVAL   表示获取信号量的值 

返回值：成功 0 失败 -1 

# 临界值的访问需要保证一致性 

## 1.互斥量（也叫互斥锁）

### 1.定义互斥锁

在线程开始之前定义一个全局变量

pthread_mutex_t mutex;

其中mutex 就是互斥锁。

### 2. 初始化互斥锁 

一般在定义多线程开始之前。

静态分配：

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```

动态分配：

```c
int  pthread_mutex_init(pthread_mutex_t *mutex,pthread_mutexattr_t * attr);
```

功能：通过该函数可以将已经定义的互斥锁设置为默认属性的锁。 

参数： mutex  定义好的互斥锁 

attr 锁的属性，NULL表示默认属性。 

返回值:成功  0 失败  -1；

### 3.加锁

```c
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
```

pthread_mutex_lock()会对互斥量上锁，如果互斥量已经处于锁住状态，则会阻塞等待互斥量被解锁。

pthread_mutex_trylock()会对互斥量上锁，如果互斥量已经处于锁住状态，则不能锁住互斥量，并返回EBUSY.

### 4.解锁

```c
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

以上两个函数一般成对出现，其中mutex参数就是要加锁的锁子。 如果有先lock 则在unlock 之前的所有代码有可以保证属于原子操作 在执行被保护的代码过程中，其他需要用到该代码的程序会阻塞等待。

### 5.销毁锁

```c
int  pthread_mutex_destroy(pthread_mutex_t * mutex);
```

功能： 将已经不在使用的互斥锁销毁。 

参数：mutex 要销毁的锁 

返回值：成功 0 失败 -1；

## 2.记录锁

