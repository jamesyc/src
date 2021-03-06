#!/bin/sh -

PADBYTE=$1

cat << __EOF__
#include <machine/param.h>
#include <machine/asm.h>

	.text
	.balign	PAGE_SIZE, $PADBYTE
	.space	$RANDOM, $PADBYTE
	.balign	PAGE_SIZE, $PADBYTE

	.globl	endboot
endboot:
	.space	PAGE_SIZE, $PADBYTE
	.space	$RANDOM % PAGE_SIZE, $PADBYTE
	.balign	16, $PADBYTE

	/*
	 * Randomly bias future data, bss, and rodata objects,
	 * does not help for objects in locore0.S though
	  */
	.data
	.space	$RANDOM % PAGE_SIZE, $PADBYTE

	.section .bss
	.space	$RANDOM % PAGE_SIZE

	.section .rodata
	.space	$RANDOM % PAGE_SIZE, $PADBYTE
__EOF__
