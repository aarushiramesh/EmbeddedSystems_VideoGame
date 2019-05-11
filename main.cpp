// main.cpp
// Runs on LM4F120/TM4C123
// Aarushi Ramesh and Melanie Boleng
// This is a starter project for the EE319K Lab 10 in C++

// Last Modified: 4/19/2018 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2017
   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2017
 Copyright 2018 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "SlidePot.h"
#include "Images.h"
#include "UART.h"
#include "Timer0.h"
//#include "Sound.h"
#include "DAC.h"
#include "Timer1.h"
#include "Timer2.h"

void altSongPlay(void);
//extern "C" void Timer2A_Handler(void);

SlidePot my(2000,0);
uint32_t Data = 0;
uint32_t Position = 0;
uint32_t x = 53;
uint32_t y = 137;

uint32_t score = 0;

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);

// Period = 80000000/64/Freq=1250000/Freq
#define C1 597 // 2093 Hz
#define B1 633 // 1975.5 Hz
#define BF1 670 // 1864.7 Hz
#define A1 710 // 1760 Hz
#define AF1 752 // 1661.2 Hz
#define G1 797 // 1568 Hz
#define GF1 845 // 1480 Hz
#define F1 895 // 1396.9 Hz
#define E1 948 // 1318.5 Hz
#define EF1 1004 // 1244.5 Hz
#define D1 1064 // 1174.7 Hz
#define DF1 1127 // 1108.7 Hz
#define C 1194 // 1046.5 Hz
#define B 1265 // 987.8 Hz
#define BF 1341 // 932.3 Hz
#define A 1420 // 880 Hz
#define AF 1505 // 830.6 Hz
#define G 1594 // 784 Hz
#define GF 1689 // 740 Hz
#define F 1790 // 698.5 Hz
#define E 1896 // 659.3 Hz
#define EF 2009 // 622.3 Hz
#define D 2128 // 587.3 Hz
#define DF 2255 // 554.4 Hz
#define C0 2389 // 523.3 Hz
#define B0 2531 // 493.9 Hz
#define BF0 2681 // 466.2 Hz
#define A0 2841 // 440 Hz
#define AF0 3010 // 415.3 Hz
#define G0 3189 // 392 Hz
#define GF0 3378 // 370 Hz
#define F0 3579 // 349.2 Hz
#define E0 3792 // 329.6 Hz
#define EF0 4018 // 311.1 Hz
#define D0 4257 // 293.7 Hz
#define DF0 4510 // 277.2 Hz
#define C7 4778 // 261.6 Hz
#define B7 5062 // 246.9 Hz
#define BF7 5363 // 233.1 Hz
#define A7 5682 // 220 Hz
#define AF7 6020 // 207.7 Hz
#define G7 6378 // 196 Hz
#define GF7 6757 // 185 Hz
#define F7 7159 // 174.6 Hz
#define E7 7584 // 164.8 Hz
#define EF7 8035 // 155.6 Hz
#define D7 8513 // 146.8 Hz
#define DF7 9019 // 138.6 Hz
#define C6 9556 // 130.8 Hz

int ind = 0;         //counter index 

struct music {
	uint16_t note;
	uint32_t duration;
};

struct music song[48] = {
	{A, 1250},			//ms 1
	{AF, 1250},
	{G, 1250},
	{GF, 1250},
	
	{G, 1250},
	{GF, 1250},
	{F, 1250},
	{E, 1250},
	
	{F, 1250},			//ms 2
	{E, 1250},
	{EF, 1250},
	{D, 1250},
	
	{DF, 1250},
	{C, 1250},
	{B, 1250},
	{BF, 1250},
	
	{A, 1250},			//ms 3
	{AF, 1250},
	{G, 1250},
	{GF, 1250},
	
	{G, 1250},
	{GF, 1250},
	{F, 1250},
	{E, 1250},
	
	{F, 1250},			//ms 4
	{E, 1250},
	{EF, 1250},
	{D, 1250},
	
	{DF, 1250},
	{C, 1250},
	{B, 1250},
	{BF, 1250},
	
	{A0, 1250},			//ms 5
	{AF0, 1250},
	{G0, 1250},
	{GF0, 1250},
	
	{G0, 1250},
	{GF0, 1250},
	{F0, 1250},
	{E0, 1250},
	
	{A0, 1250},			//ms 6
	{AF0, 1250},
	{G0, 1250},
	{GF0, 1250},
	
	{G0, 1250},
	{GF0, 1250},
	{F0, 1250},
	{E0, 1250},
};

const uint8_t wave[64] = {32,35,38,41,44,47,49,52,54,56,58,59,61,62,62,63,63,63,62,62,61,59,58,56,54,52,49,47,44,41,38,35,32,29,26,23,20,17,15,12,10,8,6,5,3,2,2,1,1,1,2,2,3,5,6,8,10,12,15,17,20,23,26,29};
uint32_t I = 0;

int32_t leftButton;
int32_t rightButton;

int8_t startGameFlag = 0;

void initButtons() {
	SYSCTL_RCGCGPIO_R |= 0x10;											//activate clock
	while(SYSCTL_RCGCGPIO_R == 0) {}								//wait
	GPIO_PORTE_DIR_R &= 0x03;												//make PE1-0 inputs
	GPIO_PORTE_DEN_R |= 0x03;		
}

void SysTick_Init(uint32_t period) {
	NVIC_ST_CTRL_R=0;														//disable SysTick during setup
	NVIC_ST_RELOAD_R=period;								//maximun reload value
	NVIC_ST_CURRENT_R=0;												//any write to current clears it
	NVIC_ST_CTRL_R=0x00000005;									//enable SysTick with core clock
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;		//set priority to 1
	NVIC_ST_CTRL_R=0x00000005;			
}

void SysTick_Handler(void) {
	uint32_t sample = my.ADCsample();
	my.Save(sample);
}


void reload(void) {
	I = (I+1) & 0x3F;												//increments the index
	DAC_Out(wave[I]);	
}


uint16_t x_brick = (Random32()>>24)%47 + 30;
uint16_t y_brick = 0; //(Random32()>>24)%35 + 50;
uint16_t x_scot = (Random32()>>24)%47 + 30;
uint16_t y_scot = 0; //(Random32()>>24)%35 + 100;
uint16_t x_coins = (Random32()>>24)%47 + 30;
uint16_t y_coins = 0;

uint16_t pickOne = 1;

void fallingObjects(void) {
	if (y_scot >= 190 || y_brick >= 190 || y_coins >=190) { // || y_bevo >= 190) {

		x_brick = (Random32()>>24)%47 + 30;
		x_scot = (Random32()>>24)%47 + 30;
		x_coins = (Random32()>>24)%47 +30;
		y_scot = 0;
		y_brick = 0;
		y_coins = 0;
		pickOne = (Random32()>>24)%3 + 1;
	}
		ind++;
		if(ind == 48) {
			ind = 0;
		}
		if (pickOne == 1) {
			y_scot++;
		}
		if (pickOne == 2) {
			y_brick++;
		}
		if (pickOne == 3) {
			y_coins++;
		}
	
}

int main(void){
	// initializations
	// update coins
	DisableInterrupts();
	PLL_Init(Bus80MHz);
//	Sound_Init();
	initButtons();
	SysTick_Init(1333333);
	ADC_Init();
	DAC_Init();
	EnableInterrupts();
	
	SlidePot *SlideP = new SlidePot(2,16);
	ST7735_InitR(INITR_REDTAB);
	ST7735_DrawBitmap(0, 160, starterScreen, 128, 160);
	
	while(startGameFlag == 0) {
		leftButton = (GPIO_PORTE_DATA_R & 0x00000002)>>1;
		if(leftButton == 1) {
			startGameFlag = 1;
		}
	}
	
	ST7735_FillScreen(0);
	ST7735_DrawBitmap(30, 160, gameBackground, 69, 160);
	Timer0_Init(&fallingObjects, 1333333);
	Timer1_Init(&reload, song[ind].note);
	
	while(startGameFlag == 1) {
		TIMER1_TAILR_R = song[ind].note;
		rightButton = GPIO_PORTE_DATA_R & 0x00000001;
		my.Sync();
		Data = ADC_In();
		Position = my.Convert(Data);
		ST7735_DrawBitmap(Position, 160, runningFenves, 23, 23);
		x = Position;
		
		if (pickOne == 1) {
			ST7735_DrawBitmap(x_scot, y_scot+1, scooter, 23, 23);
			//y_scot++;
		}
		if (pickOne == 2) {
			ST7735_DrawBitmap(x_brick, y_brick+1, bricks, 23, 23);
			//y_brick++;
		}
		if (pickOne == 3) {
			ST7735_DrawBitmap(x_coins, y_coins+1, coin, 23, 23);
			//y_coins++;
		}
		
		//game over sequence thing
		// brick detection
		if (y >= y_brick-23 && y <= y_brick) {
			if (x >= x_brick && x <= x_brick+23) {
				ST7735_FillScreen(0);
				ST7735_DrawBitmap(0, 80, gameOver, 122, 80);
				ST7735_SetCursor(0, 90);
				ST7735_SetTextColor(ST7735_YELLOW);
				ST7735_OutUDec(score);
				startGameFlag = 0;
			}
		}
		
		// scooter detection
		if (y >= y_scot-23 && y <= y_scot) {
			if (x >= x_scot && x <= x_scot+23) {
				ST7735_FillScreen(0);
				ST7735_DrawBitmap(0, 80, gameOver, 122, 80);
				ST7735_SetCursor(0, 90);
				ST7735_SetTextColor(ST7735_YELLOW);
				ST7735_OutUDec(score);
				startGameFlag = 0;
			}
		}
		
		
		if (y >= y_coins-23 && y <= y_coins) {
			if (x >= x_coins && x <= x_coins+23) {
				//ST7735_FillScreen(0);
				score++;
				//ST7735_SetCursor(0, 90);
				//ST7735_SetTextColor(ST7735_YELLOW);
				//ST7735_OutUDec(score);
				//startGameFlag = 0;
			}
		}
		
		if (rightButton == 1) {
				ST7735_FillScreen(0);
				ST7735_DrawBitmap(0, 80, gameOver, 122, 80);
				ST7735_SetCursor(0, 90);
				ST7735_SetTextColor(ST7735_YELLOW);
				ST7735_OutUDec(score);
				startGameFlag = 0;
		}
	}
}
