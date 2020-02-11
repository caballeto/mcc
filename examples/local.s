	.file	"local.c"
	.text
	.globl	func
	.type	func, @function
func:
	pushq	%rbp
	movq	%rsp, %rbp
	movb	$0, -1(%rbp)
	movb	$0, -2(%rbp)
	movl	$0, -8(%rbp)
	movl	$0, -12(%rbp)
	movq	$0, -24(%rbp)
	movq	$0, -32(%rbp)
	movsbl	-1(%rbp), %edx
	movsbl	-2(%rbp), %eax
	imull	%edx, %eax
	movl	%eax, -8(%rbp)
	movsbl	-1(%rbp), %edx
	movsbl	-2(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -12(%rbp)
	movl	-8(%rbp), %eax
	subl	-12(%rbp), %eax
	popq	%rbp
	ret
	.size	func, .-func
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$728, %rsp
	movl	$1, -832(%rbp)
	movl	$2, -828(%rbp)
	movl	$3, -824(%rbp)
	movl	$4, -820(%rbp)
	movl	$5, -816(%rbp)
	movq	-800(%rbp), %rax
	movl	%eax, %edx
	movl	-832(%rbp), %eax
	addl	%eax, %edx
	movzbl	-837(%rbp), %eax
	movsbl	%al, %eax
	addl	%edx, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 8.3.0-6ubuntu1) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
