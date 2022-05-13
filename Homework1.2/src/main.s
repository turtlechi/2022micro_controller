	.syntax unified
	.cpu cortex-m4
	.thumb

.data

.text

	.global main

main:

 	movs r0, #0X5
	movs r1, #0X9
	mul r2, r0, r1
