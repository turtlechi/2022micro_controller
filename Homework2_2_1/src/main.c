 #include "stm32l476xx.h"
#include "helper_functions.h"
#include "led_button.h"
#include "7seg.h"

// Define pins for 4 leds
#define LED_gpio GPIOB
#define LED1_pin 3
#define LED2_pin 4
#define LED3_pin 5
#define LED4_pin 6

// Define pins for button (default use on-board button PC13)
#define BUTTON_gpio GPIOC
#define BUTTON_pin 13

// Define pins for 7seg
//#define SEG_gpio GPIOB
//#define DIN_pin 3
//#define CS_pin 4
//#define CLK_pin 5
// Use to decide which part of the code will run
// Use define & ifdef to control
#define lab_led
//#define lab_led_button
//#define lab_7seg_non_decode
//#define lab_7seg_decode




int main(){

#ifdef lab_led

	if(init_led(LED_gpio, LED1_pin) != 0 || init_led(LED_gpio, LED2_pin) != 0 || init_led(LED_gpio, LED3_pin) != 0 || init_led(LED_gpio, LED4_pin) != 0||init_button(BUTTON_gpio,BUTTON_pin)!=0){
		// Fail to init LED
		return -1;}

	int state=1;
	int cycle_per_second=10;
	int debounce_cycles=100;
	int debounce_thereshold=70;
	int last_button_state=0;
	int shift_direction = 0;
	int led_data = 0b000011;
	int leds[4] = {LED1_pin, LED2_pin, LED3_pin, LED4_pin};

	while(1){
		for(int a=0;a<cycle_per_second;a++){
			int pos_num=0;
			for(int b=0;b<debounce_cycles;b++){
				if(read_gpio(BUTTON_gpio,BUTTON_pin)==0){
					pos_num++;
				}
			}
			if (pos_num>debounce_thereshold){
				if(last_button_state==0){
					state=1;
					}
				last_button_state=1;
				}
			else{
				if(last_button_state==1){
					state=0;
				}
				last_button_state=0;
			}
		}

		if(state==1){

		for(int a=0;a<4;a++){
				// Active Low
				if((led_data>>(a+1))&0x1){
					reset_gpio(LED_gpio, leds[a]);
				}
				else{
					set_gpio(LED_gpio, leds[a]);
				}
			}

			// Shift led_data
			if(shift_direction==0){
				led_data = (led_data<<1);
			}
			else{
				led_data = (led_data>>1);
			}

			// Check to see if change shift direction
			if(led_data==0b000011 || led_data==0b110000){
				shift_direction = 1-shift_direction;
			}

			// Delay for 1 sec
			delay_without_interrupt(500);
	}
}
	#endif
	while(1){}
	return 0;
}


