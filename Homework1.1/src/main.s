	.syntax unified
	.cpu cortex-m4
	.thumb

.data

.text

	.global main

main:

 	movs r0, #0X20
	movs r1, #0X10
	subs r2, r0, r1
