#define F_CPU 16000000UL
#include "segm.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
uint8_t test[] ={0x3F,~0x40,0x3F,0x3F};
static uint8_t seconds = 0;
static uint8_t minutes = 0;
static uint8_t hours = 0;
const uint8_t segm_sym_table_dot[] ={

	[0] = ~0x40,
	[1] = ~0x79,
	[2] = ~0x24,
	[3] = ~0x30,
	[4] = ~0x19,
	[5] = ~0x12,
	[6] = ~0x2,
	[7] = ~0x78,
	[8] = ~0x0,
	[9] = ~0x10

};
const uint8_t segm_sym_table[] = {
	/* Common cathode here */
	[0] = 0x3F,
	[1] = 0x06,
	[2] = 0x5B,
	[3] = 0x4F,
	[4] = 0x66,
	[5] = 0x6D,
	[6] = 0x7D,
	[7] = 0x07,
	[8] = 0x7F,
	[9] = 0x6F
};
static void sleep_ms(volatile uint16_t N)
{
	/* If volatile is not used, AVR-GCC will optimize this stuff out     */
        /* making our function completely empty                              */
	volatile uint8_t inner = 0xFF;
	while (N--) {
		while (inner--);
	}
}

void segm_min(uint8_t number){
uint8_t x = number/10;
uint8_t y = number%10;
test[2] = segm_sym_table[x];
test[3] = segm_sym_table[y];
} 
void segm_hours(uint8_t number){
uint8_t x = number/10;
uint8_t y = number%10;

test[0] = segm_sym_table[x];
test[1] = segm_sym_table_dot[y];
}

static struct segm_Port PB = {
	.DDR = &DDRB,
	.PIN = &PINB,
	.PORT = &PORTB,
};

static struct segm_Display display = {
	.SHCP = {.port = &PB, .pin = 4},
	.STCP = {.port = &PB, .pin = 5},
	.DS   = {.port = &PB, .pin = 6},
	/* We have common cathode display */
	.delay_func = &_delay_loop_1,	/* 3 cycles / loop, busy wait */
	.sleep_ms_func = &sleep_ms,	/* 3 cycles / loop, busy wait */
	.is_comm_anode = false	
};

ISR(INT0_vect,ISR_BLOCK)	/*handling  button interrupt*/
{
	if((PIND & (1 << PD0))==0){
	sleep_ms(100);
	if((PIND & (1 << PD0))==0){
	if(minutes<59){
	minutes++;
	}
	else{
	minutes =0;
	hours++;
	}
	seconds = 0;
	segm_min(minutes);
	segm_hours(hours);
	TCNT1 = 49911;
	}
	}

}
ISR(INT1_vect,ISR_BLOCK){	/*handling hour button interrupt*/
	if((PIND & (1 << PD1))==0){
	sleep_ms(100);
	if((PIND & (1 << PD1))==0){
	if(hours>=23){
	hours = 0;
	segm_hours(hours);
	}else{
	hours++;
	segm_hours(hours);
	}
	seconds = 0;
	TCNT1 = 49911;
	}
	}
}
ISR(TIMER1_OVF_vect)/*handling timer interrupt*/
{
	if(seconds<59)
	seconds++;
	else{
	seconds = 0;
	minutes++;
	}
	if(minutes>=60)
	{
	minutes = 0;
	hours++;
	}
	if(hours>23)
	hours = 0;
	segm_min(minutes);
	segm_hours(hours);
	TCNT1 = 49911;
}

void INTinit()/*external interrupt*/
{
EIMSK|= (1<<INT0)|(1<<INT1);/*enabling interrupts*/
EICRA|= (1<<ISC01)|(1<<ISC11);/*configure the interruptions by falling edge*/
DDRD|=(1<<PD0)|(1<<PD1);
PORTD|=(1<<PD0)|(1<<PD1);
}

void timer_init(){
TCCR1B|=(1<<CS12)|(1<<CS10);/*divider by 1024*/
TIMSK1|=(1<<TOIE1);/*enabling interrupt by overflow*/
TCNT1=49911;/*initial cnt. 15625 is 1 second, 65536-15625 = 49911*/
}

void leonardo_interrupt(){
/*enabling interruptions on leonardo*/
USBINT = 0;
UDINT  = 0;
for (uint8_t _i = 0; _i < 6; _i++) { // For each USB endpoint
    UENUM = _i; // select the _i-th endpoint
    UEINTX = UEIENX = 0; // Clear interrupt flags for that endpoint
}   
}

int main ()
{
leonardo_interrupt();
segm_init(&display);
INTinit();
sei();
timer_init();

while(1)
{
segm_indicate4(&display, test);
				
}
}