	.file	"test_c.c"
	.text
	.section	.rodata
.LC0:
	.string	"%s,%d\n"
	.text
	.type	arm_cpu_register_types, @function
arm_cpu_register_types:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$8, %edx
	leaq	__FUNCTION__.2706(%rip), %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	arm_cpu_register_types, .-arm_cpu_register_types
	.type	do_qemu_init_arm_cpu_register_types, @function
do_qemu_init_arm_cpu_register_types:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$11, %edx
	leaq	__FUNCTION__.2710(%rip), %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	do_qemu_init_arm_cpu_register_types, .-do_qemu_init_arm_cpu_register_types
	.section	.init_array,"aw"
	.align 8
	.quad	do_qemu_init_arm_cpu_register_types
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.section	.rodata
	.align 16
	.type	__FUNCTION__.2706, @object
	.size	__FUNCTION__.2706, 23
__FUNCTION__.2706:
	.string	"arm_cpu_register_types"
	.align 32
	.type	__FUNCTION__.2710, @object
	.size	__FUNCTION__.2710, 36
__FUNCTION__.2710:
	.string	"do_qemu_init_arm_cpu_register_types"
	.ident	"GCC: (Debian 7.4.0-6) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
