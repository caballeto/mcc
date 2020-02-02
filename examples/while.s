	.file	"while.c"
	.text
	.globl	While
	.type	While, @function
While:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
.L2:
	addl	$1, -4(%rbp)
	cmpl	$9, -4(%rbp)
	jle	.L2
	movl	-4(%rbp), %eax
	popq	%rbp
	ret
	.size	While, .-While
	.globl	For
	.type	For, @function
For:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -20(%rbp)
	movl	$0, -12(%rbp)
	movl	$0, -8(%rbp)
	movl	$1, -16(%rbp)
	movl	$1, -4(%rbp)
	jmp	.L5
.L6:
	movl	-16(%rbp), %eax
	addl	%eax, -20(%rbp)
	addl	$1, -16(%rbp)
.L5:
	cmpl	$9, -20(%rbp)
	jle	.L6
	movl	-20(%rbp), %eax
	popq	%rbp
	ret
	.size	For, .-For
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$0, %edi
	call	While
	movl	$0, %eax
	popq	%rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 8.3.0-6ubuntu1) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
