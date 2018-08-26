#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include "UART.h"


static uint8_t a = 100;
static uint8_t c = 100;
static uint8_t m = 249;
static uint8_t seed = 100;
static uint8_t rand =0;
static void soft_delay(volatile uint16_t N)
{
	/* If volatile is not used, AVR-GCC will optimize this stuff out     */
        /* making our function completely empty                              */
	volatile uint8_t inner = 0xFF;
	while (N--) {
		while (inner--);
	}
}


static uint8_t my_rand(){
seed = (a*seed+c)%m;
return seed;
}

void InitPWM() {
 	TCCR0A |= (1<<WGM00)|(1<<COM0A1); 
	TCCR0B |=(1<<CS00);
	DDRB|=(1 << 7);
	  
}	   

void SetPWMOutput(uint8_t duty)
{
   OCR0A=duty;
}


int main(void)
{
   /*Initialize PWM Channel 0*/
	
	InitPWM();
while(1){
	rand = my_rand();
	if(rand>=130){
	SetPWMOutput(rand);
	soft_delay(300);
	}
	if(rand>100 && rand<130){
	SetPWMOutput(rand);
	soft_delay(150);
   	}
	if(rand>60 && rand <=100){
	SetPWMOutput(rand);
	soft_delay(50);
	}
	uart_transmit(rand);
}
}