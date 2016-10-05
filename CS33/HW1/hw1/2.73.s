	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_saturating_add
	.align	4, 0x90
_saturating_add:                        ## @saturating_add
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	leal	(%rsi,%rdi), %ecx
	movl	%esi, %edx
	andl	%edi, %edx
	orl	%edi, %esi
	movl	%ecx, %eax
	sarl	$31, %eax
	sarl	$31, %esi
	notl	%esi
	andl	%eax, %esi
	sarl	$31, %edx
	notl	%eax
	andl	%edx, %eax
	movl	%esi, %edx
	orl	%eax, %edx
	notl	%edx
	andl	%ecx, %edx
	andl	$2147483647, %esi       ## imm = 0x7FFFFFFF
	andl	$-2147483648, %eax      ## imm = 0xFFFFFFFF80000000
	orl	%esi, %eax
	orl	%edx, %eax
	popq	%rbp
	retq
	.cfi_endproc


.subsections_via_symbols
