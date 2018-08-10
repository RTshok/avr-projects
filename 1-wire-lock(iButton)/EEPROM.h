#include <avr/io.h>
#include <stdbool.h>
void EEPROM_write(unsigned int adress, unsigned char data);
unsigned char EEPROM_read(unsigned int adress);
void add_key(uint8_t *id,uint8_t cnt_keys);
void show_keys(unsigned char * keys,unsigned int cnt_keys);
void delete_keys (unsigned int cnt_keys,unsigned char * keys);
bool check_keys(unsigned char * keys, unsigned char *key_to_check,uint8_t cnt_keys);
void read_keys_from_eeprom (char * keys,uint8_t cnt_keys);
uint8_t read_amount_of_keys(void);
void EEPROM_init(unsigned char *keys, uint8_t cnt_keys);
