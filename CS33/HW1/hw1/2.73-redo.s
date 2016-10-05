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
	addl	%esi, %edi
	jno	LBB0_2
## BB#1:
	sarl	$31, %edi
	xorl	$-2147483648, %edi      ## imm = 0xFFFFFFFF80000000
LBB0_2:
	movl	%edi, %eax
	popq	%rbp
	retq
	.cfi_endproc


.subsections_via_symbols
