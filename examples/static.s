	.file	"static.c"
	.text
	.local	x
	.comm	x,4,4
	.type	func, @function
func:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	x(%rip), %edx
	movl	x(%rip), %eax
	imull	%eax, %edx
	movl	-4(%rbp), %eax
	imull	-4(%rbp), %eax
	addl	%edx, %eax
	popq	%rbp
	ret
	.size	func, .-func
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$10, x(%rip)
	movl	$10, %edi
	call	func
	movl	$0, %eax
	popq	%rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 8.3.0-6ubuntu1) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
