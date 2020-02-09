	.file	"array.c"
	.text
	.comm	array,40,32
	.comm	z,4,4
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	leaq	array(%rip), %rax
	movq	%rax, -16(%rbp)
	leaq	array(%rip), %rax
	movq	%rax, -8(%rbp)
	movq	-16(%rbp), %rax
	addq	$4, %rax
	movl	$10, (%rax)
	movl	4+array(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$5, array(%rip)
	movl	array(%rip), %eax
	addl	$1, %eax
	movl	%eax, array(%rip)
	movl	array(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	array(%rip), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movl	$10, (%rax)
	movl	array(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 8.3.0-6ubuntu1) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
