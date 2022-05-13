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


void one(){
int shift_direction = 0;
int led_data = 0b000010;
int leds[4] = {LED1_pin, LED2_pin, LED3_pin, LED4_pin};
while(1){
	if(read_gpio(BUTTON_gpio, BUTTON_pin)==0){
		break;
	}


	for(int a=0;a<4;a++){
					// Active Low
					if((led_data>>(a))&0x1){
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
				if(led_data==0b0001 || led_data==0b1000){
					shift_direction = 1-shift_direction;
				}

				// Delay for 1 sec
				delay_without_interrupt(200);
}

}
void two(){
int shift_direction = 0;
int led_data = 0b000011;
int leds[4] = {LED1_pin, LED2_pin, LED3_pin, LED4_pin};
while(1){
	if(read_gpio(BUTTON_gpio, BUTTON_pin)==0){
		break;
	}

	if((led_data>>(1))&0x1)
	{
		reset_gpio(LED_gpio, leds[0]);
	}
	else
	{
		set_gpio(LED_gpio, leds[0]);
	}
	// Set or Reset Leds
	for(int a=1;a<4;a++)
	{
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
	delay_without_interrupt(1000);
}

}
void three(){
int shift_direction = 0;
int led_data = 0b000111;
int leds[4] = {LED1_pin, LED2_pin, LED3_pin, LED4_pin};
while(1){
	if(read_gpio(BUTTON_gpio, BUTTON_pin)==0){
		break;
	}

	for(int a=0;a<4;a++){
			// Active Low
			if((led_data>>(a+1))^0x1){
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
		if(led_data==0b00011 || led_data==0b11000){
			shift_direction = 1-shift_direction;
		}

		// Delay for 1 sec
		delay_without_interrupt(500);
}
}

void led_mode_change(int i){
	if(i == 0){
		one();
	}
	if(i == 1){
		two();
	}
	if(i == 2){
		three();
	}
}
int main(){

#ifdef lab_led

	if(init_led(LED_gpio, LED1_pin) != 0 || init_led(LED_gpio, LED2_pin) != 0 || init_led(LED_gpio, LED3_pin) != 0 || init_led(LED_gpio, LED4_pin) != 0||init_button(BUTTON_gpio,BUTTON_pin)!=0){
		// Fail to init LED
		return -1;}


	int button_press_cycle_per_second = 10;
	// Use to state how many cycles to check per button_press_cycle
	int debounce_cycles = 100;
	// Use to state the threshold when we consider a button press
	int debounce_threshold = debounce_cycles*0.6;
	// Used to implement negative edge trigger 0=not-presses 1=pressed
	int last_button_state=0;
	// Direction to shift 0=left, 1=right
	int change_mode = 0;


	while(1){

		for(int a=0;a<button_press_cycle_per_second;a++){
					// Simple Debounce without interrupt
					int pos_cnt=0;
					for(int a=0;a<debounce_cycles;a++){
						// If button press add count
						if(read_gpio(BUTTON_gpio, BUTTON_pin)==0){
							pos_cnt++;
						}
						delay_without_interrupt(1000/(button_press_cycle_per_second*debounce_cycles));
					}

					// Check if need to change state
					if(pos_cnt>debounce_threshold){
						if(last_button_state==0){
							change_mode++;
						}
						else{
							// Pressed button - Continued pressing
							// Do nothing
						}
						last_button_state = 1;
					}
					else{
						if(last_button_state==0){
							// Released button - Not pressing
							// Do nothing
						}
						else{
							// Released button - Neg edge
							// Do nothing
						}
						last_button_state = 0;
						led_mode_change(change_mode++);
					}

	#endif
	return 0;

		}





