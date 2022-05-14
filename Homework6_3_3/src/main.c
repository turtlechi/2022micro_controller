#include "stm32l476xx.h"
#include "helper_functions.h"
#include "7seg.h"
#include "keypad.h"
#include "led_button.h"
#include "timer.h"
#include "clock.h"
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

// Buzzer is fixed to PA0 due to its need for PWM signal
// Can change to other ports if needed, but need to look up the reference

// Use to decide which part of the code will run
// Use define & ifdef to control

//#define lab_interrupt_keypad
//#define lab_alarm

// Remember to use correct "startup_stm32.s"

int now_col = 3;
int keyCnt = 0, keyValue = -1;
int num=1;


int EXTIKeypadHandler(int r){
	if(r==100){

	}
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
		num=second_dis(SEG_gpio, DIN_pin,CS_pin,CLK_pin,keyValue,0);
	}
	return num;
}
void SysTick_Handler() {


	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
		reset_push(COL_gpio, now_col+COL_pin);
		now_col = (now_col+1)%4;
		set_push(COL_gpio, now_col+COL_pin);
	}

}
void EXTI3_IRQHandler(){
    if(EXTI->PR1 & EXTI_PR1_PIF3_Msk){      //pr1是有沒有被觸發的flag，已經在exit裡面設定好用falling edge觸發，表示一旦按鍵按下去就會直接執行
       EXTIKeypadHandler(0);
       EXTI->PR1 = EXTI_PR1_PIF3_Msk;       //把flag歸零
    }
}
void EXTI4_IRQHandler(){
    if(EXTI->PR1 & EXTI_PR1_PIF4_Msk){
       EXTIKeypadHandler(1);
       EXTI->PR1 = EXTI_PR1_PIF4_Msk;
    }
}
void EXTI9_5_IRQHandler(){
    if(EXTI->PR1 & EXTI_PR1_PIF5_Msk){
       EXTIKeypadHandler(2);
       EXTI->PR1 = EXTI_PR1_PIF5_Msk;
    }
    if(EXTI->PR1 & EXTI_PR1_PIF6_Msk){
       EXTIKeypadHandler(3);
       EXTI->PR1 = EXTI_PR1_PIF6_Msk;
    }
}
void EXTI15_10_IRQHandler(){
    if(EXTI->PR1 & EXTI_PR1_PIF13_Msk){
    	if(RCC->APB1ENR1 & 1<<RCC_APB1ENR1_TIM2EN)
    	{
    		timer_disable(TIM2);
    	}
    	else{
    	keyValue= EXTIKeypadHandler(100);
    	countdown_timer(SEG_gpio, DIN_pin,CS_pin,CLK_pin,keyValue);
    	EXTI->PR1 = EXTI_PR1_PIF13_Msk;
    	}
    }
}



//Use for debounce

int main(){

	// Cause we want to use floating points we need to init FPU
	FPU_init();
	EXTI_Setup();
	music_Init();
	if(init_7seg_number(SEG_gpio,DIN_pin,CS_pin,CLK_pin)!=0){
			return -1;
	}
	if(init_keypad(ROW_gpio,COL_gpio,ROW_pin,COL_pin)!=0){
			return -1;
	}
	if(init_button(BUTTON_gpio, BUTTON_pin) != 0){
		// Fail to init button
		return -1;
	}
	//10MHz 0.001s interrupt
	SystemClock_Config_Interrupt(10, 10000);        //systemclock init
	//init interrupt


	while(1){
	}

	return 0;
}


