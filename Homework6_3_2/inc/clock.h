#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "stm32l476xx.h"
#include "timer.h"
#include "7seg.h"
#include "keypad.h"

int music_Duty_Cycle;

void music_Init();
void music_Play(int note);

void countdown_timer(GPIO_TypeDef* gpio, int DIN, int CS, int CLK,uint16_t keypad_Input);

uint32_t note_Arr[8];

#endif
