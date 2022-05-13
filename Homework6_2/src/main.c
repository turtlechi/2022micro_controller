#include "stm32l476xx.h"
#include "helper_functions.h"
#include "7seg.h"
#include "keypad.h"
#include "led_button.h"
#include "timer.h"

// Define pins for 7seg
#define SEG_gpio GPIOC
#define DIN_pin 1
#define CS_pin 2
#define CLK_pin 3

// Define pins for keypad
// If need to change need to also change EXTI_Setup and IRQHandler
#define COL_gpio GPIOA
#define COL_pin 6       // 6 7 8 9
#define ROW_gpio GPIOB
#define ROW_pin 3       // 3 4 5 6

// Define pins for led (default use on-board led PA5)
#define LED_gpio GPIOA
#define LED_pin 5

// Define pins for button (default use on-board button PC13)
#define BUTTON_gpio GPIOC
#define BUTTON_pin 13

// Define Counter timer
#define COUNTER_timer TIM2

int now_col=3;
//used for debounce
int keyCnt=0,keyValue=-1;

// Buzzer is fixed to PA0 due to its need for PWM signal
// Can change to other ports if needed, but need to look up the reference

// Use to decide which part of the code will run
// Use define & ifdef to control
#define lab_systick_blink
//#define lab_interrupt_keypad
//#define lab_alarm

// Remember to use correct "startup_stm32.s"


void SysTick_Handler() {

//#ifdef lab_systick_blink
 if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
  reset_push(COL_gpio,now_col+COL_pin);
  now_col=(now_col+1)%4;
  set_push(COL_gpio,now_col+COL_pin);
 }
//#endif
}
void EXTI_Setup(){
 //enable syscfg clk
 RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
 //select output bits
 SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3_Msk;
 SYSCFG->EXTICR[0] &= (1 << SYSCFG_EXTICR1_EXTI3_Pos);
 SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR2_EXTI4_Msk;
 SYSCFG->EXTICR[0] &= (1 << SYSCFG_EXTICR2_EXTI4_Pos);
 SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR2_EXTI5_Msk;
 SYSCFG->EXTICR[0] &= (1 << SYSCFG_EXTICR2_EXTI5_Pos);
 SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR2_EXTI6_Msk;
 SYSCFG->EXTICR[0] &= (1 << SYSCFG_EXTICR2_EXTI6_Pos);

 // Enable interrupt
 EXTI->IMR1 |= EXTI_IMR1_IM3; //0b0001<<3
 EXTI->IMR1 |= EXTI_IMR1_IM4; //0b0001<<4
 EXTI->IMR1 |= EXTI_IMR1_IM5;
 EXTI->IMR1 |= EXTI_IMR1_IM6;

 // Enable Falling Edge

 EXTI->FTSR1 |= EXTI_FTSR1_FT3; //0b0001<<3
 EXTI->FTSR1 |= EXTI_FTSR1_FT4; //060001<<4
 EXTI->FTSR1 |= EXTI_FTSR1_FT5;
 EXTI->FTSR1 |= EXTI_FTSR1_FT6;

 // Enable NVIC**
 NVIC_EnableIRQ(EXTI3_IRQn);
 NVIC_EnableIRQ(EXTI4_IRQn);
 NVIC_EnableIRQ(EXTI9_5_IRQn);

}
void EXTIKeypadHandler(int r){
     int nowkey = keypad[r][(now_col +3)% 4];
     // A simple debounce
     if(nowkey == keyValue){
        keyCnt++;
     }
     else{
        keyCnt = 0;
     }
     keyValue = nowkey;
     if(keyCnt >= 5){
        keyCnt = 5;
        display_number(SEG_gpio, DIN_pin,CS_pin,CLK_pin,keyValue,2);
     }
}

void EXTI3_IRQHandler(){
    if(EXTI->PR1 & EXTI_PR1_PIF3_Msk){
       EXTIKeypadHandler(0);
       EXTI->PR1 = EXTI_PR1_PIF3_Msk;//Âk¹s
    }
}

void EXTI4_IROHandler(){
    if(EXTI->PR1 & EXTI_PR1_PIF4_Msk){
       EXTIKeypadHandler(1);
       EXTI->PR1 = EXTI_PR1_PIF4_Msk;
    }
}


void EXTI9_5_IROHandler(){
    if(EXTI->PR1 & EXTI_PR1_PIF5_Msk){
       EXTIKeypadHandler(2);
       EXTI->PR1 = EXTI_PR1_PIF5_Msk;
    }
    if(EXTI->PR1 & EXTI_PR1_PIF6_Msk){
       EXTIKeypadHandler(3);
       EXTI->PR1 = EXTI_PR1_PIF6_Msk;
    }
}


int main(){

	// Cause we want to use floating points we need to init FPU
	FPU_init();
	if(init_7seg_number(SEG_gpio,DIN_pin,CS_pin,CLK_pin)!=0){
			return -1;
	}
	if(init_keypad(ROW_gpio,COL_gpio,ROW_pin,COL_pin)!=0){
			return -1;
	}
 //10MHz 0.001s interrupt
	SystemClock_Config_Interrupt(10, 10000);
 //init interrupt
	EXTI_Setup();

	while(1){
	}

	return 0;



}
