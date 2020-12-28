
;创建时间2019.11.20

;设置堆栈段和栈指针
mov eax,cs
mov ss,ax
mov sp,0x7c00

;通过GDT的物理地址计算逻辑段地址
mov eax,[cs:gdt_base+0x7c00+0x02];
xor edx,edx
mov ebx,16
div ebx

mov ds,eax ;段基址
mov ebx,edx;段内偏移地址

         ;创建0号描述符，如果调用该描述符，会启动异常中断
         mov dword [ebx+0x00],0x00000000
         mov dword [ebx+0x04],0x00000000
         ;创建1#描述符，这是一个数据段，对应0~4GB的线性地址空间
         mov dword [ebx+0x08],0x0000ffff    ;基地址为0，段界限为0xfffff
         mov dword [ebx+0x0c],0x00cf9200    ;粒度为4KB，存储器段描述符 

         ;创建保护模式下初始代码段描述符
         mov dword [ebx+0x10],0x7c0001ff    ;基地址为0x00007c00，512字节 
         mov dword [ebx+0x14],0x00409800    ;粒度为1个字节，代码段描述符 

         ;创建以上代码段的别名描述符
         mov dword [ebx+0x18],0x7c0001ff    ;基地址为0x00007c00，512字节
         mov dword [ebx+0x1c],0x00409200    ;粒度为1个字节，数据段描述符

         ;栈段
         mov dword [ebx+0x20],0x7c00fffe
         mov dword [ebx+0x24],0x00cf9600

        ;初始化GDTR
	mov word [cs:gdt_size+0x7c00],39;更新max偏移
	lgdt [cs:gdt_size+0x7c00];该指令将偏移和物理地址加载进GDTR
	
	;A20位打开（进入32位保护模式不用20位的地址回转）
	in al,0x92
	or al, 0000_0010B
	out 0x92,al
	cli	;避免调用中断
	
	mov eax,cr0
	or eax,1
	mov cr0,eax	;设置CR0寄存器的PE位
	
	
	;进入保护模式
	jmp dword 0x0010:flush	;0010是代码段描述符的偏移（选择子）
	[bits 32]	;告诉编译器环境是32位
flush:
	mov eax,0x0018
	mov ds,eax	; data 当把选择子传给段寄存器时，处理器在完成传入操作之前，会判断选择子的偏移+7是否超过
			;了描述符表的最大偏移，并且还有判断选择子对应的段信息是否与被传入的段寄存器类型符合（可读可				;写，代码或者数据段，都有规定）
	mov eax,0x0008	;寻址0-4G的数据段
	mov es,eax
    	mov fs,eax
         	mov gs,eax
	
	mov eax,0x0020
	mov ss,eax
	xor esp,esp	;偏移清零

	mov dword [es:0x0b8000],0x072e0750 ;字符'P'、'.'及其显示属性
         	mov dword [es:0x0b8004],0x072e074d ;字符'M'、'.'及其显示属性
         	mov dword [es:0x0b8008],0x07200720 ;两个空白字符及其显示属性
         	mov dword [es:0x0b800c],0x076b076f ;字符'o'、'k'及其显示属性
	

	;开始冒泡排序
	mov ecx,gdt_size-string-1	;遍历次数
@@1:
	push ecx
	xor bx,bx
@@2:
	mov ax,[string+bx] ;al是前一个字符，ah后一个高字节
	cmp ah,al
	jge @@3;ah大于al时跳转
	xchg al,ah
	mov [string+bx],ax;传回去
@@3:
	inc bx
	loop @@2
	pop ecx	;在内循环ecx会变化，需要保护
	loop @@1;loop使ecx真正-1
	
	mov ecx,gdt_size-string
	xor ebx,ebx
@print:
;开始打印
	mov al,[string+ebx]
	mov ah,0x07
	mov [es:0xb80a0+ebx*2],ax
	inc ebx
	loop @print
	hlt

;=====================
string db 's0ke4or92xap3fv8giuzjcy5l1m7hd6bnqtw.'

gdt_size dw 0;GDT描述符的最大偏移
gdt_base dd 0x00007e00
times 510-($-$$) db 0
	              db 0x55,0xaa
