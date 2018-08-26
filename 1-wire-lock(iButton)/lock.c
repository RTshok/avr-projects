#include "onewire.h"
#include "EEPROM.h"
#include "UART.h"
static unsigned char keys[64];
static uint8_t cnt_keys;
static char uart_data[BUFFER];
void soft_delay_us(uint16_t time)
{
	while (time--) {
		_delay_us(1.0);
	}
}

void line_low(void)
{
	PORTB &= ~(1 << 5);
	DDRB |= (1 << 5);
}

void line_release(void)
{
	DDRB &= ~(1 << 5);
	PORTB |= (1 << 5);
}

bool line_read(void)
{
	uint8_t val = PINB & (1<<5);
	return val;
}

/*
void put(uint8_t byte)
{
	PORTB |= 1 << 4;
	soft_delay_us(20);
	PORTB &= ~(1 << 4);
	for (int i=0; i<8; i++) {
		if (byte & (1 << i))
			PORTB |= 1 << 4;
		else
			PORTB &= ~(1 << 4);
		soft_delay_us(20);
	}
}
*/


int main()
{	DDRB |= (1 << 1) ;
	PORTB |= (1 << 5);
	
	uart_init();
	EEPROM_init(keys,cnt_keys);
	cnt_keys = EEPROM_read(1);
	
	/* We use internal pullup resitor for 1-wire line */
	
	ow_Pin pin;
	ow_Pin_init(&pin, &line_low, &line_release, &line_read, &soft_delay_us, 5, 60, 60, 5);
	ow_err err;
	
	uart_print("USAGE : \n");
	soft_delay_us(500);
	uart_print("add_key\n");
	soft_delay_us(500);
	uart_print("show_keys\n");
	soft_delay_us(500);
	uart_print("open\n");
	soft_delay_us(500);
	uart_print("close\n");
	soft_delay_us(500);
	uart_print("keys\n");
	soft_delay_us(1000);
	uart_print("erase\n");
	uint8_t ibutton_id[8];
	uint8_t crc;
	while (1) {
		/* waits for the data from COM-port */
		uart_read_string(uart_data);
		if(comp_func(uart_data,"add_key")){
			/* delay to wait for a key */
			_delay_ms(2000);
			err = ow_cmd_readrom(&pin, &ibutton_id[0], &crc, true, false);
				if (err == OW_EOK) {
					if(check_keys(keys,&ibutton_id[0],cnt_keys)){
					uart_print("Key is already in memory!\n");
					}else{
					add_key(&ibutton_id[0],cnt_keys);
					cnt_keys++;
					EEPROM_write(1,cnt_keys);
					EEPROM_init(keys,cnt_keys);
					soft_delay_us(500);
					uart_print("\nkey has been successfully added!\n");
					}
				} else {
					uart_print("key isn't suitable \n");
			 	 }
		}
		if(comp_func(uart_data,"show_keys")){
			if(cnt_keys == 0){
			uart_print("NO KEYS ! \n");
			}
			else{
			show_keys(keys,cnt_keys);
			}
		}
		if(comp_func(uart_data,"open"))
		{	/* if LED(lock) is unable -> enable */
			if((PINB&(1<<PB1))==0){
				/* delay to wait for a key */
				_delay_ms(2000);
				err = ow_cmd_readrom(&pin, &ibutton_id[0], &crc, true, false);
				if (err == OW_EOK) {
					if(check_keys(keys,ibutton_id,cnt_keys)){
					uart_print("OPENED!\n");
					PORTB|=(1<<1);
					soft_delay_us(500);
					}else
					uart_print("NO KEY\n");
			
				} else {
				uart_print("key isn't suitable \n");
			 	 }
			}
			else
			uart_print("Already opened!\n");
		}
		if(comp_func(uart_data,"close")){
			/* if LED(lock) is enable -> make unable */
			if((PINB&(1<<PB1))!=0){ 
			PORTB &= ~(1<<1);
			uart_print("CLOSED\n");
			}
			else
			uart_print("Already closed\n");
		}	
		if(comp_func(uart_data,"keys")){
		uart_print("AMOUNT OF KEYS :\n");
		uart_transmit(cnt_keys);
		}
		if(comp_func(uart_data,"erase")){
		delete_keys(cnt_keys,keys);
		EEPROM_init(keys,cnt_keys);
		cnt_keys = 0;
		uart_print("DATA HAS BEEN ERASED!\n");
		}
		if(comp_func(uart_data,"HELP")){
		uart_print("USAGE : \n");
		soft_delay_us(500);
		uart_print("ADD_KEY\n");
		soft_delay_us(500);
		uart_print("SHOW_KEYS\n");
		soft_delay_us(500);
		uart_print("OPEN\n");
		soft_delay_us(500);
		uart_print("CLOSE\n");
		soft_delay_us(500);
		uart_print("KEYS\n");
		soft_delay_us(1000);
		uart_print("ERASE\n");
		}
	
	}
}
