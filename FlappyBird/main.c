#include "oled.h"
#include "segm.h"
#include "bird_variables.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
/* Variables for congruential method */
static uint8_t a = 7;
static uint8_t c = 8;
static uint8_t m = 31;
static uint8_t seed = 10;
/* --------------------- */
OLED display;
uint8_t x=147;
uint8_t x1=227;
uint8_t bird_pos = 25;
uint8_t score = 0;
uint8_t width_of_block = 20;
uint8_t height_of_window = 25;
uint8_t RAND[2];
/*---------------------- */
/* Array for random position of the block */
const uint8_t BIRD [] = {
0x20, 0x38, 0xCE, 0xC3, 0xF1, 0xC1, 0xC1, 0x81, 0x8D, 0xE5, 0x23, 0x2C, 0x38, 0x00, 
};
uint8_t indication_arr[] ={0x3F,0x3F,0x3F,0x3F};

/* Congruential method */
static uint8_t my_rand(){
seed = (a*seed+c)%m;
return seed;
}
/* The function which divides the score on orders */
static void get_number(uint8_t score);

static void get_number(uint8_t score){
uint8_t ones = score%10; 
indication_arr[3] = segm_sym_table[ones] ;
uint8_t decades = score/10; 
indication_arr[2] = segm_sym_table[decades];
uint8_t hundreds = score/100;
indication_arr[1] = segm_sym_table[hundreds];
uint8_t thousands = score/1000;
indication_arr[0] = segm_sym_table[thousands];
}

struct segm_Port PB = {
	.DDR = &DDRB,
	.PIN = &PINB,
	.PORT = &PORTB,
};
struct segm_Display sev_segm_display = {
	.SHCP = {.port = &PB, .pin = 4},
	.STCP = {.port = &PB, .pin = 5},
	.DS   = {.port = &PB, .pin = 6},
	/* We have common cathode display */
	.delay_func = &_delay_loop_1,	/* 3 cycles / loop, busy wait */
	.sleep_ms_func = &sleep_ms,	/* 3 cycles / loop, busy wait */
	.is_comm_anode = false	
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
ISR(TIMER1_OVF_vect)/*handling timer interrupt*/
{
	/*Deleting previous bird, and put it on the new position*/
	OLED_WITH_SPINLOCK(&display) {
	OLED_put_masked_region(&display,30,bird_pos,BIRD,13,9,4);
	bird_pos++;
	OLED_put_masked_region(&display,30,bird_pos,BIRD,13,9,3);
	}
	TCNT1 = 64500;//64500
}


ISR(INT2_vect,ISR_BLOCK){	/*handling button interrupt*/
	/* debouncing */	
	if((PIND & (1 << PD2))==0){
	sleep_ms(60);
		if((PIND & (1 << PD2))==0){
		/*Deleting previous bird, and put it on the new position*/
		OLED_WITH_SPINLOCK(&display) {
		OLED_put_masked_region(&display,30,bird_pos,BIRD,13,9,4);
		bird_pos--;
		OLED_put_masked_region(&display,30,bird_pos,BIRD,13,9,3);
		}
		/* Reset the timer to default */
		TCNT1 = 64500;
		}
	}
}






int main(){
/* Disabling USB interrupts on leonardo */
leonardo_interrupt();
/* Enabling seven_segment display */
segm_init(&sev_segm_display);
sei();
/* Array for the OLED display */
uint8_t frame_buffer[1024] = {0};
__OLED_init(&display,128,64,frame_buffer,800000,0b0111100);
for(uint8_t i = 0; i<1; i++){
RAND[i] = my_rand();
	if(RAND[i] <=10)
	RAND[i]= RAND[i] + 10;
}
/* Initialiazing position of bird variables */

/* Put the bird on the OLED display */
OLED_put_masked_region(&display,30,bird_pos,BIRD,13,9,1);
timer_init();
INTinit();
while(1){
	
	/* Making operation of moving blocks - atomic */
	OLED_WITH_SPINLOCK(&display){
		if(x>width_of_block){
		/* Putting block on the OLED display */
		OLED_put_rectangle(&display,x,0,x-width_of_block,RAND[0],OLED_FILL | 1);
		OLED_put_rectangle(&display,x,RAND[0]+height_of_window,x-width_of_block,63,OLED_FILL | 1);
		OLED_put_rectangle(&display,x+1,0,127,RAND[0],OLED_FILL | 0);
		OLED_put_rectangle (&display,x+1,RAND[0]+20,127,63,OLED_FILL | 0);
		}else
		OLED_put_rectangle (&display,x+1,0,x+1,63,OLED_FILL | 0);
		
	}
	if(x<=0){
	/* Generate new position for the window */
	OLED_WITH_SPINLOCK(&display){
	OLED_put_rectangle(&display,0,0,0,63,OLED_FILL | 0);
	}
	RAND[0] = my_rand();
		if(RAND[0] <=10)
		RAND[0]= RAND[0] + 10;	
	x=147;
	
	}
	x=x-1;
	
	OLED_WITH_SPINLOCK(&display) {
		if(x1>width_of_block){
		/* Putting block on the OLED display */
		OLED_put_rectangle(&display,x1,0,x1-width_of_block,RAND[1],OLED_FILL | 1);
		OLED_put_rectangle(&display,x1,RAND[1]+height_of_window,x1-width_of_block,63,OLED_FILL | 1);
		OLED_put_rectangle(&display,x1+1,0,127,RAND[1],OLED_FILL | 0);
		OLED_put_rectangle (&display,x1+1,RAND[1]+20,127,63,OLED_FILL | 0);
		}else
		OLED_put_rectangle (&display,x1+1,0,x1+1,63,OLED_FILL | 0);
	
	
	}
	if(x1<=0){
	OLED_WITH_SPINLOCK(&display) {
	OLED_put_rectangle(&display,0,0,0,63,OLED_FILL | 0);
	}
	RAND[1] = my_rand();
		if(RAND[1] <=10)
		RAND[1]= RAND[1] + 10;	
	x1=147;
	}
	x1=x1-1;
	
	/* Making the borders for the bird. Y axis */
	/* If the bird not in that borders: set score to zero and clean the screen  */
	if(bird_pos>=54 || bird_pos <=9){
	OLED_WITH_SPINLOCK(&display) {
	OLED_put_rectangle(&display,0,0,127,63,OLED_FILL | 0);
	bird_pos = 30;
	OLED_put_masked_region(&display,30,bird_pos,BIRD,13,9,3);
	}
	RAND[0] = my_rand();
	RAND[1] = my_rand();
	x = 147;
	x1 =227;
		score = 0;
		get_number(score);
		segm_indicate4(&sev_segm_display, indication_arr);
	}
	/* If the bird not in that borders: set score to zero and clean the screen  */
	if(x <= 50 && x>=30){
		if(bird_pos >=RAND[0]+height_of_window-3 || bird_pos <= RAND[0]+3)
		{
		OLED_WITH_SPINLOCK(&display) {
		OLED_put_rectangle(&display,0,0,127,63,OLED_FILL | 0);
		OLED_put_masked_region(&display,30,bird_pos,BIRD,13,9,4);
		}
		bird_pos = 20;
		RAND[0] = my_rand();
		RAND[1] = my_rand();
		x = 147;
		x1 =227;
		score = 0;
		get_number(score);
		segm_indicate4(&sev_segm_display, indication_arr);
		}
	}
	/* If the bird flew through the window without touching the block, increment the score */
	if(x == 28){
	score++;
	get_number(score);
	segm_indicate4(&sev_segm_display, indication_arr);
	}
	
	if(x1 <= 50 && x1>=30){
		if(bird_pos >=RAND[1]+height_of_window-3 || bird_pos <= RAND[1]+3)
		{
		OLED_WITH_SPINLOCK(&display) {
		OLED_put_rectangle(&display,0,0,127,63,OLED_FILL | 0);
		OLED_put_masked_region(&display,30,bird_pos,BIRD,13,9,4);
		}
		bird_pos = 20;
		RAND[1] = my_rand();
		x = 147;
		x1 =227;
		score = 0;
		get_number(score);
		segm_indicate4(&sev_segm_display, indication_arr);
		}
	}
	
	if(x1 == 28){
	score++;
	get_number(score);
	segm_indicate4(&sev_segm_display, indication_arr);
	}
	segm_indicate4(&sev_segm_display, indication_arr);
	//OLED_put_masked_region(&display,30,20-test+1,BIRD,13,9,4);
	
	OLED_refresh(&display);
}

}
