	.file	"editor.c"
	.text
	.globl	E
	.bss
	.align 32
	.type	E, @object
	.size	E, 104
E:
	.zero	104
	.section	.rodata
.LC0:
	.string	"Hello World!"
	.text
	.globl	editorOpen
	.type	editorOpen, @function
editorOpen:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	leaq	.LC0(%rip), %rax
	movq	%rax, -16(%rbp)
	movq	$13, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	%eax, 24+E(%rip)
	movq	-8(%rbp), %rax
	addq	$1, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, 32+E(%rip)
	movq	32+E(%rip), %rax
	movq	-8(%rbp), %rdx
	movq	-16(%rbp), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	memcpy@PLT
	movq	32+E(%rip), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	movl	$1, 16+E(%rip)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	editorOpen, .-editorOpen
	.globl	abAppend
	.type	abAppend, @function
abAppend:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	movq	-24(%rbp), %rax
	movl	8(%rax), %edx
	movl	-36(%rbp), %eax
	addl	%edx, %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	realloc@PLT
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L5
	movl	-36(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	movl	8(%rax), %eax
	movslq	%eax, %rcx
	movq	-8(%rbp), %rax
	addq	%rax, %rcx
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	movq	%rcx, %rdi
	call	memcpy@PLT
	movq	-24(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, (%rax)
	movq	-24(%rbp), %rax
	movl	8(%rax), %edx
	movl	-36(%rbp), %eax
	addl	%eax, %edx
	movq	-24(%rbp), %rax
	movl	%edx, 8(%rax)
	jmp	.L2
.L5:
	nop
.L2:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	abAppend, .-abAppend
	.globl	abFree
	.type	abFree, @function
abFree:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	free@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	abFree, .-abFree
	.section	.rodata
.LC1:
	.string	"getWindowSize"
	.text
	.globl	initEditor
	.type	initEditor, @function
initEditor:
.LFB9:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, E(%rip)
	movl	$0, 4+E(%rip)
	movl	$0, 16+E(%rip)
	leaq	12+E(%rip), %rax
	movq	%rax, %rsi
	leaq	8+E(%rip), %rax
	movq	%rax, %rdi
	call	getWindowSize
	cmpl	$-1, %eax
	jne	.L9
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	call	die
.L9:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	initEditor, .-initEditor
	.section	.rodata
.LC2:
	.string	"\033[6n"
.LC3:
	.string	"%d;%d"
	.text
	.globl	getCursorPosition
	.type	getCursorPosition, @function
getCursorPosition:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%rdi, -72(%rbp)
	movq	%rsi, -80(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -52(%rbp)
	movl	$4, %edx
	leaq	.LC2(%rip), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	write@PLT
	cmpq	$4, %rax
	je	.L13
	movl	$-1, %eax
	jmp	.L21
.L17:
	leaq	-48(%rbp), %rdx
	movl	-52(%rbp), %eax
	addq	%rdx, %rax
	movl	$1, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	read@PLT
	cmpq	$1, %rax
	jne	.L23
	movl	-52(%rbp), %eax
	movzbl	-48(%rbp,%rax), %eax
	cmpb	$82, %al
	je	.L24
	addl	$1, -52(%rbp)
.L13:
	cmpl	$30, -52(%rbp)
	jbe	.L17
	jmp	.L15
.L23:
	nop
	jmp	.L15
.L24:
	nop
.L15:
	movl	-52(%rbp), %eax
	movb	$0, -48(%rbp,%rax)
	movzbl	-48(%rbp), %eax
	cmpb	$27, %al
	jne	.L18
	movzbl	-47(%rbp), %eax
	cmpb	$91, %al
	je	.L19
.L18:
	movl	$-1, %eax
	jmp	.L21
.L19:
	movq	-80(%rbp), %rdx
	movq	-72(%rbp), %rax
	leaq	-48(%rbp), %rcx
	leaq	2(%rcx), %rdi
	movq	%rdx, %rcx
	movq	%rax, %rdx
	leaq	.LC3(%rip), %rax
	movq	%rax, %rsi
	movl	$0, %eax
	call	__isoc99_sscanf@PLT
	cmpl	$2, %eax
	je	.L20
	movl	$-1, %eax
	jmp	.L21
.L20:
	movl	$0, %eax
.L21:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L22
	call	__stack_chk_fail@PLT
.L22:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	getCursorPosition, .-getCursorPosition
	.section	.rodata
.LC4:
	.string	"\033[999C\033[999B"
	.text
	.globl	getWindowSize
	.type	getWindowSize, @function
getWindowSize:
.LFB11:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	-16(%rbp), %rax
	movq	%rax, %rdx
	movl	$21523, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	ioctl@PLT
	cmpl	$-1, %eax
	je	.L26
	movzwl	-14(%rbp), %eax
	testw	%ax, %ax
	jne	.L27
.L26:
	movl	$12, %edx
	leaq	.LC4(%rip), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	write@PLT
	cmpq	$12, %rax
	je	.L28
	movl	$-1, %eax
	jmp	.L30
.L28:
	movq	-32(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	getCursorPosition
	jmp	.L30
.L27:
	movzwl	-16(%rbp), %eax
	movzwl	%ax, %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	movzwl	-14(%rbp), %eax
	movzwl	%ax, %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	movl	$0, %eax
.L30:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L31
	call	__stack_chk_fail@PLT
.L31:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	getWindowSize, .-getWindowSize
	.globl	_die
	.type	_die, @function
_die:
.LFB12:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE12:
	.size	_die, .-_die
	.section	.rodata
.LC5:
	.string	"\033[2J"
.LC6:
	.string	"write"
.LC7:
	.string	"\033[H"
	.text
	.globl	die
	.type	die, @function
die:
.LFB13:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	$4, %edx
	leaq	.LC5(%rip), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	write@PLT
	cmpq	$-1, %rax
	jne	.L34
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	call	_die
.L34:
	movl	$3, %edx
	leaq	.LC7(%rip), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	write@PLT
	cmpq	$-1, %rax
	jne	.L35
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	call	_die
.L35:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	_die
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	die, .-die
	.section	.rodata
.LC8:
	.string	"tcsetattr"
	.text
	.globl	disableRawMode
	.type	disableRawMode, @function
disableRawMode:
.LFB14:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	40+E(%rip), %rax
	movq	%rax, %rdx
	movl	$2, %esi
	movl	$0, %edi
	call	tcsetattr@PLT
	cmpl	$-1, %eax
	jne	.L38
	leaq	.LC8(%rip), %rax
	movq	%rax, %rdi
	call	die
.L38:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	disableRawMode, .-disableRawMode
	.globl	enableRawMode
	.type	enableRawMode, @function
enableRawMode:
.LFB15:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	40+E(%rip), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	tcgetattr@PLT
	leaq	disableRawMode(%rip), %rax
	movq	%rax, %rdi
	call	atexit@PLT
	movq	40+E(%rip), %rax
	movq	48+E(%rip), %rdx
	movq	%rax, -80(%rbp)
	movq	%rdx, -72(%rbp)
	movq	56+E(%rip), %rax
	movq	64+E(%rip), %rdx
	movq	%rax, -64(%rbp)
	movq	%rdx, -56(%rbp)
	movq	72+E(%rip), %rax
	movq	80+E(%rip), %rdx
	movq	%rax, -48(%rbp)
	movq	%rdx, -40(%rbp)
	movq	84+E(%rip), %rax
	movq	92+E(%rip), %rdx
	movq	%rax, -36(%rbp)
	movq	%rdx, -28(%rbp)
	movl	-80(%rbp), %eax
	andl	$-1331, %eax
	movl	%eax, -80(%rbp)
	movl	-76(%rbp), %eax
	andl	$-2, %eax
	movl	%eax, -76(%rbp)
	movl	-72(%rbp), %eax
	orl	$48, %eax
	movl	%eax, -72(%rbp)
	movl	-68(%rbp), %eax
	andl	$-32780, %eax
	movl	%eax, -68(%rbp)
	movb	$0, -57(%rbp)
	movb	$1, -58(%rbp)
	leaq	-80(%rbp), %rax
	movq	%rax, %rdx
	movl	$2, %esi
	movl	$0, %edi
	call	tcsetattr@PLT
	cmpl	$-1, %eax
	jne	.L42
	leaq	.LC8(%rip), %rax
	movq	%rax, %rdi
	call	die
.L42:
	nop
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L41
	call	__stack_chk_fail@PLT
.L41:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	enableRawMode, .-enableRawMode
	.section	.rodata
.LC9:
	.string	"read"
	.text
	.globl	editorReadKey
	.type	editorReadKey, @function
editorReadKey:
.LFB16:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	jmp	.L44
.L46:
	cmpq	$-1, -24(%rbp)
	jne	.L44
	call	__errno_location@PLT
	movl	(%rax), %eax
	cmpl	$11, %eax
	je	.L44
	leaq	.LC9(%rip), %rax
	movq	%rax, %rdi
	call	die
.L44:
	leaq	-25(%rbp), %rax
	movl	$1, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	read@PLT
	movq	%rax, -24(%rbp)
	cmpq	$1, -24(%rbp)
	jne	.L46
	movzbl	-25(%rbp), %eax
	cmpb	$27, %al
	jne	.L47
	leaq	-11(%rbp), %rax
	movl	$1, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	read@PLT
	cmpq	$1, %rax
	je	.L48
	movl	$27, %eax
	jmp	.L73
.L48:
	leaq	-11(%rbp), %rax
	addq	$1, %rax
	movl	$1, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	read@PLT
	cmpq	$1, %rax
	je	.L50
	movl	$27, %eax
	jmp	.L73
.L50:
	movzbl	-11(%rbp), %eax
	cmpb	$91, %al
	jne	.L51
	movzbl	-10(%rbp), %eax
	cmpb	$47, %al
	jle	.L52
	movzbl	-10(%rbp), %eax
	cmpb	$57, %al
	jg	.L52
	leaq	-11(%rbp), %rax
	addq	$2, %rax
	movl	$1, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	read@PLT
	cmpq	$1, %rax
	je	.L53
	movl	$27, %eax
	jmp	.L73
.L53:
	movzbl	-9(%rbp), %eax
	cmpb	$126, %al
	jne	.L75
	movzbl	-10(%rbp), %eax
	movsbl	%al, %eax
	subl	$49, %eax
	cmpl	$7, %eax
	ja	.L75
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L56(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L56(%rip), %rdx
	addq	%rdx, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L56:
	.long	.L62-.L56
	.long	.L75-.L56
	.long	.L61-.L56
	.long	.L60-.L56
	.long	.L59-.L56
	.long	.L58-.L56
	.long	.L57-.L56
	.long	.L55-.L56
	.text
.L62:
	movl	$1005, %eax
	jmp	.L73
.L61:
	movl	$1004, %eax
	jmp	.L73
.L60:
	movl	$1006, %eax
	jmp	.L73
.L59:
	movl	$1007, %eax
	jmp	.L73
.L58:
	movl	$1008, %eax
	jmp	.L73
.L57:
	movl	$1005, %eax
	jmp	.L73
.L55:
	movl	$1006, %eax
	jmp	.L73
.L52:
	movzbl	-10(%rbp), %eax
	movsbl	%al, %eax
	subl	$65, %eax
	cmpl	$7, %eax
	ja	.L71
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L65(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L65(%rip), %rdx
	addq	%rdx, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L65:
	.long	.L70-.L65
	.long	.L69-.L65
	.long	.L68-.L65
	.long	.L67-.L65
	.long	.L71-.L65
	.long	.L66-.L65
	.long	.L71-.L65
	.long	.L64-.L65
	.text
.L70:
	movl	$1002, %eax
	jmp	.L73
.L69:
	movl	$1003, %eax
	jmp	.L73
.L68:
	movl	$1001, %eax
	jmp	.L73
.L67:
	movl	$1000, %eax
	jmp	.L73
.L64:
	movl	$1005, %eax
	jmp	.L73
.L66:
	movl	$1006, %eax
	jmp	.L73
.L51:
	movzbl	-11(%rbp), %eax
	cmpb	$79, %al
	jne	.L71
	movzbl	-10(%rbp), %eax
	movsbl	%al, %eax
	cmpl	$70, %eax
	je	.L72
	cmpl	$72, %eax
	jne	.L71
	movl	$1005, %eax
	jmp	.L73
.L72:
	movl	$1006, %eax
	jmp	.L73
.L75:
	nop
.L71:
	movl	$27, %eax
	jmp	.L73
.L47:
	movzbl	-25(%rbp), %eax
	movsbl	%al, %eax
.L73:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L74
	call	__stack_chk_fail@PLT
.L74:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	editorReadKey, .-editorReadKey
	.globl	editorMoveCursor
	.type	editorMoveCursor, @function
editorMoveCursor:
.LFB17:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	cmpl	$1003, -4(%rbp)
	je	.L77
	cmpl	$1003, -4(%rbp)
	jg	.L86
	cmpl	$1002, -4(%rbp)
	je	.L79
	cmpl	$1002, -4(%rbp)
	jg	.L86
	cmpl	$1000, -4(%rbp)
	je	.L80
	cmpl	$1001, -4(%rbp)
	je	.L81
	jmp	.L86
.L80:
	movl	E(%rip), %eax
	testl	%eax, %eax
	je	.L87
	movl	E(%rip), %eax
	subl	$1, %eax
	movl	%eax, E(%rip)
	jmp	.L87
.L81:
	movl	E(%rip), %edx
	movl	12+E(%rip), %eax
	subl	$1, %eax
	cmpl	%eax, %edx
	je	.L88
	movl	E(%rip), %eax
	addl	$1, %eax
	movl	%eax, E(%rip)
	jmp	.L88
.L79:
	movl	4+E(%rip), %eax
	testl	%eax, %eax
	je	.L89
	movl	4+E(%rip), %eax
	subl	$1, %eax
	movl	%eax, 4+E(%rip)
	jmp	.L89
.L77:
	movl	4+E(%rip), %edx
	movl	8+E(%rip), %eax
	subl	$1, %eax
	cmpl	%eax, %edx
	je	.L90
	movl	4+E(%rip), %eax
	addl	$1, %eax
	movl	%eax, 4+E(%rip)
	jmp	.L90
.L87:
	nop
	jmp	.L86
.L88:
	nop
	jmp	.L86
.L89:
	nop
	jmp	.L86
.L90:
	nop
.L86:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	editorMoveCursor, .-editorMoveCursor
	.globl	editorProcessKeypress
	.type	editorProcessKeypress, @function
editorProcessKeypress:
.LFB18:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	call	editorReadKey
	movl	%eax, -4(%rbp)
	cmpl	$1008, -4(%rbp)
	jg	.L103
	cmpl	$1007, -4(%rbp)
	jge	.L93
	cmpl	$1006, -4(%rbp)
	je	.L94
	cmpl	$1006, -4(%rbp)
	jg	.L103
	cmpl	$1005, -4(%rbp)
	je	.L95
	cmpl	$1005, -4(%rbp)
	jg	.L103
	cmpl	$17, -4(%rbp)
	je	.L96
	cmpl	$17, -4(%rbp)
	jl	.L103
	movl	-4(%rbp), %eax
	subl	$1000, %eax
	cmpl	$3, %eax
	ja	.L103
	jmp	.L102
.L96:
	movl	$4, %edx
	leaq	.LC5(%rip), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	write@PLT
	movl	$3, %edx
	leaq	.LC7(%rip), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	write@PLT
	movl	$0, %edi
	call	exit@PLT
.L95:
	movl	$0, E(%rip)
	jmp	.L92
.L94:
	movl	12+E(%rip), %eax
	subl	$1, %eax
	movl	%eax, E(%rip)
	jmp	.L92
.L93:
	movl	8+E(%rip), %eax
	movl	%eax, -8(%rbp)
	jmp	.L98
.L101:
	cmpl	$1007, -4(%rbp)
	jne	.L99
	movl	$1002, %eax
	jmp	.L100
.L99:
	movl	$1003, %eax
.L100:
	movl	%eax, %edi
	call	editorMoveCursor
.L98:
	movl	-8(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -8(%rbp)
	testl	%eax, %eax
	jne	.L101
	jmp	.L92
.L102:
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	editorMoveCursor
	nop
.L92:
.L103:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	editorProcessKeypress, .-editorProcessKeypress
	.section	.rodata
.LC10:
	.string	"\033[?25l"
.LC11:
	.string	"\033[%d;%dH"
.LC12:
	.string	"\033[?25h"
	.text
	.globl	editorRefreshScreen
	.type	editorRefreshScreen, @function
editorRefreshScreen:
.LFB19:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$0, -64(%rbp)
	movl	$0, -56(%rbp)
	leaq	-64(%rbp), %rax
	movl	$6, %edx
	leaq	.LC10(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	abAppend
	leaq	-64(%rbp), %rax
	movl	$3, %edx
	leaq	.LC7(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	abAppend
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	editorDrawRows
	movl	E(%rip), %eax
	leal	1(%rax), %ecx
	movl	4+E(%rip), %eax
	leal	1(%rax), %edx
	leaq	-48(%rbp), %rax
	movl	%ecx, %r8d
	movl	%edx, %ecx
	leaq	.LC11(%rip), %rdx
	movl	$32, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf@PLT
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movl	%eax, %edx
	leaq	-48(%rbp), %rcx
	leaq	-64(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	abAppend
	leaq	-64(%rbp), %rax
	movl	$6, %edx
	leaq	.LC12(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	abAppend
	movl	-56(%rbp), %eax
	movslq	%eax, %rdx
	movq	-64(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	write@PLT
	cmpq	$-1, %rax
	jne	.L105
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	call	die
.L105:
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	abFree
	nop
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L106
	call	__stack_chk_fail@PLT
.L106:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	editorRefreshScreen, .-editorRefreshScreen
	.section	.rodata
.LC13:
	.string	"0.0.1"
.LC14:
	.string	"ibredit --version %s"
.LC15:
	.string	"~"
.LC16:
	.string	" "
.LC17:
	.string	"\033[K"
.LC18:
	.string	"\r\n"
	.text
	.globl	editorDrawRows
	.type	editorDrawRows, @function
editorDrawRows:
.LFB20:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movq	%rdi, -120(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -112(%rbp)
	jmp	.L108
.L118:
	movl	16+E(%rip), %eax
	cmpl	%eax, -112(%rbp)
	jl	.L109
	movl	8+E(%rip), %eax
	movslq	%eax, %rdx
	imulq	$1431655766, %rdx, %rdx
	movq	%rdx, %rcx
	shrq	$32, %rcx
	cltd
	movl	%ecx, %eax
	subl	%edx, %eax
	cmpl	%eax, -112(%rbp)
	jne	.L110
	leaq	-96(%rbp), %rax
	leaq	.LC13(%rip), %rdx
	movq	%rdx, %rcx
	leaq	.LC14(%rip), %rdx
	movl	$88, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf@PLT
	movl	%eax, -108(%rbp)
	movl	12+E(%rip), %eax
	cmpl	%eax, -108(%rbp)
	jle	.L111
	movl	12+E(%rip), %eax
	movl	%eax, -108(%rbp)
.L111:
	movl	12+E(%rip), %eax
	subl	-108(%rbp), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, -104(%rbp)
	cmpl	$0, -104(%rbp)
	je	.L113
	movq	-120(%rbp), %rax
	movl	$1, %edx
	leaq	.LC15(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	abAppend
	subl	$1, -104(%rbp)
	jmp	.L113
.L114:
	movq	-120(%rbp), %rax
	movl	$1, %edx
	leaq	.LC16(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	abAppend
.L113:
	movl	-104(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -104(%rbp)
	testl	%eax, %eax
	jne	.L114
	movl	-108(%rbp), %edx
	leaq	-96(%rbp), %rcx
	movq	-120(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	abAppend
	jmp	.L115
.L110:
	movq	-120(%rbp), %rax
	movl	$1, %edx
	leaq	.LC15(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	abAppend
	jmp	.L115
.L109:
	movl	24+E(%rip), %eax
	movl	%eax, -100(%rbp)
	movl	12+E(%rip), %eax
	cmpl	%eax, -100(%rbp)
	jle	.L116
	movl	12+E(%rip), %eax
	movl	%eax, -100(%rbp)
.L116:
	movq	32+E(%rip), %rcx
	movl	-100(%rbp), %edx
	movq	-120(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	abAppend
.L115:
	movq	-120(%rbp), %rax
	movl	$3, %edx
	leaq	.LC17(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	abAppend
	movl	8+E(%rip), %eax
	subl	$1, %eax
	cmpl	%eax, -112(%rbp)
	jge	.L117
	movq	-120(%rbp), %rax
	movl	$2, %edx
	leaq	.LC18(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	abAppend
.L117:
	addl	$1, -112(%rbp)
.L108:
	movl	8+E(%rip), %eax
	cmpl	%eax, -112(%rbp)
	jl	.L118
	nop
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L119
	call	__stack_chk_fail@PLT
.L119:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	editorDrawRows, .-editorDrawRows
	.globl	main
	.type	main, @function
main:
.LFB21:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	enableRawMode
	call	initEditor
	call	editorOpen
.L121:
	call	editorRefreshScreen
	call	editorProcessKeypress
	nop
	jmp	.L121
	.cfi_endproc
.LFE21:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
