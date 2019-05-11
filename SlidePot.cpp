// SlidePot.cpp
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 3/28/2018 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "SlidePot.h"
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void){ 
	SYSCTL_RCGCADC_R |= 0x01;						//activate ADC0
	SYSCTL_RCGCGPIO_R |= 0x08;					//turn on clock for port D
	while((SYSCTL_PRGPIO_R&0x08) != 0x08){};	//wait
	GPIO_PORTD_DIR_R &= 0x04;						//make PD2 an input
	GPIO_PORTD_AFSEL_R |= 0x04;					//enable alternative function on PD2
	GPIO_PORTD_DEN_R &= 0x04;						//disable digital I/0 on PD2
	GPIO_PORTD_AMSEL_R |= 0x04;					//enable analog functionality on PD2
		
	//get the ADC ready
  ADC0_PC_R = 0x01;               //configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;          // 8) Seq 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+5;       // 11) Ain5
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
}

//------------ADCIn------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  
  uint32_t result;
  ADC0_PSSI_R = 0x0008;            // 1) start ADC
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion, clear flag
  return result;
}

// constructor, invoked on creation of class
// m and b are linear calibration coeffients 
SlidePot::SlidePot(uint32_t m, uint32_t b){
// initialize all private variables
// make slope equal to m and offset equal to b
	double l = m/(100);
	
	this->data = 0;
	this->slope = l;
	this->offset = b;
	this->distance = (l * this->data + b)/4096;
	this->flag = 0; 
}

void SlidePot::Save(uint32_t n){
// 1) save ADC sample into private variable
// 2) calculate distance from ADC, save into private variable
// 3) set semaphore flag = 1
	this->data = n;
	this->distance = (this->data * this->slope + this->offset)/4096;
	this->flag = 1;
}
uint32_t SlidePot::Convert(uint32_t n){
// use calibration data to convert ADC sample to distance
//y = -0.0005x + 1.9503
	uint32_t distance = (0.014677 *n + 16.08);
  return distance; // replace this with solution 
}

void SlidePot::Sync(void){
// 1) wait for semaphore flag to be nonzero
// 2) set semaphore flag to 0
	while(this->flag != 0) {    //keeps on checking the flag
		this->flag = 0; 
	} 
}

uint32_t SlidePot::ADCsample(void){ // return ADC sample value (0 to 4095)
// return last calculated ADC sample
	return this->data;
}

uint32_t SlidePot::Distance(void){  // return distance value (0 to 2000), 0.001cm
//return last calculated distance in 0.001cm
  return this->distance; // replace this with solution
}
