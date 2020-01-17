	.file	"comp.c"
	.text
	.globl	compare
	.type	compare, @function
compare:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -36(%rbp)
	movl	%esi, -40(%rbp)
	movl	-36(%rbp), %eax
	cmpl	-40(%rbp), %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -24(%rbp)
	movl	-36(%rbp), %eax
	cmpl	-40(%rbp), %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -20(%rbp)
	movl	-36(%rbp), %eax
	cmpl	-40(%rbp), %eax
	setl	%al
	movzbl	%al, %eax
	movl	%eax, -16(%rbp)
	movl	-36(%rbp), %eax
	cmpl	-40(%rbp), %eax
	setle	%al
	movzbl	%al, %eax
	movl	%eax, -12(%rbp)
	movl	-36(%rbp), %eax
	cmpl	-40(%rbp), %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, -8(%rbp)
	movl	-36(%rbp), %eax
	cmpl	-40(%rbp), %eax
	setge	%al
	movzbl	%al, %eax
	movl	%eax, -4(%rbp)
	movl	-24(%rbp), %edx
	movl	-20(%rbp), %eax
	addl	%eax, %edx
	movl	-16(%rbp), %eax
	addl	%eax, %edx
	movl	-12(%rbp), %eax
	addl	%eax, %edx
	movl	-8(%rbp), %eax
	addl	%eax, %edx
	movl	-4(%rbp), %eax
	addl	%edx, %eax
	popq	%rbp
	ret
	.size	compare, .-compare
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$0, %eax
	popq	%rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 8.3.0-6ubuntu1) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
