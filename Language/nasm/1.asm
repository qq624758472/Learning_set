section .text
		global _start;
	_start:
		mov eax, 4
		mov ebx, 1
		mov ecx, string
		mov edx, length
		int 80h
		
		;exit的系统调用
		mov eax, 1
		mov ebx, 0
		int 80h
		
	section .data
		string:	db 'hello world',0Ah
		length: equ 13
		
	section .bss
		var:resb 1
