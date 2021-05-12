
.global _start
	
_start:
	ldr R0,=0x56000050 		@将r0设置成GPFCON寄存器。此寄存器用来选择端口b各引脚的功能：输出，输入，其他。

	mov r1,#0x00000100
	str r1,[r0]				@设置GPF4为输出口，位【8，7】=0b01

	ldr r0,=0x56000054   	@


	str r1,[r0]

main_loop:
	b main_loop