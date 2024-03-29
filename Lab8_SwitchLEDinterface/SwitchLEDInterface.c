// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

// ***** 2. Global Declarations Section *****
unsigned long input;	// input from the switch
// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortE_Init(void);	// Initialize PortE
void Delay1ms(unsigned long msec);	// Delay function
// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
	PortE_Init();	// Initialize Port
  EnableInterrupts();           // enable interrupts for the grader

	while(1){
    input = GPIO_PORTE_DATA_R & 0x01;	// read PE0 to input
		if(input){
			GPIO_PORTE_DATA_R ^= 0x02;	// toggle output PE1
			Delay1ms(100);
			
  }
		else {
			GPIO_PORTE_DATA_R |= 0x02;	// output to PE1 set to high
	}
}
}

void PortE_Init(void) {volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x10;     // 1) Port E clock
  delay = SYSCTL_RCGC2_R;           // wait 3-5 bus cycles
	GPIO_PORTE_DIR_R |= 0x02;	// Set PE1 as output
	GPIO_PORTE_DIR_R &= ~0x01; // Set PE0 as input
	GPIO_PORTE_AFSEL_R &= ~0x03;			// no alternate function on PE0, PE1
	GPIO_PORTE_AMSEL_R &= ~0x03;			// disable analog on PE0, PE1
	GPIO_PORTE_PCTL_R &= ~0x000000FF;			// GPIO clear bit PCTL on PE0, PE1
	GPIO_PORTE_DEN_R |= 0x03;					// enable digital pins on PE0, PE1
}

// Subroutine to delay in units of milliseconds
// Inputs:  Number of milliseconds to delay
// Outputs: None
// Notes:   assumes 80 MHz clock
void Delay1ms(unsigned long msec){
	unsigned long i;
	while (msec>0){
		i = 13333;				// 1 ms, 6 cycles per loop
		while(i>0){
			i -=1;
		}
		msec -=1;		// decrement every microsecond
	}
}

