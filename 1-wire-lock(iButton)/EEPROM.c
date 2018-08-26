#include "EEPROM.h"
#include "UART.h"
#include "onewire.h"
#include <stdlib.h>

void EEPROM_write(unsigned int adress, unsigned char data){
	/* Wait for completion of previous write */
    	while(EECR & (1<<EEPE));
    /* Set up address and data registers */
	EEAR = adress;
	EEDR = data;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
    /* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}

unsigned char EEPROM_read(unsigned int adress){
 /* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
    /* Set up address register */
	EEAR = adress;
    /* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
    /* Return data from data register */
	return EEDR;
}
void add_key(uint8_t *id,uint8_t cnt_keys){
	/* That variable is to count till the last element in ID array, to 7 */
	uint8_t j = 0;
	/* Multiplying cnt_keys by 8,gives us the place where we should put the key in EEPROM */
	for(uint8_t i = (8*cnt_keys); i < (8*cnt_keys)+8;i++){
	EEPROM_write (i+2,id[j]);
	j++;

	}

}
void show_keys(unsigned char * keys,unsigned int cnt_keys){
	
	uart_print("The keys : \n");
	for(uint8_t j = 0; j<cnt_keys;j++){
	uart_print("Key number :");
	uart_transmit (j);
	uart_transmit('\n');
		/* Printing out the array of keys. If j is 0, then cycle for looks like (i=0;i<8;i++).*/
		for(uint8_t i =(8*j); i<8+(8*j);i++){
		uart_transmit(keys[i]);
		}
	uart_transmit('\n');
	}

}

void delete_keys (unsigned int cnt_keys,unsigned char * keys){
	/* Filling array of keys with zeros, also write zeros to memory */
	for (uint8_t j = 0; j<cnt_keys;j++){
		for(uint8_t i = (8*j) ;i<(8*j)+8;i++){
		EEPROM_write (2+i,0);
		keys[i] = 0;
		}
	}
	/* Writing zero to first adress of EEPROM, where is the data about amount of keys */
	cnt_keys = 0;
	EEPROM_write(1,0);
}

bool check_keys(unsigned char * keys, unsigned char *key_to_check,uint8_t cnt_keys){
	
	bool key_true = false;
	/* CRC in iButton is located in the last place of key.So use 7 */
	uint8_t crc_place = 7;
	uint8_t crc_to_check = ow_crc8_fast_arr(key_to_check,7);
	/* Compare the CRC of keys that already in memory, and the key's CRC*/
	for(uint8_t i = 0; i<cnt_keys;i++){
		if(keys[crc_place+(8*i)] == crc_to_check){
		/* If CRC's are the same, return true */
		key_true = true;
		}
	}
	return key_true;
}

void EEPROM_init(unsigned char *keys, uint8_t cnt_keys){
	/* Reading from the memory about amount of keys */
	cnt_keys = EEPROM_read(1);
	/* If there are some keys */
	if(cnt_keys!=0){
		
		for (uint8_t j = 0; j<cnt_keys;j++){
			/* If j is 0, then cycle for looks like (i=0;i<8;i++). And so on.*/
			for(uint8_t i = j*8;i<(j*8)+8;i++){
			/* Fill the array with that keys.Begin with second adress,where located the start byte of keys. */
			keys[i] = EEPROM_read(2+i);
			soft_delay_us(25);
			}
		
		}
	}
}
