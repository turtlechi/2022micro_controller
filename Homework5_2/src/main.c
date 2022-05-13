#include "stm32l476xx.h"
#include "helper_functions.h"
#include "7seg.h"
#include "led_button.h"
#include "timer.h"

// Define pins for 7seg
#define SEG_gpio GPIOC
#define DIN_pin 3
#define CS_pin 4
#define CLK_pin 5

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
#define lab_modify_system_clock
//#define lab_counter
//#define lab_music_keyboard
//#define lab_music_song

int main(){
	// Cause we want to use floating points we need to init FPU
	FPU_init();

	//7 seg
	if(init_7seg(SEG_gpio,DIN_pin,CS_pin,CLK_pin)!=0){
		return -1;
	}
	send_7seg(SEG_gpio,DIN_pin,CS_pin,CLK_pin,SEG_ADDRESS_DECODE_MODE, 0XFF);
	send_7seg(SEG_gpio,DIN_pin,CS_pin,CLK_pin,SEG_ADDRESS_SCAN_LIMIT, 0X07);
	send_7seg(SEG_gpio,DIN_pin,CS_pin,CLK_pin,SEG_ADDRESS_SHUTDOWN, 0X01);

	for(int i=1;i<8;i++){
		send_7seg(SEG_gpio,DIN_pin,CS_pin,CLK_pin,i, 15);
	}

	double time_sec=12.3;

	if(time_sec<0.0 || time_sec>1000.0){
		display_two_decimal(SEG_gpio,DIN_pin,CS_pin,CLK_pin,0.0);
	}
	else{
		timer_enable(COUNTER_timer);
			timer_init(COUNTER_timer,40000,100);
			timer_start(COUNTER_timer);

			int sec=0, last=0;
			while(1){
				if (last!=COUNTER_timer->CNT){
					if(COUNTER_timer->CNT==0){
						sec++;
					}
					last=COUNTER_timer->CNT;
					double now_time=sec+ COUNTER_timer->CNT/100.0;
					display_two_decimal(SEG_gpio,DIN_pin,CS_pin,CLK_pin,now_time);
					if(now_time==time_sec){
						break;
					}
				}
			}
			timer_stop( COUNTER_timer);
	}



	return 0;
}
