	.file	"switch.c"
	.text
	.globl	func
	.type	func, @function
func:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	imull	-4(%rbp), %eax
	cmpl	$2, %eax
	je	.L2
	cmpl	$2, %eax
	jg	.L3
	cmpl	$1, %eax
	je	.L4
	jmp	.L5
.L3:
	cmpl	$3, %eax
	je	.L6
	cmpl	$4, %eax
	je	.L7
	jmp	.L5
.L4:
	movl	$0, %eax
	jmp	.L8
.L2:
	movl	$10, %eax
	jmp	.L8
.L6:
	movl	$100, %eax
	jmp	.L8
.L7:
	movl	$1000, %eax
	jmp	.L8
.L5:
	movl	$-1, %eax
.L8:
	popq	%rbp
	ret
	.size	func, .-func
	.ident	"GCC: (Ubuntu 8.3.0-6ubuntu1) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
