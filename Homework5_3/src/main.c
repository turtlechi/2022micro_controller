	#include "stm32l476xx.h"
#include "helper_functions.h"
#include "keypad_function.h"
#include "gpio_function.h"
#include "led_button.h"
#include "timer.h"

Keypad_TypeDef keypad_Gpio ={
		.out_Gpio[0] = GPIOB,
		.out_Pin[0] = 6,

		.out_Gpio[1] = GPIOA,
		.out_Pin[1] = 7,

		.out_Gpio[2] = GPIOA,
		.out_Pin[2] = 6,

		.out_Gpio[3] = GPIOA,
		.out_Pin[3] = 5,

		.in_Gpio[0] = GPIOB,
		.in_Pin[0] = 10,

		.in_Gpio[1] = GPIOA,
		.in_Pin[1] = 8,

		.in_Gpio[2] = GPIOA,
		.in_Pin[2] = 9,

		.in_Gpio[3] = GPIOC,
		.in_Pin[3] = 7
};


// Define Counter timer
#define COUNTER_timer TIM2
#define gpio GPIOA
#define s_pin 1

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

	init_Keypad(&keypad_Gpio);
	init_sound(gpio,s_pin);

	uint16_t keypad_Input = 0;


	while(1){
		keypad_Input = read_Keypad(&keypad_Gpio);

		for(int i = 0; i < 16; i++){
			if((keypad_Input >> i) & 0x01){
				SystemClock_Config(i);
				send_sound(gpio,s_pin);
				break;
			}
		}

   }



	return 0;
}
