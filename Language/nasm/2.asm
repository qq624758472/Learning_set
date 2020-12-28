section .text
		global _start;
	_start:
		mov ax,0xb800                 ;指向文本模式的显示缓冲区
        mov es,ax

		mov eax, 4
		mov ebx, 1        

		;往显存中放数据
		

		mov ecx, string
		mov edx, length
		int 80h

        jmp end1
		
    end:
        mov ax, 4c00H
        int 21H
    end1:
        ;;exit的系统调用
		mov eax, 1
		mov ebx, 0
		int 80h
		
section .data
	string:	db 'hello world',0Ah
	length: equ 12
		
; section .bss
; 	var:resb 1
