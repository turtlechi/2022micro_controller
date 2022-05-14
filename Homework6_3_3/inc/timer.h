#ifndef __timer__H__
#define __timer__H__

#include "stm32l476xx.h"

typedef struct tim_Init_Data_Struct{
	int PSC;
	int ARR;
}Timer_Init_Data;

typedef struct tim_PWM_Init_Data_Struct{
	uint8_t channel;
	int PSC;
	int ARR;
	int CCRx;
	//uint8_t CCxP 0 = active high 1 = active low
}Timer_PWM_Init_Data;

void SystemClock_Config(int speed);
void SystemClock_Config_Interrupt(int speed, int load);

void timer_enable(TIM_TypeDef *timer);
void timer_disable(TIM_TypeDef *timer);
void timer_init(TIM_TypeDef *timer, int psc, int arr);
void timer_reload(TIM_TypeDef *timer);
void timer_start(TIM_TypeDef *timer);
void timer_stop(TIM_TypeDef *timer);

void GPIO_init_AF();
void PWM_channel_init();

#endif
