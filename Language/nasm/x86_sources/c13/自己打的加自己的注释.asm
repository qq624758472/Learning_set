;微型内核程序
;定义常量，不占用内存空间
core_code_seg_sel     equ  0x38    ;内核代码段选择子
core_data_seg_sel     equ  0x30    ;内核数据段选择子 
sys_routine_seg_sel   equ  0x28    ;系统公共例程代码段的选择子 
video_ram_seg_sel     equ  0x20    ;视频显示缓冲区的段选择子
core_stack_seg_sel    equ  0x18    ;内核堆栈段选择子
mem_0_4_gb_seg_sel    equ  0x08    ;整个0-4GB内存的段的选择子


;============
;系统核心的头部文件，用于加载核心程序 给主引导程序提供内核的程序大小和描述符位置
core_length      dd core_end       ;核心程序总长度#00

sys_routine_seg  dd section.sys_routine.start
;系统公用例程段位置#04

core_data_seg    dd section.core_data.start
;核心数据段位置#08

core_code_seg    dd section.core_code.start
;核心代码段位置#0c


core_entry       dd start          ;核心代码段入口点#10
                dw core_code_seg_sel

[bits 32]
section sys_routine vstart = 0
;公共例程代码段 主要是保存内核级别的库函数提供用户程序调用
         ;字符串显示例程
put_string:                                 ;显示0终止的字符串并移动光标 
                                            ;输入：DS:EBX=串地址
         push ecx
  .getc:
         mov cl,[ebx]
         or cl,cl
         jz .exit
         call put_char
         inc ebx
         jmp .getc

  .exit:
         pop ecx
         retf                               ;段间返回

;-------------------------------------------------------------------------------
put_char:                                   ;在当前光标处显示一个字符,并推进
                                            ;光标。仅用于段内调用 
                                            ;输入：CL=字符ASCII码 
         pushad

         ;以下取当前光标位置
         mov dx,0x3d4
         mov al,0x0e
         out dx,al
         inc dx                             ;0x3d5
         in al,dx                           ;高字
         mov ah,al

         dec dx                             ;0x3d4
         mov al,0x0f
         out dx,al
         inc dx                             ;0x3d5
         in al,dx                           ;低字
         mov bx,ax                          ;BX=代表光标位置的16位数

         cmp cl,0x0d                        ;回车符？
         jnz .put_0a
         mov ax,bx
         mov bl,80
         div bl
         mul bl
         mov bx,ax
         jmp .set_cursor

  .put_0a:
         cmp cl,0x0a                        ;换行符？
         jnz .put_other
         add bx,80
         jmp .roll_screen

  .put_other:                               ;正常显示字符
         push es
         mov eax,video_ram_seg_sel          ;0xb8000段的选择子
         mov es,eax
         shl bx,1
         mov [es:bx],cl
         pop es

         ;以下将光标位置推进一个字符
         shr bx,1
         inc bx

  .roll_screen:
         cmp bx,2000                        ;光标超出屏幕？滚屏
         jl .set_cursor

         push ds
         push es
         mov eax,video_ram_seg_sel
         mov ds,eax
         mov es,eax
         cld
         mov esi,0xa0                       ;小心！32位模式下movsb/w/d 
         mov edi,0x00                       ;使用的是esi/edi/ecx 
         mov ecx,1920
         rep movsd
         mov bx,3840                        ;清除屏幕最底一行
         mov ecx,80                         ;32位程序应该使用ECX
  .cls:
         mov word[es:bx],0x0720
         add bx,2
         loop .cls

         pop es
         pop ds

         mov bx,1920

  .set_cursor:
         mov dx,0x3d4
         mov al,0x0e
         out dx,al
         inc dx                             ;0x3d5
         mov al,bh
         out dx,al
         dec dx                             ;0x3d4
         mov al,0x0f
         out dx,al
         inc dx                             ;0x3d5
         mov al,bl
         out dx,al

         popad
         ret  

set_up_gdt_descriptor:                  ;输入edx：eax的描述符
        push eax                        ;输出cx是段的选择子
        push ebx
        push edx
        push ds
        push es

        mov ebx,core_data_seg_sel
        mov ds,ebx
        sgdt [pgdt]

        mov ebx,mem_0_4_gb_seg_sel
        mov es,ebx
        movzx ebx,word[pgdt]        ;读界限
        inc bx                      ;下一个描述符在GDT中的偏移地址
        add ebx,[pgdt+2]            ;读基地址

        mov [es:ebx], eax           ;由于es指向4GB的地址，ebx地址就是物理地址
        mov [es:ebx+4], edx
        add word [pdgt],8
        lgdt [pgdt] ;update pgdt
        mov ax,[pgdt]
        xor dx,dx
        mov bx,8
        div bx
        mov cx,ax
        shl cx,3                     ;构造段选择子（这里的RTL是0


make_seg_descriptor:
                                            ;eax：传入基地址 ebx：界限 ecx：字节粒度
        mov edx,eax
        shl eax,16
        or ax,bx

        and ebx,0xffff0000
        rol edx,8
        bswap edx
        xor bx,bx
        or edx,ebx
        or edx,ecx

        retf
allocate_memory：                           ;传入ecx：内存大小
                                            ;输出ecx：内存的起始地址
        push eax
        push ebx
        push ecx
        push ds

        mov eax,core_data_seg_sel
        mov ds,eax
        
        mov eax,[ram_alloc]
        add eax, ecx                        ;下一次的起始地址

        mov ecx,[ram_alloc]                        ;传回的起始地址

        ;下面操作是处理器要求下一个起始地址需要4字节对齐
        mov ebx, eax
        add ebx, 0xfffffffc
        add ebx,4
        test eax,0x00000003                 ;eax && 最后两位是11的二进制数
        cmovnz eax,ebx                      ;不为零说明原本不是4字节对齐的，凑整
        mov [ram_alloc], eax                ;更新下一个地址的值

        pop ds
        pop ecx
        pop ebx
        pop eax
        retf    ;段间转移
read_hard_disk_0:                           ;从硬盘读取一个逻辑扇区
                                            ;EAX=逻辑扇区号
                                            ;DS:EBX=目标缓冲区地址
                                            ;返回：EBX=EBX+512
         push eax 
         push ecx
         push edx
      
         push eax
         
         mov dx,0x1f2
         mov al,1
         out dx,al                          ;读取的扇区数

         inc dx                             ;0x1f3
         pop eax
         out dx,al                          ;LBA地址7~0

         inc dx                             ;0x1f4
         mov cl,8
         shr eax,cl
         out dx,al                          ;LBA地址15~8

         inc dx                             ;0x1f5
         shr eax,cl
         out dx,al                          ;LBA地址23~16

         inc dx                             ;0x1f6
         shr eax,cl
         or al,0xe0                         ;第一硬盘  LBA地址27~24
         out dx,al

         inc dx                             ;0x1f7
         mov al,0x20                        ;读命令
         out dx,al

  .waits:
         in al,dx
         and al,0x88
         cmp al,0x08
         jnz .waits                         ;不忙，且硬盘已准备好数据传输 

         mov ecx,256                        ;总共要读取的字数
         mov dx,0x1f0
  .readw:
         in ax,dx
         mov [ebx],ax
         add ebx,2
         loop .readw

         pop edx
         pop ecx
         pop eax
      
         retf                               ;段间返回
;============
section core_data vstart = 0
;系统核心数据段


;符号地址检索表
salt:
salt_1           db  '@PrintString'
            times 256-($-salt_1) db 0
                dd  put_string
                dw  sys_routine_seg_sel

salt_2           db  '@ReadDiskData'
            times 256-($-salt_2) db 0
                dd  read_hard_disk_0
                dw  sys_routine_seg_sel

salt_3           db  '@PrintDwordAsHexString'
            times 256-($-salt_3) db 0
                dd  put_hex_dword
                dw  sys_routine_seg_sel

salt_4           db  '@TerminateProgram'
            times 256-($-salt_4) db 0
                dd  return_point
                dw  core_code_seg_sel
salt_item_len   equ $-salt_4        ;每个符号的大小
salt_items      equ ($-salt)/salt_item_len


    
ram_alloc        dd  0x00100000    ;下次分配内存时的起始地址
core_buf times 2048 db 0    ;用户程序目标地址



;============
section core_code vstart = 0
;代码段
load_relocate_program:          加载并重定位用户程序
                                  ;输入：ESI=起始逻辑扇区号
                                  ;返回：AX=指向用户程序头部的选择子 
;1相关寄存器入栈 2从起始扇区读一个扇区（读硬盘操作） 3 开空间再加载用户程序 
;4 重定位段描述符，用户程序基地址 5 重定位SALT相关函数的位置

push eax
push ebx
push ecx
push esi
push edi
push ds
push es

mov eax,core_data_seg_sel
mov ds,eax

mov eax,esi
mov ebx,core_buf
call sys_routine_seg_sel:read_hard_disk_0

;判断程序的大小
mov eax,[core_buf]
mov ebx,eax
and ebx,0xfffffe00
add ebx,512 
test eax,0x000001ff
cmovnz eax,ebx  ;不为零即不是512的倍数，就凑整

mov ecx,eax ;通过寄存器传入所需的内存空间
call sys_routine_seg_sel:allocate_memory    ;分配内存空间
;下面加载用户程序
mov ebx,ecx
push ebx                    ;用户程序首地址保存
xor edx,edx
mov ecx,512
div ecx
mov ecx,eax                 ;总扇区数

mov eax,mem_0_4_gb_seg_sel;ds-> 0~4GB段
mov ds,eax

mov eax,esi ;起始扇区
.b1:
    call sys_routine_seg_sel:read_hard_disk_0
    inc eax     ;扇区数从起始开始不断增加，直到ecx=0跳出循环
    loop .b1
;载入程序完毕
    ;程序头部段描述符
    pop edi     ;把首地址弹入edi中
    mov eax,edi
    mov ebx,[edi+0x04]
    dec ebx
    mov ecx,0x00409200
    call sys_routine_seg_sel:make_seg_descriptor
    call sys_routine_seg_sel:set_up_gdt_descriptor
    mov [edi+0x04],cx  ;把段的描述符传回替代程序长度

    ;重定位用户程序代码段描述符
    mov eax,edi
    add eax,[edi+0x14]
    mov ebx,[edi+0x18]
    dec ebx
    mov ecx, 0x00409200
    call sys_routine_seg_sel:make_seg_descriptor
    call sys_routine_seg_sel:set_up_gdt_descriptor
    mov [edi+0x14],cx                   

    ;建立程序代码段描述符
    mov eax,edi
    add eax,[edi+0x14]                 ;代码起始线性地址
    mov ebx,[edi+0x18]                 ;段长度
    dec ebx                            ;段界限
    mov ecx,0x00409800                 ;字节粒度的代码段描述符
    call sys_routine_seg_sel:make_seg_descriptor
    call sys_routine_seg_sel:set_up_gdt_descriptor
    mov [edi+0x1c],cx

    ;建立程序堆栈段描述符(根据用户需求分配空间)
    mov ecx,[edi+0x0c]                 ;4KB的倍率 
    mov ebx,0x000fffff
    sub ebx,ecx                        ;得到段界限
    mov eax,4096                        
    mul dword [edi+0x0c]                         
    mov ecx,eax                        ;准备为堆栈分配内存 
    call sys_routine_seg_sel:allocate_memory        ;返回栈段空间的基地址
    add eax,ecx                        ;得到堆栈的高端物理地址 
    mov ecx,0x00c09600                 ;4KB粒度的堆栈段描述符
    call sys_routine_seg_sel:make_seg_descriptor
    call sys_routine_seg_sel:set_up_gdt_descriptor
    mov [edi+0x08],cx
    
    ;重定位SALT(一般汇编程序这部分是交给链接器完成的，俗称找符号的定义地址)
    mov eax,[edi+0x04] ;此时该位置已经被改成头部段选择子
    mov es,eax
    mov eax,core_data_seg_sel
    mov ds,eax           

    cld
    mov ecx,[es:0x24]                   ;用户SALT条目数
    mov edi,0x28                        ;SALT起始位置
.b2:
    push ecx
    push edi

    mov ecx,salt_items          ;内核SALT条目数
    mov esi,salt                ;起始位置
.b3:
    push edi
    push esi
    push ecx

    mov ecx,64                  ;每条都是有256个字节来写函数名的，所以要比较64次才比较完
    repe cmpsd                  ;es；edi 和ds：esi比较 相等就继续
    jnz .b4                     ;如果ecx不为零，那么跳入b4
    ;如果两个字符串匹配成功
    mov eax,[esi]               ;内核该字符串的偏移地址赋给用户
    mov [es:edi-256],eax        ;如果成功匹配，edi的位置就是字符串的最后一位
    mov ax,[esi+4]              ;传入段选择子
    mov [es:edi-252],ax
.b4:
    pop ecx
    pop esi
    pop edi
    add esi,salt_item_len       ;指向下一个字符串
    loop .b3

    pop edi
    add edi,256
    pop ecx                     ;内循环ecx出栈，下面ecx就是外循环的
    loop .b2

    mov ax, [es:0x04]           ;上面重定位了头部段描述符
    pop es                             ;恢复到调用此过程前的es段 
    pop ds                             ;恢复到调用此过程前的ds段

    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx

    ret
start:
    mov ecx,core_data_seg_sel   ;ds->data_core
    mov ds,ecx
    
    mov ebx,message_1
    call sys_routine_seg_sel:put_string

    ;显示处理器品牌信息 
    mov eax,0x80000002
    cpuid   ;返回处理器信息->eax,ebx,ecx,edx
    mov [cpu_brand + 0x00],eax
    mov [cpu_brand + 0x04],ebx
    mov [cpu_brand + 0x08],ecx
    mov [cpu_brand + 0x0c],edx

    mov eax,0x80000003
    cpuid
    mov [cpu_brand + 0x10],eax
    mov [cpu_brand + 0x14],ebx
    mov [cpu_brand + 0x18],ecx
    mov [cpu_brand + 0x1c],edx

    mov eax,0x80000004
    cpuid
    mov [cpu_brand + 0x20],eax
    mov [cpu_brand + 0x24],ebx
    mov [cpu_brand + 0x28],ecx
    mov [cpu_brand + 0x2c],edx

    mov ebx,cpu_brnd0;换行空格
    call sys_routine_seg_sel:put_string
    mov ebx,cpu_brand
    call sys_routine_seg_sel:put_string
    mov ebx,cpu_brnd1
    call sys_routine_seg_sel:put_string

    mov ebx,message_5
    call sys_routine_seg_sel:put_string

    mov esi,50
    call load_relocate_program      ;返回的是ax即头部描述符
    mov [esp_pointer],esp           ;保存栈顶指针 

    mov ds,ax
    jmp far [0x10]

return_point:               ;用户程序返回点
    mov eax,core_date_seg_sel
    mov ds,eax

    mov eax,core_stack_seg_sel
    mov ss,eax
    mov esp,[esp_point]

    mov ebx,message_6
    call sys_routine_seg_sel:put_char
;===============================================================================
SECTION core_trail
;-------------------------------------------------------------------------------
core_end: