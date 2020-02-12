	.file	"args.c"
	.text
	.globl	myfunc
	.type	myfunc, @function
myfunc:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, %r10d
	movl	%esi, %edi
	movl	%edx, %esi
	movl	%r8d, %r11d
	movl	16(%rbp), %edx
	movl	24(%rbp), %eax
	movl	%r10d, %r8d
	movw	%r8w, -20(%rbp)
	movw	%di, -24(%rbp)
	movw	%si, -28(%rbp)
	movw	%cx, -32(%rbp)
	movl	%r11d, %ecx
	movw	%cx, -36(%rbp)
	movl	%r9d, %ecx
	movw	%cx, -40(%rbp)
	movw	%dx, -44(%rbp)
	movw	%ax, -48(%rbp)
	movzwl	-20(%rbp), %edx
	movzwl	-24(%rbp), %eax
	imull	%eax, %edx
	movzwl	-28(%rbp), %eax
	imull	%eax, %edx
	movzwl	-32(%rbp), %eax
	imull	%eax, %edx
	movzwl	-36(%rbp), %eax
	imull	%eax, %edx
	movzwl	-40(%rbp), %eax
	imull	%eax, %edx
	movzwl	-44(%rbp), %eax
	imull	%eax, %edx
	movzwl	-48(%rbp), %eax
	imull	%edx, %eax
	movw	%ax, -2(%rbp)
	movzwl	-20(%rbp), %edx
	movzwl	-24(%rbp), %eax
	addl	%eax, %edx
	movzwl	-28(%rbp), %eax
	addl	%eax, %edx
	movzwl	-32(%rbp), %eax
	addl	%eax, %edx
	movzwl	-36(%rbp), %eax
	addl	%eax, %edx
	movzwl	-40(%rbp), %eax
	addl	%eax, %edx
	movzwl	-44(%rbp), %eax
	addl	%eax, %edx
	movzwl	-48(%rbp), %eax
	addl	%edx, %eax
	movw	%ax, -4(%rbp)
	movzwl	-4(%rbp), %eax
	addl	$20, %eax
	popq	%rbp
	ret
	.size	myfunc, .-myfunc
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	pushq	$8
	pushq	$7
	movl	$6, %r9d
	movl	$5, %r8d
	movl	$4, %ecx
	movl	$3, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	myfunc
	addq	$16, %rsp
	movw	%ax, -2(%rbp)
	movswl	-2(%rbp), %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 8.3.0-6ubuntu1) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
