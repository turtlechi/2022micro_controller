.syntax unified
.cpu cortex-m4
.thumb

.text
	.global main
	.include "../src/pin.s"

// r0 = value


GPIO_init:
    // Enable AHB2 clock
    movs r0, #0x6
    ldr r1, =RCC_AHB2ENR
    str r0, [r1]
    // Set PB3 ~ PB6 to output mode
    ldr r1, =GPIOB_MODER
    ldr r0, [r1]
    and r0, #0xFFFFC03F
    orr r0, #0x1540
    str r0, [r1]
   

    ldr r1, =GPIOB_OTYPER
    ldr r0, [r1]
    and r0, #0xFFFFFF87
    orr r0, #0x0000
    str r0, [r1]
    
    //display led
	ldr r1, =GPIOB_ODR    
    ldr r0, [r1]          // Get Current Output
    and r0, #0xFFFFFF87   // Clear PB3 to PB6
    orr r0, #0xBF    // Reverse r0 (Active Low) (FFFFFFBF vs. BF)?
    str r0, [r1]       // Select the needed

main:
    bl GPIO_init    // Setup GPIO
  


