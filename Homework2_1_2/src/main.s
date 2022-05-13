.syntax unified
.cpu cortex-m4
.thumb

// r0 = value
.text
	.global main
	.include "../src/pin.s"

GPIO_init:
    // Enable AHB2 clock
    movs r0, #0x6
    ldr r1, =RCC_AHB2ENR
    str r0, [r1]
    // Set PB3 ~ PB6 to output mode
   
    ldr r1, =GPIOC_MODER
    ldr r0, [r1]
    and r0, #0x000F
    orr r0, #0x0550
    str r0, [r1]
   
   
    ldr r1, =GPIOC_OTYPER
    ldr r0, [r1]
    mov r0, #0x03c0
    str r0, [r1]
    
    //display led
	ldr r1, =GPIOC_ODR
    ldr r0, [r1]          // Get Current Output
    and r0, #0xFFFFF00F   // Clear PB3 to PB6
    orr r0, #0x0100   // Reverse r0 (Active Low) (FFFFFFBF vs. BF)?
    str r0, [r1]       // Select the needed

main:
    bl GPIO_init    // Setup GPIO
  


