	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_any_odd_one
	.align	4, 0x90
_any_odd_one:                           ## @any_odd_one
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
	testl	$-1431655766, %edi      ## imm = 0xFFFFFFFFAAAAAAAA
	setne	%al
	movzbl	%al, %eax
	popq	%rbp
	retq
	.cfi_endproc


.subsections_via_symbols
