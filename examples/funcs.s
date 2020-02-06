	.file	"funcs.c"
	.text
	.comm	y,4,4
	.globl	m
	.data
	.align 4
	.type	m, @object
	.size	m, 4
m:
	.long	5
	.globl	x
	.align 4
	.type	x, @object
	.size	x, 4
x:
	.long	5
	.text
	.globl	func
	.type	func, @function
func:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -20(%rbp)
	cmpl	$5, -20(%rbp)
	jne	.L2
	movl	-20(%rbp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	jmp	.L3
.L2:
	movl	$25, -4(%rbp)
	movl	-4(%rbp), %eax
	addl	%eax, -20(%rbp)
	movl	-4(%rbp), %eax
	subl	-20(%rbp), %eax
.L3:
	popq	%rbp
	ret
	.size	func, .-func
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$5, -28(%rbp)
	movb	$5, -29(%rbp)
	leaq	-28(%rbp), %rax
	movq	%rax, -24(%rbp)
	leaq	-24(%rbp), %rax
	movq	%rax, -16(%rbp)
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L6
	call	__stack_chk_fail@PLT
.L6:
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 8.3.0-6ubuntu1) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
