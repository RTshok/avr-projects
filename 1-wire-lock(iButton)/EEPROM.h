#include <avr/io.h>
#include <stdbool.h>

/**
* EEPROM_write - writing byte to the adress in EEPROM memory
* @adress - the adress where you want to put the data
* @data - the data which you want to put into the memory
*/

void EEPROM_write(unsigned int adress, unsigned char data);

/**
* EEPROM_read - writing byte to the adress in EEPROM memory
* @adress - the adress from what you want to read the data.
*/

unsigned char EEPROM_read(unsigned int adress);

/**
* add_key - writing key byte by byte in memory.
* @id - it's actually the key, which we want to add to the memory.
* @cnt_keys - amount of keys.
*
* We use cnt_keys to know where we should put the key because of avoiding erasing another data.
*
*/

void add_key(uint8_t *id,uint8_t cnt_keys);

/**
* show_keys - printing out the keys
* @keys - it's the array with keys.
* @cnt_keys - amount of keys.
*
* That func is printing data from the array, one by one.
*
*/

void show_keys(unsigned char * keys,unsigned int cnt_keys);

/**
* delete_keys - filling the memory with zeros.
* @cnt_keys - amount of keys.
* @keys - the array with the keys
*
* Memory and the array with keys are filling with zeros, and begin to carry no data.
* Also the cnt_keys is 0. Because we erase all the keys.
*
*/

void delete_keys (unsigned int cnt_keys,unsigned char * keys);

/**
* check_keys - checks for coincidence beetween CRC of inputed key, and the CRC of keys that are already available.
* @cnt_keys - amount of keys.
* @keys - the array with the keys
* @key_to_check - the key that is going to be checked if it available in the memory.
*/

bool check_keys(unsigned char * keys, unsigned char *key_to_check,uint8_t cnt_keys);

/**
* EEPROM_init - reading the data from memory and put it in the array.
* @keys - the array where we put the data
* @cnt_keys - the amount of keys.
*/

void EEPROM_init(unsigned char *keys, uint8_t cnt_keys);

