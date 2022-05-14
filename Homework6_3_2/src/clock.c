#include "clock.h"
#include "helper_functions.h"

void clock_Init(){

}


void countdown_timer(GPIO_TypeDef* gpio, int DIN, int CS, int CLK,uint16_t keypad_Input){
	TIM_TypeDef* counter_timer= TIM3;
	if(keypad_Input<0.0 || keypad_Input>1000.0){
			display_number(gpio, DIN,CS,CLK,0,2);
		}
	else{
		timer_enable(counter_timer);
		timer_init(counter_timer,40000,100);    //0.01¬ícounter+1
		timer_start(counter_timer);

			int sec=keypad_Input, last=0;
			while(1){
				if (last!=counter_timer->CNT){
					if(counter_timer->CNT==0){
						sec--;
					}
					last=counter_timer->CNT;
					double now_time=sec;
					display_number(gpio, DIN,CS,CLK,sec,2);
					if(now_time==0){

						break;
					}
				}
			}
			timer_stop(counter_timer);
	}
}

void music_Play(int note){
	TIM2->ARR = note_Arr[note - 1];
	TIM2->CCR1 = (note_Arr[note - 1]*music_Duty_Cycle)/100;

	timer_start(TIM2);

	return;
}

void music_Init(){

	// Timer_PWM_Init_Data timer_PWM_Data = {
			// .channel = 1,
			//.PSC = (40 - 1),
			//.ARR = 100,
			//.CCRx = 50,
//	};

	timer_enable(TIM2);
	PWM_channel_init();
	GPIO_init_AF();              //PA0
	timer_reload(TIM2);
	timer_start(TIM2);

	return;
}

void EXTI_Setup(){
	//Enable SYSCFG CLK
	RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN;
	// Seletct output bits
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3_Msk; 		//0b000<<12
	SYSCFG->EXTICR[0] |= (1 << SYSCFG_EXTICR1_EXTI3_Pos);	//0b001<<12
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI4_Msk;			//0b000<<0
	SYSCFG->EXTICR[1] |= (1 << SYSCFG_EXTICR2_EXTI4_Pos);	//0b001<<0
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5_Msk;			//0b000<<4
	SYSCFG->EXTICR[1] |= (1 << SYSCFG_EXTICR2_EXTI5_Pos);	//0b001<<4
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI6_Msk;			//0b000<<8
	SYSCFG->EXTICR[1] |= (1 << SYSCFG_EXTICR2_EXTI6_Pos);	//0b001<<8
	//Enable interrupt
	EXTI->IMR1 |= EXTI_IMR1_IM3;	//0b001<<3
	EXTI->IMR1 |= EXTI_IMR1_IM4;	//0b001<<4
	EXTI->IMR1 |= EXTI_IMR1_IM5;	//0b001<<5
	EXTI->IMR1 |= EXTI_IMR1_IM6;	//0b001<<6
	//Enable Falling Edge
	EXTI->FTSR1 |= EXTI_FTSR1_FT3;	//0b001<<3
	EXTI->FTSR1 |= EXTI_FTSR1_FT4;	//0b001<<4
	EXTI->FTSR1 |= EXTI_FTSR1_FT5;	//0b001<<5
	EXTI->FTSR1 |= EXTI_FTSR1_FT6;	//0b001<<6
	//Enable NVIC**
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);

	//user button
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13_Msk;
	SYSCFG->EXTICR[3] |= (1 << SYSCFG_EXTICR4_EXTI13_Pos);
	EXTI->IMR1 |= EXTI_IMR1_IM13;	//0b001<<13
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;	//0b001<<13
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	//set priority
	NVIC_SetPriority(EXTI15_10_IRQn,0);
	NVIC_SetPriority(SysTick_IRQn,1);
	NVIC_SetPriority(EXTI3_IRQn,2);
	NVIC_SetPriority(EXTI4_IRQn,2);
	NVIC_SetPriority(EXTI9_5_IRQn,2);

}


