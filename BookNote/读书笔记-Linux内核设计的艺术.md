# Linux内核设计的艺术

intel8086系列的cpu可以分别在16位实模式和32位保护模式下运行。为了兼容intel将所有80x86系列cpu都设计为上电既进入16位实模式状态运行。并将cs值置为：0xffff    ip值置为：0x0000.这样cs:ip指向0xffff0

Intel 8086 在16位实模式下可以访问 1MB 的内存空间，地址范围为 0x00000 到 0xFFFFF。

16位实模式下，有20位地址总线，所有可以访问1mb的地址空间。

出于各方面的考虑，计算机系统的设计者将这 1MB 的内存空间从物理上分为几个部分。
8086 有 20 根地址线，但并非全都用来访问 DRAM，也就是内存条。事实上，这些地址
线经过分配，大部分用于访问 DRAM，剩余的部分给了只读存储器 ROM 和外围的板卡.

1.0x00000-0x9FFFF:DRAM(动态随机访问存储器):

2.0xA0000-0xEFFFF:   分给外围设备

3.0xF0000-0xFFFFF: ROM(只读存储器),占据内存顶端的64k空间. 固化了开机时要执行的指令

0xB8000～0xBFFFF : 分给显卡的.

8086加电或复位时,cs=0xffff,ip=0x0000,位于rom处,开始执行bios程序. 

物理地址为0xFFFF0, 到最后结束也只有16个字节.所以一般为一个jmp指令.

处理器取指令执行的自然顺序是从内存的低地址向高地址推进.

## 1.linux0.11启动流程分析

### 1.1 初始化阶段/boot/中的汇编代码

1.bios入口地址为0xffff0，bios的第一条指令就设计在这个位置上。

2.bios检测显卡内存，并在内存中建立中断向量表和中断服务程序。

中断向量表：0x00000~0x003ff，1kb的内存空间。并在紧挨着他的位置用256字节构建bios数据区：0x00400~0x004ff，在大约56kb以后的位置(0x0x0e2ce)，加载了8kb左右的与中断向量表相对应的中断服务程序。

3.bios调用0x19中断服务程序(启动加载服务程序)将启动盘的第一个扇区512b读入到内存0x7c00处。这个512b的程序就是启动扇区。就是linux0.11代码中bootsect.s汇编而成的。

4.在bootsect.s中,start.执行将0x7c00处的代码(也就是自己)复制到0x9000处.   然后跳转到新地址中的go标记继续执行.

```asm
; 刘十三
; 1.将要加载的setup程序的扇区数(SETUPLEN),和被加载到的位置(SETUPSEG)
; 启动扇区被bios加载的位置(BOOTSEG),和将要移动到的新位置(INITSEG)
; 内核被加载的位置(SYSSEG),内核的末尾位置(ENDSEG)
; 根文件系统设备号(ROOT_DEV)
entry start
start:
    mov    ax,#BOOTSEG
    mov    ds,ax
    mov    ax,#INITSEG
    mov    es,ax
    mov    cx,#256
    sub    si,si
    sub    di,di
    rep
    movw                
    ; 从start到这行, bootsect将自身从内存0x7c00处拷贝到0x9000处.
```

5.初始化栈寄存器，就可使用push和pop操作， 压栈方向从高地址到低地址。

```assembly
;  刘十三:
;  2.对DS(数据段寄存器) ES(附加段寄存器) SS(栈基址寄存器) 设置成0x9000, 并将sp(栈顶指针)指向0xff00处.
;  对这几个寄存器的设置,就可以在后边使用push和pop操作。
go:    mov    ax,cs
    mov    ds,ax
    mov    es,ax
;  put stack at 0x9ff00.
    mov    ss,ax
    mov    sp,#0xFF00        ;  arbitrary value >>512
```

6.调用0x13中断，将软盘从第2扇区开始的4个扇区（2~5扇区），既setup.s对应的程序加载到0x90200处，紧挨着启动扇区。

```assembly
load_setup:
    mov    dx,#0x0000        ;  drive 0, head 0
    mov    cx,#0x0002        ;  sector 2, track 0
    mov    bx,#0x0200        ;  address = 512, in INITSEG
    mov    ax,#0x0200+SETUPLEN    ;  service 2, nr of sectors
    int    0x13            ;  read it
    jnc    ok_load_setup        ;  ok - continue
    mov    dx,#0x0000
    mov    ax,#0x0000        ;  reset the diskette
    int    0x13
    j    load_setup
```

7.调用0x13中断，将system模块加载到内存。

​    system模块有240个扇区，然后跳转到0x90200处执行。

8.setup.s开始运行，提取内核运行需要的机器系统数据。

从0x41和0x46的中断向量函数中获取硬盘参数表1和硬盘参数表2并分别放在0x9000:0x0080和0x9000:0x0090处。  大约占用内存0x90000~0x901fc. 覆盖了原来的bootsect程序，只剩余2字节未被覆盖。 后续main函数执行时，需要用到这里边的机器系统数据。

9.

未完待续，汇编太多，看不下去了。

### 1.2 从main到怠速状态

### 系统调用

1.系统调用号:  对应相应的系统调用。